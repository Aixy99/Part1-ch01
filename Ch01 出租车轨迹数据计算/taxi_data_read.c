#include "ch01.h"

Time set_time(char timeStr[20]) {
	Time time = { 0 };
	
	char cleanStr[20] = { 0 };
	int j = 0;
	for (int i = 0; timeStr[i] != '\0'; i++) {
		if (timeStr[i] != ' ') {
			cleanStr[j++] = timeStr[i];
		}
	}
	
	if (sscanf(cleanStr, "%4d%2d%2d%2d%2d%2d",
		&time.year,
		&time.month,
		&time.day,
		&time.hour,
		&time.minute,
		&time.second) != 6) {
		printf("时间格式错误: %s\n", timeStr);
		time.year = 0;
		time.month = 0;
		time.day = 0;	
		time.hour = 0;
		time.minute = 0;
		time.second = 0;
	}
	return time;
}

TaxiDataArray* create_taxi_data_array(int initialCapacity) {
	TaxiDataArray* taxiData = (TaxiDataArray*)malloc(sizeof(TaxiDataArray));
	if (NULL == taxiData) {
		return NULL;
	}

	taxiData->data = (TaxiData*)malloc(sizeof(TaxiData) * initialCapacity);
	if (NULL == taxiData->data) {
		
		free(taxiData);
		return NULL;
	}

	taxiData->size = 0;
	taxiData->capacity = initialCapacity;
	return taxiData;
}

void free_taxi_data_array(TaxiDataArray* taxiData) {
	if (NULL != taxiData) {
		if (NULL != taxiData->data) {
			free(taxiData->data);
		}
		free(taxiData);
	}
}

int read_taxi_data(const char* fileName, TaxiDataArray* taxiData) {
	FILE* fp = fopen(fileName, "r");
	if (fp == NULL) {
		printf("无法打开文件: %s\n", fileName);
		return 1;
	}

	char buffer[1024];
	// 读取并验证文件头
	if (!fgets(buffer, sizeof(buffer), fp)) {
		printf("文件为空\n");
		fclose(fp);
		return 2;
	}

	while (fgets(buffer, sizeof(buffer), fp)) {
		// 移除行尾的换行符或回车符
		buffer[strcspn(buffer, "\r\n")] = 0;
		
		// 跳过空行
		if (strlen(buffer) == 0) {
			continue;
		}

		// 容量检查和扩容...
		if (taxiData->size >= taxiData->capacity) {
			int newCapacity = taxiData->capacity * 2;
			TaxiData* newData = (TaxiData*)realloc(taxiData->data, 
							   sizeof(TaxiData) * newCapacity);
			if (newData == NULL) {
				printf("内存分配失败\n");
				return 2;
			}
			taxiData->data = newData;
			taxiData->capacity = newCapacity;
		}

		TaxiData* currentLineData = &(taxiData->data[taxiData->size]);
		char beijingTimeStr[20] = { 0 };

		// 使用更严格的格式检查
		if (sscanf(buffer, "%[^,],%d,%[^,],%lf ,%lf",
			currentLineData->vehicleId,
			&currentLineData->state,
			beijingTimeStr,
			&currentLineData->x,
			&currentLineData->y) != 5) {
			
			printf("数据格式错误: %s\n", buffer);
			continue;  // 跳过错误的行
		}

		// 验证数据合理性
		if (strlen(currentLineData->vehicleId) == 0 ||
			currentLineData->state < 0 ||
			currentLineData->x == 0 ||
			currentLineData->y == 0) {
			printf("数据值异常: %s\n", buffer);
			continue;
		}

		currentLineData->beijingTime = set_time(beijingTimeStr);
		if (currentLineData->beijingTime.year == 0) {
			printf("时间格式错误: %s\n", buffer);
			continue;
		}

		taxiData->size++;
	}

	fclose(fp);
	return 0;
}

void print_taxi_data(TaxiDataArray* taxiData) {
	for (int i = 0; i < taxiData->size; i++) {
		printf("%-3s %d %.8lf %lf %lf\n",
			taxiData->data[i].vehicleId,
			taxiData->data[i].state,
			utc_to_mjd(beijing_time_to_utc(taxiData->data[i].beijingTime)),
			taxiData->data[i].x,
			taxiData->data[i].y);
	}
}