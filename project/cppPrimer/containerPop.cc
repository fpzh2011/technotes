#include <iostream>
#include <vector>
#include <deque>
#include <list>

using namespace std;

class Test {
public:
	~Test() { cerr << "destory Test." << endl; }
};

int 
main () {
	list<Test> v(10);
	v.pop_back();
	cout << "Press Enter to exit." << endl;
	cin.get();
	return 0;
}
