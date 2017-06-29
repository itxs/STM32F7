#include "usbd_video_core.h"
#include "uvc.h"
#include "test_pic1.h"


/*********************************************
   VIDEO Device library callbacks
 *********************************************/
static uint8_t  usbd_video_Init       (struct _USBD_HandleTypeDef *pdev, uint8_t cfgidx);
static uint8_t  usbd_video_DeInit     (struct _USBD_HandleTypeDef *pdev, uint8_t cfgidx);
static uint8_t  usbd_video_Setup      (struct _USBD_HandleTypeDef *pdev, USBD_SetupReqTypedef *req);
static uint8_t  usbd_video_EP0_RxReady(struct _USBD_HandleTypeDef *pdev);
static uint8_t  usbd_video_DataIn     (struct _USBD_HandleTypeDef *pdev, uint8_t epnum);
static uint8_t  usbd_video_DataOut    (struct _USBD_HandleTypeDef *pdev, uint8_t epnum);
static uint8_t  usbd_video_SOF        (struct _USBD_HandleTypeDef *pdev);
static uint8_t  usbd_video_OUT_Incplt (struct _USBD_HandleTypeDef *pdev, uint8_t epnum);
static uint8_t  usbd_video_IN_Incplt  (struct _USBD_HandleTypeDef *pdev, uint8_t epnum);

/*********************************************
   VIDEO Requests management functions
 *********************************************/
static void VIDEO_Req_GetCurrent(struct _USBD_HandleTypeDef *pdev, USBD_SetupReqTypedef *req);
static void VIDEO_Req_SetCurrent(struct _USBD_HandleTypeDef *pdev, USBD_SetupReqTypedef *req);
static uint8_t  *USBD_video_GetCfgDesc (uint16_t *length);
static void VIDEO_Req_GetRes(struct _USBD_HandleTypeDef *pdev, USBD_SetupReqTypedef *req);


static uint32_t  usbd_video_AltSet = 0;//number of current interface alternative setting
static uint8_t usbd_video_CfgDesc[];
uint8_t  VideoCtl[64];
uint8_t  VideoCtlCmd = 0;
uint32_t VideoCtlLen = 0;
uint8_t  VideoCtlUnit = 0;

uint8_t play_status = 0;//0 - stream stopped, 1 - ready to stream, 2 - stream running

extern uint8_t new_frame_trigger;//timer set it to 1 every 100ms

//data array for Video Probe and Commit
VideoControl    videoCommitControl =
{
  {0x00,0x00},                      // bmHint
  {0x01},                           // bFormatIndex
  {0x01},                           // bFrameIndex
  {DBVAL(INTERVAL),},          // dwFrameInterval
  {0x00,0x00,},                     // wKeyFrameRate
  {0x00,0x00,},                     // wPFrameRate
  {0x00,0x00,},                     // wCompQuality
  {0x00,0x00,},                     // wCompWindowSize
  {0x00,0x00},                      // wDelay
  {DBVAL(MAX_FRAME_SIZE)},    // dwMaxVideoFrameSize
  {0x00, 0x00, 0x00, 0x00},         // dwMaxPayloadTransferSize
  {0x00, 0x00, 0x00, 0x00},         // dwClockFrequency
  {0x00},                           // bmFramingInfo
  {0x00},                           // bPreferedVersion
  {0x00},                           // bMinVersion
  {0x00},                           // bMaxVersion
};

VideoControl    videoProbeControl =
{
  {0x00,0x00},                      // bmHint
  {0x01},                           // bFormatIndex
  {0x01},                           // bFrameIndex
  {DBVAL(INTERVAL),},          // dwFrameInterval
  {0x00,0x00,},                     // wKeyFrameRate
  {0x00,0x00,},                     // wPFrameRate
  {0x00,0x00,},                     // wCompQuality
  {0x00,0x00,},                     // wCompWindowSize
  {0x00,0x00},                      // wDelay
  {DBVAL(MAX_FRAME_SIZE)},    // dwMaxVideoFrameSize
  {0x00, 0x00, 0x00, 0x00},         // dwMaxPayloadTransferSize
  {0x00, 0x00, 0x00, 0x00},         // dwClockFrequency
  {0x00},                           // bmFramingInfo
  {0x00},                           // bPreferedVersion
  {0x00},                           // bMinVersion
  {0x00},                           // bMaxVersion
};

