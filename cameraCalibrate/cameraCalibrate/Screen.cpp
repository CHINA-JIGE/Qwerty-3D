/***********************************************************************

										qwertyScreen : 	
		encapsulation of screen physical description/mesh descriptor

*************************************************************************/

#include "Screen.h"

qwerty::Screen::Screen()
{
	//device related param must be queried via operating system
	HDC hdc = ::GetDC(0);
	//The width/height of the screen of the primary display monitor, in pixels
	mPhysicalWidth= ::GetDeviceCaps(hdc, HORZSIZE);
	mPhysicalHeight  = ::GetDeviceCaps(hdc, VERTSIZE);
}

int qwerty::Screen::GetPhysicalWidth()
{
	return mPhysicalWidth;
}

int qwerty::Screen::GetPhysicalHeight()
{
	return mPhysicalHeight;
}
