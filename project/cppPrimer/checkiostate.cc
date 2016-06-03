#include <iostream>

using namespace std;

int 
main () {
	int i = 9999, j = 8888, count = 0;
	cout << "input 2 int. i=" << i << " j=" << j << endl << endl;
	while(!cin.eof()) {
		if(cin >> i) {
			cout
				<< "i rdstate: " << cin.rdstate() << '\t'
				<< "fail: " << cin.fail() << '\t'
				<< "good: " << cin.good() << '\t'
				<< "i=" << i << endl;
		}
		cin.clear(cin.rdstate() & ~cin.failbit);
		if(cin >> j) {
			cout
				<< "j rdstate: " << cin.rdstate() << '\t'
				<< "fail: " << cin.fail() << '\t'
				<< "good: " << cin.good() << '\t'
				<< "j=" << j << endl << endl;
		}
		if(count++ > 3) {
				break;
			}
	}
	cout
		<< "i=" << i << " j=" << j << " count=" << count << '\n'
		<< "rdstate: " << cin.rdstate() << '\t'
		<< "fail: " << cin.fail() << '\t'
		<< "eof: " << cin.eof() << '\t'
		<< "good: " << cin.good() << '\t'
		<< endl;
	return 0;
}
