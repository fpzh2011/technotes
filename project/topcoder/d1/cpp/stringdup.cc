/*
144333322221
44333322221
443332221
122
 */

#include <vector>
#include <iostream>
#include <map>
#include <limits>

using namespace std;

class StringDup {
public:
	char getMax(const string &s) {
		map<char,unsigned int[2]> m;	//int数组第一个元素表示字符第一次出现的下标，从1开始；第二个元素表示字符出现次数
		int index = 0;
		for(auto c : s) {
			++m[c][1];
			++index;
			if(m[c][0] == 0) {
				m[c][0] = index;
			}
		}
		//寻找最大
		unsigned int maxcount = 0, minindex = std::numeric_limits<unsigned int>::max();
		char maxchar;
		for(const auto &p : m) {
			//cout << p.first << ' ' << p.second[0] << ' ' << p.second[1] << endl;
			//be carefule, in this case, map key is sorted by char, not the input sequence
			if(p.second[1] > maxcount || p.second[1] == maxcount && p.second[0] < minindex) {
				maxchar = p.first;
				maxcount = p.second[1];
				minindex = p.second[0];
			}
		}
		cout << "Result: " << maxchar << ' ' <<  maxcount << ' ' << minindex << endl;
		return maxchar;
	}
};

int 
main (int argc, char *argv[]) {
	if(argc < 2) {
		cout << "Usage: " << argv[0] << " string" << endl;
		return 0;
	}
	StringDup sdup;
	cout << sdup.getMax(argv[1]) << endl;
	return 0;
}
