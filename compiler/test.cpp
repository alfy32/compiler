#include <iostream>
#include <memory>

using namespace std;

int main() {

	shared_ptr<int> num = make_shared<int>(10);

	pair<string, shared_ptr<int> > p = make_pair("num", num);

	cout << *(p.second) << endl;

	return 0; 
}