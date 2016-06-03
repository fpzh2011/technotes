#include <vector>
#include <iostream>
#include <string>

using namespace std;

void 
print (const vector<int> &v) {
	for(auto i : v) {
		cout << i << ' ';
	}
	cout << endl;
}

//sum=accumulate(v,0)
vector<vector<int>> & 
football (int sum, int target, size_t index) {
	static const vector<int> score = {2, 3, 7};
	static vector<vector<int>> vv;
	static vector<int> v;
	if(sum >= target || index >= score.size()) {
		return vv;
	}
	int s = 0;
	for(size_t i = index; i < score.size(); ++i) {
		s = score[i];
		v.push_back(s);
		if((sum + s) == target) {
			vv.push_back(v);
			v.pop_back();
			return vv;
		}
		else if((sum+s) < target) {
			football(sum+s, target, i);
		}
		v.pop_back();
	}
	return vv;
}

int 
main (int argc, char *argv[]) {
	if(argc < 2) {
		cout << "Usage: " << argv[0] << " targetNum" << endl;
		return 0;
	}
	int target = stoi(argv[1]);
	auto vv = football(0, target, 0);
	cout << "The count of results is : " << vv.size() << endl;
	for(auto const &r : vv) {
		print(r);
	}
	return 0;
}
