#include <opencv2\opencv.hpp>
#include "Screen.h"
#include "Camera.h"

qwerty::Screen scr;
qwerty::Camera cam;

int main()
{
	cam.RecognizeChessboard("..\\media\\group2\\1.jpg", false);
	cam.RecognizeChessboard("..\\media\\group2\\2.jpg", false);
	cam.RecognizeChessboard("..\\media\\group2\\3.jpg", false);
	cam.RecognizeChessboard("..\\media\\group2\\4.jpg", false);
	cam.RecognizeChessboard("..\\media\\group2\\5.jpg", false);
	cam.RecognizeChessboard("..\\media\\group2\\6.jpg", false);
	cam.RecognizeChessboard("..\\media\\group2\\7.jpg", false);
	cam.RecognizeChessboard("..\\media\\group2\\8.jpg", false);
	cam.RecognizeChessboard("..\\media\\group2\\9.jpg", false);
	cam.Calibrate();
	return 0;
}