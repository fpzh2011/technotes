#include <algorithm>
#include <iostream>
#include <vector>
#include <iterator>

using namespace std;

int 
main (int argc, char *argv[]) {
	if(argc < 2) {
		cout << "Usage: " << argv[0] << " len" << endl;
		return 0;
	}
	int len = stoi(string(argv[1]));
	cout << "Please input strings:\n";
	istream_iterator<string> it(cin), eof;
	vector<string> v(it, eof);
	auto pit = partition(v.begin(), v.end(),  
			[len] (const string &a) { return a.size() < len; }
		);
	for(auto i = v.cbegin(); i != pit; ++i) {
		cout << *i << ' ';
	}
	cout << endl;
	for(auto i = pit; i != v.cend(); ++i) {
		cout << *i << ' ';
	}
	cout << endl;
	return 0;
}
