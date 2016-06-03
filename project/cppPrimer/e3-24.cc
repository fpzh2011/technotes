#include <iostream>
#include <vector>

using namespace std;

int 
main () {
	int i;
	vector<int> v;
	while(cin >> i) {
		v.push_back(i);
	}
	if(!v.empty()) {
		for(auto ip = v.cbegin(); ip != v.cend()-1; ++ip) {
			cout << *ip + *(ip+1) << ' ';
		}
		cout << endl;
		for(auto head = v.cbegin(), tail = v.cend()-1; head <= tail; ++head, --tail) {
			cout << *head + *tail << ' ';
		}
		cout << endl;
	}
}
