#include "ch01.h"

double calculate_azimuth(double x1, double y1, double x2, double y2) {
	double changeX = x2 - x1;
	double changeY = y2 - y1;
	double radian = atan(changeY / changeX);
	double angle = radian * 180 / PI;

	//if (changeX < 0.0) {
	//	angle += 180;
	//} else if (fabs(changeX < 1e-7)) { // == 0
	//	if (changeY > 0) {
	//		angle = 90;
	//	} else if (changeY < 0) {
	//		angle = 270;
	//	}
	//}

	if (angle < 0) {
		angle += 360;
	} else if (angle > 360) {
		angle -= 360;
	}
	return angle;
}