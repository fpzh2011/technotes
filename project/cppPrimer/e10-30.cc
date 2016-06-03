#include <vector>
#include <algorithm>
#include <iterator>
#include <iostream>

using namespace std;

int
main () {
	cout << "Please input nums:" << endl;
	istream_iterator<int> it(cin), eof;
	vector<int> v(it, eof);
	sort(v.begin(), v.end());
	for(auto i : v) {
		cout << i << ' ';
	}
	cout << endl;
	sort(v.rbegin(), v.rend());
	for(auto i : v) {
		cout << i << ' ';
	}
	cout << endl;
	return 0;
}
