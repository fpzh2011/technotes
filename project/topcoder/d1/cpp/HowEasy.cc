#include "HowEasy.h"
#include <sstream>
#include <cctype>
#include <limits>

using size_type = std::string::size_type;

size_type HowEasy::pointVal (const std::string &problem) {
	std::istringstream is(problem);
	std::string s;
	int avglen, sum, count;
	avglen = sum = count = 0;
	while(is >> s) {
		if(auto len = wordlen(s)) {
			++count;
			sum += len;
		}
	}
	if(count > 0) {
		avglen = sum / count;
	}
	return getPointByLen(avglen);
}

//如果s是一个word，则返回剔除末尾句点的长度；否则，返回0。
auto HowEasy::wordlen (const std::string &s) -> decltype(s.size()) {
	decltype(s.size()) i = 0;
	for(auto c : s) {
		if(!(islower(c) || isupper(c))) {
			break;
		}
		++i;
	}
	if(!s.empty() && (i == s.size() || i == s.size()-1 && s.back() == '.' && s != ".")) {
		return i;
	}
	return 0;
}

size_type HowEasy::getPointByLen (size_type len) {
	std::string::size_type a[][3] = 
		{
		0, 3, 250, 
		4, 5, 500, 
		6, std::numeric_limits<std::string::size_type>::max(), 1000,
		};
	size_type r = 0;
	for(auto itv : a) {
		if(len >= itv[0] && len <= itv[1]) {
			r = itv[2];
			break;
		}
	}
	return r;
}
