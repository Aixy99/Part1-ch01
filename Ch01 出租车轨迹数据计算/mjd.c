#include "ch01.h"

Time beijing_time_to_utc(Time beijingTime) {
	// 每个月的天数（非闰年）
	const int daysInMonth[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
	
	Time utcTime = beijingTime;  // 复制时间，不修改原始数据
	
	// 减去8小时得到UTC时间
	utcTime.hour -= 8;
	
	// 处理小时借位（负数情况）
	while (utcTime.hour < 0) {
		utcTime.hour += 24;
		utcTime.day--;
	}
	
	// 处理天数借位
	while (1) {
		// 如果天数小于1，需要借位到上个月
		if (utcTime.day < 1) {
			utcTime.month--;
			
			// 处理月份借位
			if (utcTime.month < 1) {
				utcTime.month = 12;
				utcTime.year--;
			}
			
			// 获取上个月的天数
			int prevMonthDays = daysInMonth[utcTime.month - 1];
			if (utcTime.month == 2 && 
				(utcTime.year % 4 == 0 && (utcTime.year % 100 != 0 || utcTime.year % 400 == 0))) {
				prevMonthDays = 29;  // 闰年二月
			}
			
			utcTime.day += prevMonthDays;
			continue;
		}
		break;  // 不需要继续处理
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