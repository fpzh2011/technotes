/* 给定一个日期（非公元前），输出星期 */

#include <stdio.h>

#include "weekday.h"

/* 日期比较。date1 > date2，返回正值；date1 == date2，返回0；date1 < date2，返回负值.*/
//16位及以下机器算法无效
static int 
datecmp (const date *pdate1, const date *pdate2) {
	long ld1, ld2;
	ld1 = pdate1->year * 10000L + pdate1->month * 100L + (long)pdate1->day;
	ld2 = pdate2->year * 10000L + pdate2->month * 100L + (long)pdate2->day;
	return (int)(ld1 - ld2);
}

//是否闰年
#define leapyear(year) ((year % 4) == 0 && year % 100 != 0 \
		|| (year % 400) == 0)

static const int dayofmonth[2][13] = {	//每月月份。第二行是闰年。
	{0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},
	{0, 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}
};

/*校验日期值是否合法。只支持公元后的年份*/
int 
validate_date (const date *pd) {
	int year, month, day;
	year = pd->year;
	month = pd->month;
	day = pd->day;

	if(year < 0 || month <= 0 || month > 12 || day < 0) {
		return 0;
	}

	return dayofmonth[leapyear(pd->year)][month] >= day;
}

/* 返回pd在当年的第几天 */
static int
dayofyear (const date *pd) {
	int i, sum, leap = leapyear(pd->year);
	for(i = 1, sum = 0; i < pd->month; i++) {
		sum += dayofmonth[leap][i];
	}
	return sum + pd->day;
}

/*返回两个日期相差天数. *pd1 - *pd2 */ 
static int 
diff_date(const date *pd1, const date *pd2) {
	int diff, s = datecmp(pd1, pd2);
	if(s < 0) {	//让pd1始终指向更大的date
		const date *temp = pd1;
		pd1 = pd2;
		pd2 = temp;
	}
	if(pd1->year == pd2->year) {
		return dayofyear(pd1) - dayofyear(pd2);
	}
	diff = 365 + leapyear(pd2->year) -dayofyear(pd2) +	//pd2剩余天数
		dayofyear(pd1);
	//计算pd2与pd1之间的年份天数
	for(int y = pd2->year + 1; y < pd1->year; y++) {
		diff += (365 + leapyear(y));
	}
	return diff * (s < 0 ? -1 : 1);
}

/* 根据日期，获取星期 */
const char* 
weekday (const date *pd) {
	static const char* weekdays[] = {
		"Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday", "Sunday"
	};
	static const date date900101 = {1990, 1, 1};
	static const int index900101 = 1;
	int diff = diff_date(pd, &date900101);
	if(diff >= 0) {
		return weekdays[diff % 7];
	}
	else {
		return weekdays[(7 - (-diff) % 7) % 7];
	}
}

