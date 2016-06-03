#include <iostream>

using namespace std;

const static string u = "bdfhijklpt";
const static string l = "fgjpqy";
const static string e = u + l;

int 
main (int argc, char *argv[]) {
	if(argc < 2) {
		cout << "Usage: " << argv[0] << " inputstring" << endl;
		return 0;
	}
	string s(argv[1]);
	decltype(s.size()) maxbegin, begin, maxend, end;
	if((maxbegin = begin = s.find_first_not_of(e, 0)) == string::npos) { 
		return 0;
	}
	maxend = end = s.find_first_of(e, begin);
	while(end != string::npos) {
		if((begin = s.find_first_not_of(e, end)) == string::npos) {
			break;
		}
		if((end = s.find_first_of(e, begin)) == string::npos) {
			end = s.size() + 1;
		}
		if((end - begin) > (maxend - maxbegin)) {
			maxend = end;
			maxbegin = begin;
		}
	}
	cout << s.substr(maxbegin, maxend-maxbegin) << endl;
	return 0;
}
