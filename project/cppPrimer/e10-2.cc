#include <list>
#include <algorithm>
#include <iostream> 
#include <string>
#include <iterator>

using namespace std;

int 
main (int argc, char *argv[]) {
	if(argc < 2) {
		cout << "Usage: " << argv[0] << " searchstring" << endl;
		return 0;
	}
	string search = string(argv[1]);
	cout << "please input strings:\n";
	istream_iterator<string> si(cin), eof;
	list<string> v(si, eof);
	cout << "count is " << count(v.cbegin(), v.cend(), search) << endl;
	return 0;
}
