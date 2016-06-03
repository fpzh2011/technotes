#include <iostream>
#include <string>

using namespace std;

class Person {
public:
	Person(const string n, const string a);
	const string& getName() const;
	const string& getAddr() const;
private :
	string name, addr;
};

Person::Person(const string n, const string a) : name(n), addr(a){ }

const string& Person::getName() const {
	return this->name;
}

const string& Person::getAddr() const {
	return this->addr;
}

int 
main () {
	Person p = Person("abc", "defghi");
	cout << p.getName() << ' ' << p.getAddr() << endl;
	return 0;
}