/* VIDEO interface class callbacks structure */
USBD_ClassTypeDef  USBD_UVC =
{
  usbd_video_Init,
  usbd_video_DeInit,
  usbd_video_Setup,
  NULL, /* EP0_TxSent */
  usbd_video_EP0_RxReady,
  usbd_video_DataIn,
  usbd_video_DataOut,
  usbd_video_SOF,
  usbd_video_IN_Incplt,
  usbd_video_OUT_Incplt,
  USBD_video_GetCfgDesc,
#ifdef USB_OTG_HS_CORE  
  USBD_video_GetCfgDesc, /* use same config as per FS */
#endif    
};


/* USB VIDEO device Configuration Descriptor */
static uint8_t usbd_video_CfgDesc[] =
{
  /* Configuration 1 */
  USB_CONFIGUARTION_DESC_SIZE,               // bLength                  9
  USB_CONFIGURATION_DESCRIPTOR_TYPE,         // bDescriptorType          2
  WBVAL(USB_VIDEO_DESC_SIZ),
  0x02,                                      // bNumInterfaces           2
  0x01,                                      // bConfigurationValue      1 ID of this configuration
  0x00,                                      // iConfiguration           0 no description available
  USB_CONFIG_BUS_POWERED ,                   // bmAttributes          0x80 Bus Powered
  USB_CONFIG_POWER_MA(100),                  // bMaxPower              100 mA
  
  
  /* Interface Association Descriptor */
  UVC_INTERFACE_ASSOCIATION_DESC_SIZE,       // bLength                  8
  USB_INTERFACE_ASSOCIATION_DESCRIPTOR_TYPE, // bDescriptorType         11
  0x00,                                      // bFirstInterface          0
  0x02,                                      // bInterfaceCount          2
  CC_VIDEO,                                  // bFunctionClass          14 Video
  SC_VIDEO_INTERFACE_COLLECTION,             // bFunctionSubClass        3 Video Interface Collection
  PC_PROTOCOL_UNDEFINED,                     // bInterfaceProtocol       0 (protocol undefined)
  0x02,                                      // iFunction                2
  
  
  /* VideoControl Interface Descriptor */
  
  
  /* Standard VC Interface Descriptor  = interface 0 */
  USB_INTERFACE_DESC_SIZE,                   // bLength                  9
  USB_INTERFACE_DESCRIPTOR_TYPE,             // bDescriptorType          4
  USB_UVC_VCIF_NUM,                          // bInterfaceNumber         0 index of this interface (VC)
  0x00,                                      // bAlternateSetting        0 index of this setting
  0x00,                                      // bNumEndpoints            0 no endpoints
  CC_VIDEO,                                  // bInterfaceClass         14 Video
  SC_VIDEOCONTROL,                           // bInterfaceSubClass       1 Video Control
  PC_PROTOCOL_UNDEFINED,                     // bInterfaceProtocol       0 (protocol undefined)
  0x02,                                      // iFunction                2
  
  
  /* Class-specific VC Interface Descriptor */
  UVC_VC_INTERFACE_HEADER_DESC_SIZE(1),      // bLength                 13 12 + 1 (header + 1*interface
  CS_INTERFACE,                              // bDescriptorType         36 (INTERFACE)
  VC_HEADER,                                 // bDescriptorSubtype      1 (HEADER)
  WBVAL(UVC_VERSION),                        // bcdUVC                  1.10 or 1.00
  WBVAL(VC_TERMINAL_SIZ),                    // wTotalLength            header+units+terminals
  DBVAL(6000000),                       	 // dwClockFrequency		6.000000 MHz
  0x01,                                      // bInCollection            1 one streaming interface
  0x01,                                      // baInterfaceNr( 0)        1 VS interface 1 belongs to this VC interface
  
  
  /* Input Terminal Descriptor (Camera) */
  UVC_CAMERA_TERMINAL_DESC_SIZE(2),          // bLength                 17 15 + 2 controls
  CS_INTERFACE,                              // bDescriptorType         36 (INTERFACE)
  VC_INPUT_TERMINAL,                         // bDescriptorSubtype       2 (INPUT_TERMINAL)
  0x01,                                      // bTerminalID              1 ID of this Terminal
  WBVAL(ITT_CAMERA),                         // wTerminalType       0x0201 Camera Sensor
  0x00,                                      // bAssocTerminal           0 no Terminal associated
  0x00,                                      // iTerminal                0 no description available
  WBVAL(0x0000),                             // wObjectiveFocalLengthMin 0
  WBVAL(0x0000),                             // wObjectiveFocalLengthMax 0
  WBVAL(0x0000),                             // wOcularFocalLength       0
  0x02,                                      // bControlSize             2
  0x00, 0x00,                                // bmControls          0x0000 no controls supported
  
  /* Output Terminal Descriptor */
  UVC_OUTPUT_TERMINAL_DESC_SIZE(0),          // bLength                  9
  CS_INTERFACE,                              // bDescriptorType         36 (INTERFACE)
  VC_OUTPUT_TERMINAL,                        // bDescriptorSubtype       3 (OUTPUT_TERMINAL)
  0x02,                                      // bTerminalID              2 ID of this Terminal
  WBVAL(TT_STREAMING),                       // wTerminalType       0x0101 USB streaming terminal
  0x00,                                      // bAssocTerminal           0 no Terminal assiciated
  0x01,                                      // bSourceID                1 input pin connected to output pin unit 1
  0x00,                                      // iTerminal                0 no description available
  
  
  /* Video Streaming (VS) Interface Descriptor */
  
  
  /* Standard VS Interface Descriptor  = interface 1 */
  // alternate setting 0 = Zero Bandwidth
  USB_INTERFACE_DESC_SIZE,                   // bLength                  9
  USB_INTERFACE_DESCRIPTOR_TYPE,             // bDescriptorType          4
  USB_UVC_VSIF_NUM,                          // bInterfaceNumber         1 index of this interface
  0x00,                                      // bAlternateSetting        0 index of this setting
  0x00,                                      // bNumEndpoints            0 no EP used
  CC_VIDEO,                                  // bInterfaceClass         14 Video
  SC_VIDEOSTREAMING,                         // bInterfaceSubClass       2 Video Streaming
  PC_PROTOCOL_UNDEFINED,                     // bInterfaceProtocol       0 (protocol undefined)
  0x00,                                      // iInterface               0 no description available
  
  
  /* Class-specific VS Header Descriptor (Input) */
  UVC_VS_INTERFACE_INPUT_HEADER_DESC_SIZE(1,1),// bLength               14 13 + (1*1) (no specific controls used)
  CS_INTERFACE,                              // bDescriptorType         36 (INTERFACE)
  VS_INPUT_HEADER,                           // bDescriptorSubtype       1 (INPUT_HEADER)
  0x01,                                      // bNumFormats              1 one format descriptor follows
  WBVAL(VC_HEADER_SIZ),
  USB_ENDPOINT_IN(1),                        // bEndPointAddress      0x83 EP 3 IN
  0x00,                                      // bmInfo                   0 no dynamic format change supported
  0x02,                                      // bTerminalLink            2 supplies terminal ID 2 (Output terminal)
  0x00,                                      // bStillCaptureMethod      0 NO supports still image capture
  0x01,                                      // bTriggerSupport          0 HW trigger supported for still image capture
  0x00,                                      // bTriggerUsage            0 HW trigger initiate a still image capture
  0x01,                                      // bControlSize             1 one byte bmaControls field size
  0x00,                                      // bmaControls(0)           0 no VS specific controls
  
  
  /* Class-specific VS Format Descriptor  */
  VS_FORMAT_UNCOMPRESSED_DESC_SIZE,     /* bLength 27*/
  CS_INTERFACE,                         /* bDescriptorType : CS_INTERFACE */
  VS_FORMAT_UNCOMPRESSED,               /* bDescriptorSubType : VS_FORMAT_UNCOMPRESSED subtype */
  0x01,                                 /* bFormatIndex : First (and only) format descriptor */
  0x01,                                 /* bNumFrameDescriptors : One frame descriptor for this format follows. */
  //0x59,0x55,0x59,0x32,                  /* Giud Format YUY2 {32595559-0000-0010-8000-00AA00389B71} */
  0x4E,0x56,0x31,0x32,                  /* NV12:           3231564E-0000-0010-8000-00AA00389B71 */
  0x00,0x00,
  0x10,0x00,
  0x80,0x00,0x00,0xAA,0x00,0x38,0x9B,0x71,
  //16,                                 // bBitsPerPixel : Number of bits per pixel used to specify color in the decoded video frame - 16 for yuy2
  12,									//12 for nv12
  0x01,                                 /* bDefaultFrameIndex : Default frame index is 1. */
  0x00,                                 /* bAspectRatioX : Non-interlaced stream not required. */
  0x00,                                 /* bAspectRatioY : Non-interlaced stream not required. */
  0x00,                                 /* bmInterlaceFlags : Non-interlaced stream */
  0x00,                                 /* bCopyProtect : No restrictions imposed on the duplication of this video stream. */
  
  /* Class-specific VS Frame Descriptor */
  VS_FRAME_UNCOMPRESSED_DESC_SIZE,      /* bLength 30*/
  CS_INTERFACE,                         /* bDescriptorType : CS_INTERFACE */
  VS_FRAME_UNCOMPRESSED,                /* bDescriptorSubType : VS_FRAME_UNCOMPRESSED */
  0x01,                                 /* bFrameIndex : First (and only) frame descriptor */
  0x02,                                 /* bmCapabilities : Still images using capture method 0 are supported at this frame setting.D1: Fixed frame-rate. */
  WBVAL(WIDTH),                         /* wWidth (2bytes): Width of frame is 128 pixels. */
  WBVAL(HEIGHT),                        /* wHeight (2bytes): Height of frame is 64 pixels. */
  DBVAL(MIN_BIT_RATE),                  /* dwMinBitRate (4bytes): Min bit rate in bits/s  */ // 128*64*16*5 = 655360 = 0x000A0000 //5fps
  DBVAL(MAX_BIT_RATE),                  /* dwMaxBitRate (4bytes): Max bit rate in bits/s  */ // 128*64*16*5 = 655360 = 0x000A0000
  DBVAL(MAX_FRAME_SIZE),                /* dwMaxVideoFrameBufSize (4bytes): Maximum video or still frame size, in bytes. */ // 128*64*2 = 16384 = 0x00004000
  DBVAL(INTERVAL),				        /* dwDefaultFrameInterval : 1,000,000 * 100ns -> 10 FPS */ // 5 FPS -> 200ms -> 200,000 us -> 2,000,000 X 100ns = 0x001e8480
  0x01,                                 /* bFrameIntervalType : Continuous frame interval */
  DBVAL(INTERVAL),                      /* dwMinFrameInterval : 1,000,000 ns  *100ns -> 10 FPS */
  
  /* Color Matching Descriptor */
  VS_COLOR_MATCHING_DESC_SIZE,          /* bLength */
  CS_INTERFACE,                         /* bDescriptorType : CS_INTERFACE */
  0x0D,                                 /* bDescriptorSubType : VS_COLORFORMAT */
  0x01,                                 /* bColorPrimarie : 1: BT.709, sRGB (default) */
  0x01,                                 /* bTransferCharacteristics : 1: BT.709 (default) */
  0x04,                                 /* bMatrixCoefficients : 1: BT. 709. */
  
  
  /* Standard VS Interface Descriptor  = interface 1 */
  // alternate setting 1 = operational setting
  USB_INTERFACE_DESC_SIZE,                   // bLength                  9
  USB_INTERFACE_DESCRIPTOR_TYPE,             // bDescriptorType          4
  USB_UVC_VSIF_NUM,                          // bInterfaceNumber         1 index of this interface
  0x01,                                      // bAlternateSetting        1 index of this setting
  0x01,                                      // bNumEndpoints            1 one EP used
  CC_VIDEO,                                  // bInterfaceClass         14 Video
  SC_VIDEOSTREAMING,                         // bInterfaceSubClass       2 Video Streaming
  PC_PROTOCOL_UNDEFINED,                     // bInterfaceProtocol       0 (protocol undefined)
  0x00,                                      // iInterface               0 no description available
  
  
  
  /* Standard VS Isochronous Video data Endpoint Descriptor */
  USB_ENDPOINT_DESC_SIZE,                   // bLength                  7
  USB_ENDPOINT_DESCRIPTOR_TYPE,             // bDescriptorType          5 (ENDPOINT)
  USB_ENDPOINT_IN(1),                       // bEndpointAddress      0x83 EP 3 IN
  USB_ENDPOINT_TYPE_ISOCHRONOUS,            // bmAttributes             1 isochronous transfer type
  WBVAL(VIDEO_PACKET_SIZE),                 // wMaxPacketSize
  0x01                                      // bInterval                1 one frame interval
};




