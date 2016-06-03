#include <string>
#include <iostream>

using namespace std;

int 
main () {
	string str, s1, s2;
	int maxcount = 0, count = 0;
	if(cin >> s1) {
		++count;
		while(cin >> s2) {
			if(s1 == s2) {
				++count;
			}
			else {
				if(count > 1 && maxcount < count) {
					maxcount = count;
					str = s1;
				}
				s1 = s2;
			}
		}
	}
	if(maxcount > 1 && !str.empty()) {
		cout << "max str is: " << str << "\ncount is: " << maxcount << endl;
	}
	else {
		cout << "no dup str" << endl;
	}
}
