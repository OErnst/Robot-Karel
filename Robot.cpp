#include "Robot.hpp"

using namespace std;

Position Robot::position() const {
	return x_pos;
}

Direction Robot::direction() const {
	return x_direction;
}

void Robot::setX(int x) {
	x_pos.x = x;
}
void Robot::setY(int y) {
	x_pos.y = y;
}

int Robot::getX() {
	return x_pos.x;
}
int Robot::getY() {
	return x_pos.y;
}

void Robot::setDirection(Direction d) {
	x_direction = d;
}

World &Robot::getMap() {
	return map;
}

const World &Robot::getConstMap() const {
	return map;
}

bool Robot::checkPosition() {
	return !map.isWallOrEnd(x_pos.x, x_pos.y);
}

bool Robot::prepareWorld(std::istream &world) {
	map.createWorld(world);
	if (!checkPosition()) {
		Complain::invalidWorld(2, "robot has a problem");
	}
	return true;
}

Position Robot::getPositionInFrontOfRobot() {
	Position p;
	if (x_direction == Direction::North) {
		p.y = getY() - 1;
		p.x = getX();
	} else if (x_direction == Direction::West) {
		p.y = getY();
		p.x = getX() - 1;
	} else if (x_direction == Direction::South) {
		p.y = getY() + 1;
		p.x = getX();
	} else {
		p.y = getY();
		p.x = getX() + 1;
	}
	return p;
}

bool Robot::moveX() {
	x_pos = getPositionInFrontOfRobot();
	return !map.isWallOrEnd(getX(), getY());
}

void Robot::leftX() {
	if (x_direction == Direction::North) {
		x_direction = Direction::West;
	} else if (x_direction == Direction::West) {
		x_direction = Direction::South;
	} else if (x_direction == Direction::South) {
		x_direction = Direction::East;
	} else {
		x_direction = Direction::North;
	}
}

void Robot::rightX() {
	if (x_direction == Direction::North) {
		x_direction = Direction::East;
	} else if (x_direction == Direction::West) {
		x_direction = Direction::North;
	} else if (x_direction == Direction::South) {
		x_direction = Direction::West;
	} else {
		x_direction = Direction::South;
	}
}

bool Robot::pickupX() {
	if (map.getChangableTile(getX(), getY()).marks == 0) {
		return false;
	}
	map.getChangableTile(getX(), getY()).marks--;
	return true;
}

bool Robot::putdownX() {
	map.getChangableTile(getX(), getY()).marks++;
	return true;
}

bool Robot::ifwallX() {
	Position p = getPositionInFrontOfRobot();
	return map.isWallOrEnd(p.x, p.y);
}

bool Robot::ifmarksX() {
	return map.tile(x_pos).marks > 0;
}
