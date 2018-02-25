/***********************************************************************

							qwertyScreen : 	
		encapsulation of screen physical description/mesh descriptor

*************************************************************************/
#pragma once
#include <opencv2\opencv.hpp>
#include <Windows.h>

namespace qwerty
{
	//encapsulation of screen physical description/mesh descriptor
	class Screen
	{
	public:

		Screen();

		//physical size of display in milli-meter
		int GetPhysicalWidth();

		//physical size of display in milli-meter
		int GetPhysicalHeight();

	private:

		int mPhysicalWidth;
		int mPhysicalHeight;

	};
}