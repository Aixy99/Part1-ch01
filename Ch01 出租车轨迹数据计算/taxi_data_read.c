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
		printf("ʱ���ʽ����: %s\n", timeStr);
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
		printf("�޷����ļ�: %s\n", fileName);
		return 1;
	}

	char buffer[1024];
	// ��ȡ����֤�ļ�ͷ
	if (!fgets(buffer, sizeof(buffer), fp)) {
		printf("�ļ�Ϊ��\n");
		fclose(fp);
		return 2;
	}

	while (fgets(buffer, sizeof(buffer), fp)) {
		// �Ƴ���β�Ļ��з���س���
		buffer[strcspn(buffer, "\r\n")] = 0;
		
		// ��������
		if (strlen(buffer) == 0) {
			continue;
		}

		// ������������...
		if (taxiData->size >= taxiData->capacity) {
			int newCapacity = taxiData->capacity * 2;
			TaxiData* newData = (TaxiData*)realloc(taxiData->data, 
							   sizeof(TaxiData) * newCapacity);
			if (newData == NULL) {
				printf("�ڴ����ʧ��\n");
				return 2;
			}
			taxiData->data = newData;
			taxiData->capacity = newCapacity;
		}

		TaxiData* currentLineData = &(taxiData->data[taxiData->size]);
		char beijingTimeStr[20] = { 0 };

		// ʹ�ø��ϸ�ĸ�ʽ���
		if (sscanf(buffer, "%[^,],%d,%[^,],%lf ,%lf",
			currentLineData->vehicleId,
			&currentLineData->state,
			beijingTimeStr,
			&currentLineData->x,
			&currentLineData->y) != 5) {
			
			printf("���ݸ�ʽ����: %s\n", buffer);
			continue;  // �����������
		}

		// ��֤���ݺ�����
		if (strlen(currentLineData->vehicleId) == 0 ||
			currentLineData->state < 0 ||
			currentLineData->x == 0 ||
			currentLineData->y == 0) {
			printf("����ֵ�쳣: %s\n", buffer);
			continue;
		}

		currentLineData->beijingTime = set_time(beijingTimeStr);
		if (currentLineData->beijingTime.year == 0) {
			printf("ʱ���ʽ����: %s\n", buffer);
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