#include "ch01.h"

double calculate_distance(double x1, double y1, double x2, double y2) {
	double distance = sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2)) / 1000; // km
	return distance;
}

double calculate_speed(double x1, double y1, Time t1, double x2, double y2, Time t2) {
	double distance = calculate_distance(x1, y1, x2, y2);

	double time = fabs(utc_to_mjd(beijing_time_to_utc(t1)) - utc_to_mjd(beijing_time_to_utc(t2))) * 24; // hour

	return distance / time;
}