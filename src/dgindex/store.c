/* 
 *  Mutated into DGIndex. Modifications Copyright (C) 2004, Donald Graft
 * 
 *	Copyright (C) Chia-chen Kuo - April 2001
 *
 *  This file is part of DVD2AVI, a free MPEG-2 decoder
 *	
 *  DVD2AVI is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2, or (at your option)
 *  any later version.
 *   
 *  DVD2AVI is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU General Public License for more details.
 *   
 *  You should have received a copy of the GNU General Public License
 *  along with GNU Make; see the file COPYING.  If not, write to
 *  the Free Software Foundation, 675 Mass Ave, Cambridge, MA 02139, USA. 
 *
 */

#include "global.h"
#include "filter.h"

#define MAX_WINDOW_WIDTH 1024
#define MAX_WINDOW_HEIGHT 768

__forceinline static void Store_RGB24(unsigned char *src[]);

static void FlushRGB24(void);

static BITMAPINFOHEADER birgb, birgbsmall;

static char VideoOut[_MAX_PATH];
static unsigned char *y444;
static int playback, Old_Playback;
static long frame_size;
static bool TFF, RFF, TFB, BFB, frame_type;

static char *FrameType[] = {
	"Interlaced", "Progressive"
};

void Write_Frame(unsigned char *src[], D2VData d2v, DWORD frame)
{
	int repeat;
	char *ext, szTemp[_MAX_PATH];

	frame_type = d2v.pf;
	TFF = d2v.trf>>1;
	if (FO_Flag == FO_RAW)
		RFF = 0;
	else
		RFF = d2v.trf & 0x01;

	// Update the main window title bar.
	sprintf(szBuffer, "DGIndex - ");
	ext = strrchr(Infilename[CurrentFile], '\\');
	strncat(szBuffer, ext+1, strlen(Infilename[0])-(int)(ext-Infilename[0]));
	sprintf(szTemp, " [%dx%d] [File %d/%d]", Clip_Width, Clip_Height, CurrentFile+1, NumLoadedFiles);
	strcat(szBuffer, szTemp);
	if (VOB_ID && CELL_ID)
	{
		sprintf(szTemp, " [Vob %d] [Cell %d]", VOB_ID, CELL_ID);
		strcat(szBuffer, szTemp);
	}
	SetWindowText(hWnd, szBuffer);

	if (!frame)
	{
		TFB = BFB = false;
		playback = Old_Playback = 0;
		frame_size = 0;

		Clip_Width = Coded_Picture_Width;
		Clip_Height = Coded_Picture_Height;
		CLIP_AREA = HALF_CLIP_AREA = CLIP_STEP = CLIP_HALF_STEP = 0;

		if (Luminance_Flag)
			InitializeFilter();

		if (ClipResize_Flag)
		{
			if (Clip_Top || Clip_Bottom || Clip_Left || Clip_Right)
			{
				Clip_Width -= Clip_Left+Clip_Right;
				Clip_Height -= Clip_Top+Clip_Bottom;

				CLIP_AREA = Coded_Picture_Width * Clip_Top;
				HALF_CLIP_AREA = (Coded_Picture_Width>>1) * Clip_Top;

				CLIP_STEP = Coded_Picture_Width * Clip_Top + Clip_Left;
				CLIP_HALF_STEP = (Coded_Picture_Width>>1) * Clip_Top + (Clip_Left>>1);
			}
		}

		NINE_CLIP_WIDTH = Clip_Width * 9;
		QUAD_CLIP_WIDTH = Clip_Width<<2;
		DOUBLE_CLIP_WIDTH = Clip_Width<<1;
		HALF_CLIP_WIDTH = Clip_Width>>1;

		LUM_AREA = Coded_Picture_Width * Clip_Height;
		DOUBLE_WIDTH = Coded_Picture_Width<<1;
		HALF_WIDTH = Coded_Picture_Width>>1;
		HALF_WIDTH_D8 = (Coded_Picture_Width>>1) - 8;
		PROGRESSIVE_HEIGHT = (Coded_Picture_Height>>1) - 2;
		INTERLACED_HEIGHT = (Coded_Picture_Height>>2) - 2;
		RGB_DOWN1 = Clip_Width * (Clip_Height - 1) * 3;
		RGB_DOWN2 = Clip_Width * (Clip_Height - 2) * 3;

		if (Clip_Width > MAX_WINDOW_WIDTH || Clip_Height > MAX_WINDOW_HEIGHT)
			ResizeWindow(Clip_Width/2, Clip_Height/2);
		else
		{
			ResizeWindow(Clip_Width, Clip_Height);
			ResizeWindow(Clip_Width, Clip_Height);
		}

		ZeroMemory(&birgb, sizeof(BITMAPINFOHEADER));
		birgb.biSize = sizeof(BITMAPINFOHEADER);
		birgb.biWidth = Clip_Width;
		birgb.biHeight = Clip_Height;
		birgb.biPlanes = 1;
		birgb.biBitCount = 24;
		birgb.biCompression = BI_RGB;
		birgb.biSizeImage = Clip_Width * Clip_Height * 3;

		ZeroMemory(&birgbsmall, sizeof(BITMAPINFOHEADER));
		birgbsmall.biSize = sizeof(BITMAPINFOHEADER);
		birgbsmall.biWidth = Clip_Width;
		birgbsmall.biHeight = Clip_Height;
		birgbsmall.biPlanes = 1;
		birgbsmall.biBitCount = 24;
		birgbsmall.biCompression = BI_RGB;
		birgbsmall.biSizeImage = Clip_Width * Clip_Height * 3;

		if (FO_Flag!=FO_FILM)
		{
			if (TFF)
				SetDlgItemText(hDlg, IDC_FIELD_ORDER, "Top");
			else
				SetDlgItemText(hDlg, IDC_FIELD_ORDER, "Bottom");
		}
	}

	repeat = DetectVideoType(frame, d2v.trf);

	if (FO_Flag != FO_FILM || repeat)
	{
		Store_RGB24(src);
	}

	if (FO_Flag != FO_FILM && repeat==2)
	{
		Store_RGB24(src);
	}

	if (Info_Flag && process.locate==LOCATE_RIP)
	{
		sprintf(szBuffer, "%s", FrameType[frame_type]);
		SetDlgItemText(hDlg, IDC_FRAME_TYPE, szBuffer);

		sprintf(szBuffer, "%s", matrix_coefficients == 7 ? "SMPTE 240M" :
								matrix_coefficients == 6 ? "SMPTE 170M" :
								matrix_coefficients == 5 ? "ITU-R BT.470-2" :
								matrix_coefficients == 4 ? "FCC" : "ITU-R BT.709");
		SetDlgItemText(hDlg, IDC_COLORIMETRY, szBuffer);

		sprintf(szBuffer, "%s", picture_structure == 3 ? "Frame" : "Field");
		SetDlgItemText(hDlg, IDC_FRAME_STRUCTURE, szBuffer);

		sprintf(szBuffer, "%d", frame+1);
		SetDlgItemText(hDlg, IDC_CODED_NUMBER, szBuffer);

		sprintf(szBuffer, "%d", playback);
		SetDlgItemText(hDlg, IDC_PLAYBACK_NUMBER, szBuffer);

		if ((frame & 31) == 31)
		{
			double rate, rate_avg;
			timing.ed = timeGetTime();

			sprintf(szBuffer, "%.2f", 1000.0*(playback-Old_Playback)/(timing.ed-timing.mi+1));
			SetDlgItemText(hDlg, IDC_FPS, szBuffer);
			rate = ((double) (Bitrate_Monitor * 8 * Frame_Rate) / (playback-Old_Playback)) / 1000000.0;
			Bitrate_Average += Bitrate_Monitor;
			rate_avg = (Bitrate_Average * 8 * Frame_Rate) / playback / 1000000.0;
			sprintf(szBuffer, "%.3f Mbps", rate);
			SetDlgItemText(hDlg, IDC_BITRATE, szBuffer);
			sprintf(szBuffer, "%.3f Mbps", rate_avg);
			SetDlgItemText(hDlg, IDC_BITRATE_AVG, szBuffer);
			Bitrate_Monitor = 0;
			timing.mi = timing.ed;
			Old_Playback = playback;
		}
	}
}