static uint8_t  usbd_video_Init (struct _USBD_HandleTypeDef *pdev, uint8_t cfgidx)
{  
	/* Open EP IN */
	USBD_LL_OpenEP(pdev, USB_ENDPOINT_IN(1), USBD_EP_TYPE_ISOC, VIDEO_PACKET_SIZE);
	/* Initialize the Video Hardware layer */
	return USBD_OK;
}


static uint8_t  usbd_video_DeInit (struct _USBD_HandleTypeDef *pdev,
                                   uint8_t cfgidx)
{ 
	USBD_LL_CloseEP(pdev, USB_ENDPOINT_IN(1));
	/* DeInitialize the Audio output Hardware layer */
	return USBD_OK;
}

static uint8_t  usbd_video_Setup (struct _USBD_HandleTypeDef *pdev,
                                  USBD_SetupReqTypedef *req)
{
  uint16_t len;
  uint8_t  *pbuf;
  
  switch (req->bmRequest & USB_REQ_TYPE_MASK)
  {
  case USB_REQ_TYPE_CLASS :    
    switch (req->bRequest)
    {
	case GET_RES:
		VIDEO_Req_GetRes(pdev, req);
    case GET_CUR:
    case GET_DEF:
    case GET_MIN:
    case GET_MAX:
		VIDEO_Req_GetCurrent(pdev, req);
      break;
    case SET_CUR:
		VIDEO_Req_SetCurrent(pdev, req);
      break;

    default:
      USBD_CtlError (pdev, req);
      //STM_EVAL_LEDOn(LED4);
      return USBD_FAIL;
    }
    break;

    
    /* Standard Requests -------------------------------*/
  case USB_REQ_TYPE_STANDARD:
    switch (req->bRequest)
    {
    case USB_REQ_GET_DESCRIPTOR: 
      if( (req->wValue >> 8) == CS_DEVICE)
      {
#ifdef USB_OTG_HS_INTERNAL_DMA_ENABLED
        pbuf = usbd_video_Desc;   
#else
        pbuf = usbd_video_CfgDesc + 18;
#endif 
        len = MIN(USB_VIDEO_DESC_SIZ , req->wLength);
      }
      
      USBD_CtlSendData (pdev, pbuf, len);
      break;
      
    case USB_REQ_GET_INTERFACE :
      USBD_CtlSendData (pdev, (uint8_t *)&usbd_video_AltSet, 1);
      break;
      
    case USB_REQ_SET_INTERFACE :
      if ((uint8_t)(req->wValue) < VIDEO_TOTAL_IF_NUM)
      {
        usbd_video_AltSet = (uint8_t)(req->wValue);

        if (usbd_video_AltSet == 1) {
        	//STM_EVAL_LEDOn(LED5);
        	play_status = 1;
        } else {
        	//STM_EVAL_LEDOff(LED5);
			USBD_LL_FlushEP(pdev, USB_ENDPOINT_IN(1));
        	play_status = 0;
        }
      }
      else
      {
        /* Call the error management function (command will be nacked */
        USBD_CtlError (pdev, req);
      }
      break;
    }
  }
  return USBD_OK;
}


