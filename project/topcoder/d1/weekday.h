#ifndef WEEKDAY
#define WEEKDAY

typedef struct { 
	int year;
	int month;
	int day;
} date;

int validate_date (const date *pd);

const char* weekday(const date *pd);

//打印日期
# define printdate(pd) { \
	printf("%04d-%02d-%02d", (pd)->year, (pd)->month, (pd)->day); \
}

#endif

