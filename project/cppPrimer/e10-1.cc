#include <vector>
#include <algorithm>
#include <iostream> 
#include <string>

using namespace std;

int 
main (int argc, char *argv[]) {
	if(argc < 2) {
		cout << "Usage: " << argv[0] << " searchnum" << endl;
		return 0;
	}
	int search = stoi(string(argv[1]));
	cout << "please input ints:\n";
	//可以用流迭代器改进下面的代码
	int i;
	vector<int> v;
	while(cin >> i) {
		v.push_back(i);
	}
	cout << "count is " << count(v.cbegin(), v.cend(), search) << endl;
	return 0;
}