static void Store_RGB24(unsigned char *src[])
{
	if (chroma_format==CHROMA420)
	{
		conv420to422(src[1], u422, frame_type);
		conv420to422(src[2], v422, frame_type);

		conv422to444(u422, u444);
		conv422to444(v422, v444);
	}
	else
	{
		conv422to444(src[1], u444);
		conv422to444(src[2], v444);
	}

	if (Luminance_Flag)
	{
		LuminanceFilter(src[0], lum);
		y444 = lum;
	}
	else
		y444 = src[0];

	if (BFB)
	{
		conv444toRGB24odd(y444, u444, v444, rgb24);
		TFB = true;
		FlushRGB24();

		conv444toRGB24even(y444, u444, v444, rgb24);
		BFB = true;
		FlushRGB24();
	}
	else
	{
		conv444toRGB24even(y444, u444, v444, rgb24);
		BFB = true;
		FlushRGB24();

		conv444toRGB24odd(y444, u444, v444, rgb24);
		TFB = true;
		FlushRGB24();
	}

	if (FO_Flag!=FO_FILM && FO_Flag!=FO_RAW && RFF)
		if (TFF)
		{
			TFB = true;
			FlushRGB24();
		}
		else
		{
			BFB = true;
			FlushRGB24();
		}
}

static void FlushRGB24()
{
	static double DisplayTime = 0;
	static unsigned int timeOffset = 0;
	static old_speed;
	double rate;
	int sleep_time;
	int elapsed;
	
	if (TFB & BFB)
	{
		if (PlaybackSpeed == SPEED_MAXIMUM)
		{
			// maximum
			ShowFrame(false);
			old_speed = PlaybackSpeed;
		}
		else if (PlaybackSpeed == SPEED_SINGLE_STEP)
		{
			ShowFrame(false);
			old_speed = PlaybackSpeed;
			if (process.locate == LOCATE_RIP)
			{
				for (;;)
				{
					// Wait for right arrow to be hit before displaying
					// the next frame.
					Sleep(100);
					if (Stop_Flag || RightArrowHit == true)
						break;
				}
				RightArrowHit = false;
			}
		}
		else
		{
			if (playback == 0 || PlaybackSpeed != old_speed)
			{
				DisplayTime = 0;
				timeOffset = timeGetTime();
			}
			old_speed = PlaybackSpeed;
			elapsed = timeGetTime() - timeOffset;
			sleep_time = (int) (DisplayTime - elapsed);
			if (sleep_time >= 0)
				Sleep(sleep_time);
			ShowFrame(false);
			switch (PlaybackSpeed)
			{
				case SPEED_SUPER_SLOW: // super slow
					rate = 5.0;
					break;
				case SPEED_SLOW: // slow
					rate = 10.0;
					break;
				default:
				case SPEED_NORMAL: // normal
					rate = Frame_Rate;
					break;
				case SPEED_FAST: // fast
					rate = 2 * Frame_Rate;
					break;
			}
			DisplayTime += 1000.0 / rate;
		}
		playback++;
		TFB = BFB = false;
	}
}

