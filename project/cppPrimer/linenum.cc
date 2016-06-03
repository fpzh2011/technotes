//读入一个文件，添加行号后输出

#include <iostream>
#include <fstream>

using namespace std;

int 
main (int argc, char *argv[]) {
	if(argc < 3) {
		cerr << "Usage: " << argv[0] << " inputfile outputfile" << endl;
		return 0;
	}
	ifstream ifs(argv[1]);
	if(!ifs.is_open()) {
		cerr << argv[1] << " opened failed." << endl;
	}
	ofstream ofs(argv[2]);
	if(!ofs.is_open()) {
		cerr << argv[2] << " opened failed." << endl;
	}
	string s;
	int count = 0;
	while(getline(ifs,s)) {
		ofs << s << endl;
	}
	return 0;
}
