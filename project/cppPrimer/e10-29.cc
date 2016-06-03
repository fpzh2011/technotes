/* 读入一个文件，逆序输出各单词 */

#include <fstream>
#include <vector>
#include <iterator>
#include <iostream>

using namespace std;

int 
main (int argc, char *argv[]) {
	if(argc < 2) {
		cout << "Usage: " << argv[0] << " filename." << endl;
		return 0;
	}
	ifstream ifs(argv[1]);
	if(!ifs) {
		cerr << argv[1] << " open failed.\n";
		return 0;
	}
	istream_iterator<string> it(ifs), eof;
	vector<string> v(it, eof);
	for(auto p = v.crbegin(); p != v.crend(); ++p) {
		cout << *p << endl;
	}
	cout << endl;
	ifs.close();
	return 0;
}
