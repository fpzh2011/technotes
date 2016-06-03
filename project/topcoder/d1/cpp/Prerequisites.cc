#include "Prerequisites.h"
#include <string>
#include <vector>
#include <set>
#include <map>
#include <list>
#include <cctype>
#include <regex>

bool cmpclass (std::string class1, std::string class2);

std::string *orderClasses (std::string *a) {
	std::string *p = new std::string;
	*p = std::to_string(cmpclass(*a, *a));
	return p;
}


//namespace {
static const std::string pattern = "([A-Z]{3})([1-9][0-9]{2})";
static const std::regex r(pattern);
// 比较两个class。dept是3-4个大写字母，num是100-999的数字
bool cmpclass (std::string class1, std::string class2) {
	//std::smatch results;
	//return std::regex_search(class1, results, r);
	return true;
}

// 将输入string数组解析为map
bool parseInput (std::string[], std::map<std::string, std::set<std::string, decltype(cmpclass)*>> &input, std::list<std::string> &result);

// 将class插入到结果集
bool insert (std::list<std::string> &result, std::string classid);
//}
