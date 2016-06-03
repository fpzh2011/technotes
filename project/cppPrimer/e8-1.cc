#include <iostream>

using namespace std;

int 
main () {
	string s;
	auto old_state = cin.rdstate();
	while(cin >> s) {
		cout << s << endl;
	}
	cin.setstate(old_state);
	return 0;
}
