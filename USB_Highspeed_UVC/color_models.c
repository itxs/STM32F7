#include "color_models.h"

RGB_Pix YUV_to_RGB(YUV_Pix* pix)
{
	RGB_Pix out;
	out.R = pix->Y + (int8_t)(1.13983*(pix->V - 128));
	out.G = pix->Y - (int8_t)(0.39465*(pix->U - 128)) - (int8_t)(0.58060 * (pix->V - 128));
	out.B = pix->Y + (int8_t)(2.03211*(pix->U - 128));
	return out;
}

YUV_Pix RGB_to_YUV(RGB_Pix* pix)
{
	YUV_Pix out;
	out.Y = (int8_t)(0.299 * pix->R) + (int8_t)(0.587 * pix->G) + (int8_t)(0.114 * pix->B);
	out.U = (int8_t)(-0.14713 * pix->R) - (int8_t)(0.28886 * pix->G) + (int8_t)(0.436 * (pix->B + 128));
	out.V = (int8_t)(0.615 * pix->R) - (int8_t)(0.51499 * pix->G) - (int8_t)(0.10001 * (pix->B + 128));
	return out;
}
