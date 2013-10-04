#include <iostream>
#include <fstream>
#include <vector>
#include <cstdio>
#include <ctype.h>
#include <tuple>

using namespace std;

string toUpper(string str) {
	for(int i = 0; i<str.length(); i++) {
		if(i > 0 && isupper(str[i]))
			str.insert(i++,"_");

		str[i] = toupper(str[i]);
	}

	return str;
}

int main() {
	vector<tuple<string,bool,string>> names;

	names.push_back(tuple<string,bool,string>("SymbolTable",false,""));
	names.push_back(tuple<string,bool,string>("Symbol", false, ""));
	names.push_back(tuple<string,bool,string>("Const", true, get<0>(names[1])));
	names.push_back(tuple<string,bool,string>("Var", true, get<0>(names[1])));
	names.push_back(tuple<string,bool,string>("Func", true, get<0>(names[1])));
	names.push_back(tuple<string,bool,string>("Type", true, get<0>(names[1])));
	names.push_back(tuple<string,bool,string>("String", true, get<0>(names[2])));
	names.push_back(tuple<string,bool,string>("Char", true, get<0>(names[2])));
	names.push_back(tuple<string,bool,string>("Int", true, get<0>(names[2])));
	names.push_back(tuple<string,bool,string>("Simple", true, get<0>(names[5])));
	names.push_back(tuple<string,bool,string>("Record", true, get<0>(names[5])));
	names.push_back(tuple<string,bool,string>("Array", true, get<0>(names[5])));
	
	cout << names.size() << endl;

	for(int i = 0; i < names.size(); i++){
		string name = get<0>(names[i]);

	 	cout << i << " " << name << endl;

		string hname = name + ".hpp";
		string cname = name + ".cpp";

		ofstream hfile(hname.c_str());

		hfile << "#ifndef " << toUpper(name) << "_HPP" << endl
			  << "#define " << toUpper(name) << "_HPP" << endl
			  << endl;

		if(!get<1>(names[i])) {

			hfile << "class " << name << " {" << endl
				  << endl
				  << "};" << endl;
		} else {
			hfile << "#include \"" << get<2>(names[i]) << ".hpp\"" << endl
				  << endl
				  << "class " << name << " : public " << get<2>(names[i]) << " {" << endl
				  << endl
				  << "};" << endl;
		}

		hfile << endl
			  << "#endif" << endl;

		ofstream cfile(cname.c_str());

		cfile << "#include \"" << name << ".hpp\"" << endl;
	}

	return 0;
}