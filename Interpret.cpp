#include "Interpret.hpp"

using namespace std;

Interpret::Interpret(std::istream &world, std::istream &program) {
	doWorldPart(world);
	doProgramPart(program);
}

const Robot &Interpret::robot() const {
	return droid;
}

const World &Interpret::world() const {
	return droid.getConstMap();
}

void Interpret::doWorldPart(std::istream &world) {
	SecondLineParam secondLineParam = droid.getMap().doFirstTwoLines(world);
	droid.setDirection(secondLineParam.d);
	droid.setX(secondLineParam.x);
	droid.setY(secondLineParam.y);
	droid.prepareWorld(world);
}

void Interpret::doProgramPart(std::istream &program) {

	this->program.prepareProgram(program);
	this->program.prepareMain();
}

int Interpret::doProcedure() {
	string function = program.getStack().top().function;
	u_int index = program.getStack().top().instruction;
	string instruction;

	if (index >= program.getProcedures()[function].size()) {
		instruction = "BREAK";
	} else {
		instruction = program.getProcedures()[function][index];
	}

	int switchValue;
	if (program.getFunctions().find(instruction) != program.getFunctions().end()) {
		switchValue = program.getFunctions()[instruction];
	} else {
		switchValue = 11;
	}
	program.getStack().top().instruction++;
	return identifyInstruction(switchValue, instruction);
}

int Interpret::identifyInstruction(int switchValue, const string &instruction) {
	switch (switchValue) {
	case 0:
		return 0;
	case 1:
		return breakX();
	case 2:
		program.setRunning(false);
		return 2;
	case 3:
		if (!droid.moveX())
			robotFails();
		return 3;
	case 4:
		droid.leftX();
		return 4;
	case 5:
		droid.rightX();
		return 5;
	case 6:
		if (!droid.pickupX())
			robotFails();
		return 6;
	case 7:
		droid.putdownX();
		return 7;
	case 8:
		return ifFunction(droid.ifwallX(), 8);
	case 9:
		return ifFunction(droid.ifmarksX(), 8);
	case 10:
		return elseFunction();
	default:
		program.getStack().push({ instruction, 0, false });
		return -1;
	}
}

unsigned Interpret::run() {
	unsigned num = 0;
	while (program.isRunning()) {
		doProcedure();
		num++;
	}
	return num;
}
/*
void Interpret::testing() {
	cout << "Steps: " << run() << "\n\n";
	droid.getMap().printWorld();
	cout << droid.getX() << ", " << droid.getY() << "\n";
}
*/
bool Interpret::hasProcedure(const std::string &name) const {
	return program.getCProcedures().find(name) != program.getCProcedures().end();
}

bool Interpret::running() const {
	return program.isRunning();
}

bool Interpret::step() {
	if (running()) {
		doProcedure();
	}
	return running();
}

int Interpret::breakX() {
	program.getStack().pop();
	if (program.getStack().empty()) {
		program.setRunning(false);
		return 2;
	}
	return 1;
}

void Interpret::robotFails() {
	program.robotFails();
}

int Interpret::ifFunction(bool ifResult, int whichIf) {
	program.getStack().top().lastIf = ifResult;
	if (program.getStack().top().lastIf) {
		return doProcedure();
	}
	program.getStack().top().instruction++;

	return whichIf;
}

int Interpret::elseFunction() {
	if (!program.getStack().top().lastIf) {
		return doProcedure();
	}
	program.getStack().top().lastIf = false;
	program.getStack().top().instruction++;

	return 10;
}
