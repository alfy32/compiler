#include <iostream>
#include <memory>
#include <map>
#include <string>

using namespace std;

class Name {

};

class MyName: public Name {

};

int main() {

	shared_ptr<int> num = make_shared<int>(10);

	pair<string, shared_ptr<int> > p = make_pair("num", num);

	cout << *(p.second) << endl;

	std::map<std::string, std::string> maps;

	std::string me = "alan";

	maps[me] = "sam";

	if(maps.find(me) != maps.end()) {
		cout << "Found it.";
	}

	cout << "The map: " << maps["alan"] << endl;

	return 0; 
}