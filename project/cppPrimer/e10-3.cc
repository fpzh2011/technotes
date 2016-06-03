#include <vector>
#include <iterator>
#include <iostream>
#include <algorithm>

using namespace std;

int 
main () {
	cout << "Please input nums:\n";
	istream_iterator<int> is(cin), eof;
	vector<int> v(is, eof);
	cout << "Sum: " << accumulate(v.cbegin(), v.cend(), 0) << endl;
}
