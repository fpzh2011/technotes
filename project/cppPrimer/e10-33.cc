#include <vector>
using std::vector;

#include <iterator>
using std::istream_iterator; using std::ostream_iterator;

#include <fstream>
using std::ofstream;

#include <iostream>
using std::cin; using std::cout; using std::endl; using std::cerr;

int 
main (int argc, char *argv[]) {
	if(argc < 3) {
		cout << "Usage: " << argv[0] << " oddfile evenfile." << endl;
		return 0;
	}
	istream_iterator<int> it(cin), eof;
	vector<int> v(it, eof);
	ofstream odd(argv[1]);
	if(!odd) {
		cerr << argv[1] << " file open failed.\n";
		return 0;
	}
	ofstream even(argv[2]);
	if(!even) {
		cerr << argv[2] << " file open failed.\n";
		return 0;
	}
	ostream_iterator<int> oit(odd, " "), eit(even, "\n");
	for(auto i : v) {
		if(i % 2) {
			*oit = i;
		}
		else {
			*eit = i;
		}
	}
	odd.close();
	even.close();
	return 0;
}
