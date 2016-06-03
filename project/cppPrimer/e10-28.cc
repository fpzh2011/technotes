#include <vector>
#include <iterator>
#include <iostream>
#include <list>

using namespace std;

int
main () {
	vector<int> v = {1,2,3,4,5,6,7,8,9,0};
	list<int> v1, v2, v3;
	copy(v.cbegin(), v.cend(), inserter(v1,v1.begin()));
	for(auto i : v1) {
		cout << i << ' ';
	}
	cout << endl;
	copy(v.cbegin(), v.cend(), back_inserter(v2));
	for(auto i : v2) {
		cout << i << ' ';
	}
	cout << endl;
	copy(v.cbegin(), v.cend(), front_inserter(v3));
	for(auto i : v3) {
		cout << i << ' ';
	}
	cout << endl;
	return 0;
}
