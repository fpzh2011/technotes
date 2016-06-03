#include <forward_list>
#include <iostream>

using namespace std;

void 
print(forward_list<string> fl) {
	for(auto s : fl) {
		cout << s << ' ';
	}
	cout << endl;
}

int 
main (int argc, char *argv[]) {
	if(argc < 3) {
		cout << "Usage: " << argv[0] << " pattern insert" << endl;
		return 0;
	}
	//print original list
	forward_list<string> fl = {"a", "b", "c", "d", "e", "f"};
	print(fl);
	//find & insert
	auto p1 = fl.before_begin(), p2 = fl.begin();
	while(p2 != fl.end()) {
		if(*p2 == argv[1]) {
			fl.insert_after(p2, argv[2]);
			print(fl);
			return 0;
		}
		++p2;
		++p1;
	}
	if(p2 == fl.end()) {
		fl.insert_after(p1, argv[1]);
		print(fl);
	}
	return 0;
}
