# string字符串到算数类型转换

	#include 
	字符串转换为算数类型
	float              stof(const string& str, size_t *idx = 0);
	double             stod(const string& str, size_t *idx = 0);
	long double        stold(const string& str, size_t *idx = 0);
	int                stoi(const string& str, size_t *idx = 0, int base = 10);
	long               stol(const string& str, size_t *idx = 0, int base = 10);
	unsigned long      stoul(const string& str, size_t *idx = 0, int base = 10);
	long long          stoll(const string& str, size_t *idx = 0, int base = 10);
	unsigned long long stoull(const string& str, size_t *idx = 0, int base = 10);
	算数类型转换为字符串
	string to_string(int val);
	string to_string(unsigned val);
	string to_string(long val);
	string to_string(unsigned long val);
	string to_string(long long val);
	string to_string(unsigned long long val);
	string to_string(float val);
	string to_string(double val);
	string to_string(long double val);


http://stackoverflow.com/questions/5290089/how-to-convert-a-number-to-string-and-vice-versa-in-c