static uint8_t  usbd_video_EP0_RxReady (struct _USBD_HandleTypeDef *pdev)
{ 
  return USBD_OK;
}


//handle request from HOST <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
static uint8_t  usbd_video_DataIn (struct _USBD_HandleTypeDef *pdev, uint8_t epnum)
{
  static uint16_t packets_cnt = 0xffff;
  static uint8_t header[2] = {2,0};//length + data
  // uint8_t* data_pointer;
  uint8_t packet[VIDEO_PACKET_SIZE];
  uint16_t i;
  static uint32_t picture_pos;
  
  USBD_LL_FlushEP(pdev, USB_ENDPOINT_IN(1));//very important
  
  packets_cnt++;
  if (packets_cnt>(PACKETS_IN_FRAME-1))
  {
    //start of new frame
    packets_cnt = 0;
    header[1]^= 1;//toggle bit0 every new frame
    picture_pos = 0;
  }
  
  packet[0] = header[0];
  packet[1] = header[1];
  
  for (i=2;i<VIDEO_PACKET_SIZE;i++)
  {
    /*if ((packets_cnt< 10) && (STM_EVAL_PBGetState(BUTTON_USER) != 0)) //react to button
    {
      packet[i] = 0xAA;
    }
    else
    {
      
    }*/
	packet[i] = nv12_picture[picture_pos];
    picture_pos++;
  }
  
  if (play_status == 2)
  {
	  USBD_LL_Transmit(pdev, USB_ENDPOINT_IN(1), (uint8_t*)&packet, (uint32_t)VIDEO_PACKET_SIZE);
  }
  else
  {
    packets_cnt = 0xffff;
  }
  
  return USBD_OK;
}

