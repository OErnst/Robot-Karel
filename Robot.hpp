#ifndef KAREL_ROBOT_HPP
#define KAREL_ROBOT_HPP

#include "World.hpp"
#include "defs.hpp"

class Robot {
	//robot has attribute world so that he can easily perform instructions
private:
	Position x_pos;
	Direction x_direction = Direction::North;
	World map;
	bool checkPosition();
	Position getPositionInFrontOfRobot();

public:
	void setX(int x);
	void setY(int y);
	int getX();
	int getY();
	void setDirection(Direction d);
	World& getMap();
	const World& getConstMap() const;
	bool prepareWorld(std::istream& world);
	bool moveX();
	void leftX();
	void rightX();
	bool pickupX();
	bool putdownX();
	bool ifwallX();
	bool ifmarksX();
	/**
     * Returns the current position of the robot.
     */
	Position position() const;

	/**
     * Returns the current direction of the robot.
     */
	Direction direction() const;
};

#endif //KAREL_ROBOT_HPP
