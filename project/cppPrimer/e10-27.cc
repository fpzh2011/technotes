#include <vector>
#include <list>
#include <algorithm>
#include <iostream>
#include <iterator>

using namespace std;

int
main () {
	cout << " Please input nums." << endl;
	istream_iterator<int> it(cin), eof;
	vector<int> v(it, eof);
	sort(v.begin(), v.end());
	list<int> l(v.size());
	auto uit = unique_copy(v.begin(), v.end(), l.begin());
	cout << "list.size() is : " << l.size() << endl;
	for(auto i : l) {
		cout << i << ' ';
	}
	cout << endl;
	for(auto p = l.begin(); p != uit; ++p) {
		cout << *p << ' ';
	}
	cout << endl;
	return 0;
}
