#include "ch01.h"

int main() {
	const char* fileName = "../data/出租车数据2.txt";
	const char* outputFileName = "../data/结果2.txt";

	TaxiDataArray* taxiData = create_taxi_data_array(1);

	if (read_taxi_data(fileName, taxiData) || taxiData == NULL) {
		if (taxiData != NULL) {
			free_taxi_data_array(taxiData);
		}
		return 1;
	}

	double startMJD = 0.0;
	double endMJD = 0.0;
	double speed = 0.0;
	double azimuth = 0.0;
	double nearDistance = 0.0;
	double totalDistance = 0.0;
	int indexBefore = -1;
	int indexAfter = -1;
	int indexFirst = -1;
	int indexLast = -1;

	FILE* fp = fopen(outputFileName, "w");
	fprintf(fp, "---------------速度和方位角计算结果-------------\n");

	// 先找到第一个和最后一个T2记录
	for (int i = 0; i < taxiData->size; i++) {
		if (strstr(taxiData->data[i].vehicleId, "T2")) {
			if (indexFirst == -1) {
				indexFirst = i;
			}
			indexLast = i;  // 更新最后一个T2记录的位置
		}
	}

	int dataCount = 0;

	// 处理相邻记录计算速度和方位角
	for (int i = indexFirst; i < indexLast; i++) {
		if (!strstr(taxiData->data[i].vehicleId, "T2")) {
			continue;
		}
		
		indexBefore = i;
		// 找下一个T2记录
		for (int j = i + 1; j < taxiData->size; j++) {
			if (strstr(taxiData->data[j].vehicleId, "T2")) {
				indexAfter = j;
				break;
			}
		}
		
		// 计算相关数据
		startMJD = utc_to_mjd(beijing_time_to_utc(taxiData->data[indexBefore].beijingTime));
		endMJD = utc_to_mjd(beijing_time_to_utc(taxiData->data[indexAfter].beijingTime));
		speed = calculate_speed(taxiData->data[indexBefore].x, taxiData->data[indexBefore].y, taxiData->data[indexBefore].beijingTime,
			taxiData->data[indexAfter].x, taxiData->data[indexAfter].y, taxiData->data[indexAfter].beijingTime);
		azimuth = calculate_azimuth(taxiData->data[indexBefore].x, taxiData->data[indexBefore].y,
			taxiData->data[indexAfter].x, taxiData->data[indexAfter].y);
		nearDistance = calculate_distance(taxiData->data[indexBefore].x, taxiData->data[indexBefore].y,
			taxiData->data[indexAfter].x, taxiData->data[indexAfter].y);
		totalDistance += nearDistance;

		fprintf(fp, "%02d; %.5lf-%.5lf; %6.3lf; %7.3lf\n", dataCount++, startMJD, endMJD, speed, azimuth);
	}
	fprintf(fp, "---------------距离计算结果-------------\n");
	fprintf(fp, "累积距离：%.3lf (km)\n", totalDistance);
	fprintf(fp, "首尾直线距离：%.3lf (km)\n", calculate_distance(taxiData->data[indexFirst].x, taxiData->data[indexFirst].y,
		taxiData->data[indexLast].x, taxiData->data[indexLast].y));
	free_taxi_data_array(taxiData);
	return 0;
}