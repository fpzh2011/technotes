/* 162.html */

#include <string>

class HowEasy {
public:
	std::string::size_type pointVal (const std::string &s);
private:
	auto wordlen (const std::string &s) -> decltype(s.size());
	std::string::size_type getPointByLen (std::string::size_type len);
};
