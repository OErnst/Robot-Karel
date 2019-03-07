#ifndef KAREL_PROGRAM_HPP
#define KAREL_PROGRAM_HPP

#include "StaticTools.hpp"
#include "errors.hpp"
#include <map>
#include <stack>
#include <string>
#include <vector>

struct StackPart {
	std::string function;
	unsigned int instruction;
	bool lastIf;
};

struct NonExistingFunction {
	std::string name;
	std::string calledFrom;
};

class Program {
public:
	void prepareProgram(std::istream& instructions);
	std::map<std::string, int>& getFunctions() {
		return functions;
	}
	std::map<std::string, std::vector<std::string>>& getProcedures() {
		return procedures;
	}
	const std::map<std::string, std::vector<std::string>>& getCProcedures() const {
		return procedures;
	}
	bool isRunning() const {
		return running;
	}
	void setRunning(bool value) {
		running = value;
	}

	std::stack<StackPart>& getStack() {
		return stack;
	}
	void prepareMain();
	void robotFails();

private:
	std::array<std::string, 13> standardFunctions{ { "SKIP", "BREAK", "HALT", "MOVE", "LEFT", "RIGHT", "PICKUP", "PUTDOWN", "IFWALL", "IFMARK", "ELSE", "DEFINE", "END" } };
	std::map<std::string, int> functions = { { "SKIP", 0 }, { "BREAK", 1 }, { "HALT", 2 }, { "MOVE", 3 }, { "LEFT", 4 }, { "RIGHT", 5 }, { "PICKUP", 6 }, { "PUTDOWN", 7 }, { "IFWALL", 8 }, { "IFMARK", 9 }, { "ELSE", 10 } };
	std::map<std::string, std::vector<std::string>> procedures;
	bool running = true;
	std::stack<StackPart> stack;
	bool insideFunction = false;
	u_int actualLine = 0;
	std::vector<std::string> splittedLine;
	std::string actualFunction;
	std::vector<std::string> functionInstructions;
	std::vector<NonExistingFunction> notExistingFunctions;

	void classicInstruction();
	void defineFunction(std::string& name);
	bool isDefine();
	bool isIfOrElse();
	bool isEnd();

	void addNotExistingFunction(std::string& name);
	bool checkAllNotExForExistence();

	bool isNotIfInstruction();
	bool checkCorrectName(std::string& name);
	bool standardInstruction(std::string& ins);
	bool isOneOfConditions(const std::string& name);
};

#endif //KAREL_PROGRAM_HPP
