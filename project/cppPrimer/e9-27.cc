#include <forward_list>
#include <iostream>

using namespace std;

int 
main () {
	forward_list<int> il = {1,2,3,4,5,6,7,8,9};
	for(auto i : il) {
		cout << i << ' ';
	}
	cout << endl;
	
	return 0;
}