static uint8_t  usbd_video_DataOut(struct _USBD_HandleTypeDef *pdev, uint8_t epnum)
{     
  return USBD_OK;
}


static uint8_t  usbd_video_SOF(struct _USBD_HandleTypeDef *pdev)
{
  if (play_status == 1)
  {
	USBD_LL_FlushEP(pdev, USB_ENDPOINT_IN(1));
	USBD_LL_Transmit(pdev->pData, USB_ENDPOINT_IN(1), (uint8_t*)0x0002, 2);//header
	
    play_status = 2;
  }
  return USBD_OK;
}

static uint8_t  usbd_video_OUT_Incplt(struct _USBD_HandleTypeDef *pdev , uint8_t epnum)
{
  return USBD_OK;
}

static uint8_t  usbd_video_IN_Incplt(struct _USBD_HandleTypeDef *pdev , uint8_t epnum)
{
  return USBD_OK;
}

//CLASS SPECIFIC REQUEST
static void VIDEO_Req_GetCurrent(struct _USBD_HandleTypeDef *pdev, USBD_SetupReqTypedef *req)
{  
  /* Send the current mute state */
  USBD_LL_FlushEP(pdev, USB_ENDPOINT_OUT(0));
  
  if(req->wValue == 256)
  {
    //Probe Request
    USBD_CtlSendData (pdev, (uint8_t*)&videoProbeControl, req->wLength);
  }
  else if (req->wValue == 512)
  {
    //Commit Request
    USBD_CtlSendData (pdev, (uint8_t*)&videoCommitControl, req->wLength);
  }
}

//PC SEND DATA TO uC
static void VIDEO_Req_SetCurrent(struct _USBD_HandleTypeDef *pdev, USBD_SetupReqTypedef *req)
{
  if (req->wLength)
  {
    /* Prepare the reception of the buffer over EP0 */
    if(req->wValue == 256)
    {
      //Probe Request
      USBD_CtlPrepareRx (pdev, (uint8_t*)&videoProbeControl, req->wLength);
    }
    else if (req->wValue == 512)
    {
      //Commit Request
      USBD_CtlPrepareRx (pdev, (uint8_t*)&videoCommitControl, req->wLength);
    }
  }
}


static void VIDEO_Req_GetRes(struct _USBD_HandleTypeDef *pdev, USBD_SetupReqTypedef *req)
{
  USBD_CtlSendData (pdev, (uint8_t*)&videoProbeControl, 0);
}

/**
  * @brief  USBD_video_GetCfgDesc 
  *         Returns configuration descriptor.
  * @param  speed : current device speed
  * @param  length : pointer data length
  * @retval pointer to descriptor buffer
  */
static uint8_t  *USBD_video_GetCfgDesc (uint16_t *length)
{
  *length = sizeof (usbd_video_CfgDesc);
  return usbd_video_CfgDesc;
}
