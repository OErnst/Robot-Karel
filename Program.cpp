#include "Program.hpp"

using namespace std;

void Program::prepareProgram(std::istream &instructions) {
	string line;

	while (getline(instructions, line)) {
		actualLine++;
		splittedLine = StaticTools::splitter(line);
		if (splittedLine.empty() || splittedLine[0][0] == '#') {
			continue;
		}
		if (splittedLine.size() > 2) {
			Complain::invalidSource(actualLine, "Too many words");
		}
		if (isDefine()) {
			continue;
		}
		if (!insideFunction) {
			Complain::invalidSource(actualLine, "Instruction outside function.");
		}

		if (isIfOrElse()) {
			continue;
		}
		if (splittedLine.size() != 1) {
			Complain::invalidSource(actualLine, "Too many words");
		}
		if (!insideFunction) {
			Complain::invalidSource(actualLine, "Instruction outside function.");
		}
		if (isEnd()) {
			continue;
		}
		classicInstruction();
	}

	if (insideFunction) {
		Complain::invalidSource(actualLine + 1, "No end found."); // end o jedna níž z nějakého důvodu bacha
	}
	if (procedures.find("MAIN") == procedures.end()) {
		Complain::undefinedReference("@ENTRY", "MAIN", "no main found");
	}
	checkAllNotExForExistence();
}

bool Program::standardInstruction(std::string &ins) {
	for (const string &s : standardFunctions) {
		if (s == ins) {
			return true;
		}
	}
	return false;
}

bool Program::checkCorrectName(std::string &name) {
	for (char x : name) {
		if (!(x >= 65 && x <= 90) && !(x >= 97 && x <= 122) && !(x >= 48 && x <= 57)) {
			return false;
		}
	}
	return true;
}

void Program::classicInstruction() {
	if (functions.find(splittedLine[0]) != functions.end()) {
		functionInstructions.push_back(splittedLine[0]);
	} else {
		addNotExistingFunction(splittedLine[0]);
	}
}

bool Program::isEnd() {
	if (splittedLine[0] == "END") {
		insideFunction = false;
		procedures[actualFunction] = functionInstructions;
		functionInstructions.clear();
		return true;
	}
	return false;
}

bool Program::isOneOfConditions(const string &name) {
	return name == "IFWALL" || name == "IFMARK" || name == "ELSE";
}

bool Program::isIfOrElse() {
	if (isOneOfConditions(splittedLine[0])) {
		if (splittedLine.size() != 2) {
			Complain::invalidSource(actualLine, "There should be only two words but totally different amount found");
		}
		//check for if before else
		if (splittedLine[0] == "ELSE" && (functionInstructions.size() < 2 || isNotIfInstruction())) {
			Complain::invalidSource(actualLine, "ELSE is not correctly used");
		}

		if (isOneOfConditions(splittedLine[1])) {
			Complain::invalidSource(actualLine, "Invalid instruction after if");
		}

		functionInstructions.push_back(splittedLine[0]);
		if (functions.find(splittedLine[1]) != functions.end()) {
			functionInstructions.push_back(splittedLine[1]);
		} else {
			addNotExistingFunction(splittedLine[1]);
		}
		return true;
	}
	return false;
}
void Program::defineFunction(std::string &name) {
	if (!checkCorrectName(name) || standardInstruction(name)) {
		Complain::invalidSource(actualLine, "Wrong name");
	}
	if (procedures.find(name) == procedures.end()) {
		procedures.insert(pair<string, vector<string>>(name, {}));
	} else {
		Complain::invalidSource(actualLine, "Function defined twice.");
	}
}

bool Program::isDefine() {
	if (splittedLine[0] == "DEFINE") {
		if (insideFunction) {
			Complain::invalidSource(actualLine, "Define inside function.");
		}
		if (splittedLine.size() == 2) {
			defineFunction(splittedLine[1]);
			insideFunction = true;
			actualFunction = splittedLine[1];
			return true;
		}
		Complain::invalidSource(actualLine, "Define what? or defining too many things.");
	}
	return false;
}

bool Program::checkAllNotExForExistence() {
	for (NonExistingFunction &nef : notExistingFunctions) {
		if (procedures.find(nef.name) == procedures.end()) {
			Complain::undefinedReference(nef.calledFrom, nef.name, "from " + nef.calledFrom + " to " + nef.name);
		}
	}
	return true;
}

void Program::addNotExistingFunction(std::string &name) {
	if (!checkCorrectName(name) || standardInstruction(name)) {
		Complain::invalidSource(actualLine, "Illegal characters...");
	}
	notExistingFunctions.push_back({ name, actualFunction });
	functionInstructions.push_back(name);
}

void Program::prepareMain() {
	stack.push({ "MAIN", 0, false });
}

void Program::robotFails() {
	u_int line = 0;
	for (u_int i = 0; i < stack.top().instruction; i++) {
		string instruction = procedures[stack.top().function][i];
		if (instruction != "IFWALL" && instruction != "IFMARK" && instruction != "ELSE") {
			line++;
		}
	}
	Complain::runtimeError(stack.top().function, line - 1, "Robot is doing illigal business");
}

bool Program::isNotIfInstruction() {
	return functionInstructions[functionInstructions.size() - 2] != "IFWALL" && functionInstructions[functionInstructions.size() - 2] != "IFMARK";
}
