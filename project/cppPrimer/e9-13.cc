#include <vector>
#include <list>
#include <deque>
#include <iostream>

using namespace std;

int 
main () {
	list<int> iv = {1,2,3,4,5};
	deque<int> od, ed;
	for(auto i : iv) {
		i%2 == 0 ? ed.push_back(i) : od.push_back(i);
	}
	cout << "odd deque: ";
	for(auto o : od) {
		cout << o << ' ';
	}
	cout << endl;
	cout << "even deque: ";
	for(auto e : ed) {
		cout << e << ' ';
	}
	cout << endl;
	return 0;
}
