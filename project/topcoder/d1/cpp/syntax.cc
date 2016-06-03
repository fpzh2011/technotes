#include <stack>
#include <iostream>

using namespace std;

inline bool 
isOpenChar (char c) {
	return c == '(' || c == '[' || c == '{';
}

inline bool 
isCloseChar (char c) {
	return c == ')' || c == ']' || c == '}';
}

inline bool 
mymatch (char o, char c) {
	return
		o == '(' && c == ')' ||
		o == '[' && c == ']' || 
		o == '{' && c == '}';

}

int 
main (int argc, char *argv[]) {
	if(argc < 2) {
		cout << "Usage: " << argv[0] << " inputstring" << endl;
		return 0;
	}
	stack<char> s;
	string str(argv[1]);
	for(char c : str) {
		if(isOpenChar(c)) {
			s.push(c);
		}
		else if(isCloseChar(c)) {
			if(s.empty()) {
				cout << false << endl;
				return 0;
			}
			char open = s.top();
			s.pop();
			if(!mymatch(open, c)) {
				cout << false << endl;
				return 0;
			}
		}
	}
	if(s.empty()) {
		cout << true << endl;
	}
	else {
		cout << false << endl;
	}
	return 0;
}
