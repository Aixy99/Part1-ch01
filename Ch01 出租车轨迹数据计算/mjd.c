#include "ch01.h"

Time beijing_time_to_utc(Time beijingTime) {
	// ÿ���µ������������꣩
	const int daysInMonth[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
	
	Time utcTime = beijingTime;  // ����ʱ�䣬���޸�ԭʼ����
	
	// ��ȥ8Сʱ�õ�UTCʱ��
	utcTime.hour -= 8;
	
	// ����Сʱ��λ�����������
	while (utcTime.hour < 0) {
		utcTime.hour += 24;
		utcTime.day--;
	}
	
	// ����������λ
	while (1) {
		// �������С��1����Ҫ��λ���ϸ���
		if (utcTime.day < 1) {
			utcTime.month--;
			
			// �����·ݽ�λ
			if (utcTime.month < 1) {
				utcTime.month = 12;
				utcTime.year--;
			}
			
			// ��ȡ�ϸ��µ�����
			int prevMonthDays = daysInMonth[utcTime.month - 1];
			if (utcTime.month == 2 && 
				(utcTime.year % 4 == 0 && (utcTime.year % 100 != 0 || utcTime.year % 400 == 0))) {
				prevMonthDays = 29;  // �������
			}
			
			utcTime.day += prevMonthDays;
			continue;
		}
		break;  // ����Ҫ��������
	}
	
	return utcTime;
}

double utc_to_mjd(Time utc) {
	double mjd = 0;
	mjd = -678987 + 367 * utc.year
		- (int)((7.0 / 4.0) * (utc.year + (int)((utc.month + 9) / 12.0)))
		+ (int)((275 * utc.month) / 9.0) + utc.day + utc.hour / 24.0
		+ utc.minute / 1440.0 + utc.second / 86400.0;
	return mjd;
}