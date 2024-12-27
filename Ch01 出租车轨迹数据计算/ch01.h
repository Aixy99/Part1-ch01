#ifndef CH01_H
#define CH01_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define PI 3.1415926535

/* 结构体声明 */
typedef struct {
	int year;
	int month;
	int day;
	int hour;
	int minute;
	int second;
} Time;
typedef struct {
	char vehicleId[10];	// 车辆标识
	int state;			// 运营状态
	Time beijingTime;	// 时间戳
	double x;			// x分量
	double y;			// y分量
} TaxiData;

typedef struct {
	TaxiData* data;		// 出租车数据数组
	int size;			// 数组数据量
	int capacity;		// 容量
} TaxiDataArray;	

/* 函数声明 */
TaxiDataArray* create_taxi_data_array(int initialCapacity);

void free_taxi_data_array(TaxiDataArray* taxiData);

int read_taxi_data(const char* fileName, TaxiDataArray* taxiData);

void print_taxi_data(TaxiDataArray* taxiData);

double utc_to_mjd(Time utc);

Time beijing_time_to_utc(Time beijingTime);

double calculate_azimuth(double x1, double y1, double x2, double y2);

double calculate_distance(double x1, double y1, double x2, double y2);

double calculate_speed(double x1, double y1, Time t1, double x2, double y2, Time t2);
#endif // !CH01_H