int DetectVideoType(int frame, int trf)
{
	static int Old_TRF, Repeat_On, Repeat_Off;
	static bool Repeat_Init;

	if (frame)
	{
		if ((trf & 3) == ((Old_TRF+1) & 3))
			FILM_Purity++;
		else
			VIDEO_Purity++;
	}
	else
	{
		FILM_Purity = VIDEO_Purity = Repeat_On = Repeat_Off = 0;
		Repeat_Init = false;
	}

	Old_TRF = trf;

	if (trf & 1)
		Repeat_On ++;
	else
		Repeat_Off ++;

	if (Repeat_Init)
	{
		if (Repeat_Off-Repeat_On == 5)
		{
			Repeat_Off = Repeat_On = 0;
			return 0;
		}
		else if (Repeat_On-Repeat_Off == 5)
		{
			Repeat_Off = Repeat_On = 0;
			return 2;
		}
	}
	else
	{
		if (Repeat_Off-Repeat_On == 3)
		{
			Repeat_Off = Repeat_On = 0;
			Repeat_Init = true;
			return 0;
		}
		else if (Repeat_On-Repeat_Off == 3)
		{
			Repeat_Off = Repeat_On = 0;
			Repeat_Init = true;
			return 2;
		}
	}

	return 1;
}

void ShowFrame(bool move)
{
	unsigned int x, xx, y, yy, incy, incyy;
	unsigned char *yp, *yyp;
	unsigned int Clip_HeightOver2 = Clip_Height / 2;
	unsigned int Clip_WidthTimes3Over2 = Clip_Width * 3 / 2;
	

	if (!Check_Flag)
		return;

	if (rgb24 && rgb24small && (move || Display_Flag))
	{
		if (Clip_Width > MAX_WINDOW_WIDTH || Clip_Height > MAX_WINDOW_HEIGHT)
		{
			// Zoom out by two in both directions. Quick and dirty.
			yp = rgb24small + (Clip_Height - 1) * Clip_Width * 3;
			yyp = rgb24 + (Clip_Height - 1) * Clip_Width * 3;
			incy = Clip_Width * 3;
			incyy = 2 * Clip_Width * 3;
			for (y = yy = Clip_Height - 1; y >= Clip_HeightOver2; y--, yy-=2)
			{
				for (x = xx = 0; x < Clip_WidthTimes3Over2; x+=3, xx+=6)
				{
					yp[x] = yyp[xx];
					yp[x+1] = yyp[xx+1];
					yp[x+2] = yyp[xx+2];
				}
				yp -= incy;
				yyp -= incyy;
			}
			SetDIBitsToDevice(hDC, 0, 0, Clip_Width / 2, Clip_Height, 0, 0, Clip_Height/2, Clip_Height/2,
							  rgb24small + Clip_Height/2 * Clip_Width * 3, (LPBITMAPINFO) &birgbsmall, DIB_RGB_COLORS);
		}
		else
		{
			SetDIBitsToDevice(hDC, 0, 0, Clip_Width, Clip_Height, 0, 0, 0, Clip_Height,
							  rgb24, (LPBITMAPINFO)&birgb, DIB_RGB_COLORS);
		}
	}
}