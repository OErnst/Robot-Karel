#include "StaticTools.hpp"

using namespace std;
vector<string> StaticTools::splitter(const string &str) {
	bool taking = false;
	vector<string> values;
	string temporaryString;
	for (char i : str) {
		if (taking && (i == ' ' || i == '\t')) {
			values.push_back(temporaryString);
			temporaryString = "";
			taking = false;
			continue;
		}
		if (i > 32) {
			temporaryString += i;
			taking = true;
		}
	}
	if (!temporaryString.empty()) {
		values.push_back(temporaryString);
	}
	return values;
}

bool StaticTools::isNumber(const std::string &str) {
	for (char x : str) {
		if (!isdigit(x)) {
			return false;
		}
	}
	return true;
}
