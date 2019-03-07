#include "World.hpp"

using namespace std;

size_t World::width() const {
	return x_width;
}

size_t World::height() const {
	return x_height;
}

bool World::createWorld(istream &world) {
	mapTable.resize(x_width, vector<Tile>(x_height));
	string row;
	u_int i = 0;
	while (getline(world, row)) {
		if (row.size() != x_width || i >= x_height) {
			Complain::invalidWorld(3 + i, "wrong length of line or too long file");
		}
		for (u_int j = 0; j < x_width; j++) {
			Tile tile;
			if (row[j] == '#') {
				tile.wall = true;
				tile.marks = 0;
			} else if (row[j] > 47 && row[j] < 58) {
				tile.wall = false;
				tile.marks = static_cast<unsigned int>(row[j] - 48);
			} else if (row[j] == ' ') {
				tile.wall = false;
				tile.marks = 0;
			} else {
				Complain::invalidWorld(3 + i, "wrong char");
			}
			mapTable[j][i] = tile;
		}
		i++;
		row = "";
	}
	if (i != x_height) {
		Complain::invalidWorld(3 + i, "too short file");
	}

	return true;
}

void World::setHeight(size_t h) {
	x_height = h;
}

void World::setWidth(size_t w) {
	x_width = w;
}

bool World::isWallOrEnd(int x, int y) {
	return x < 0 || y < 0 || x >= static_cast<int>(width()) || y >= static_cast<int>(height()) || mapTable[x][y].wall;
}

Tile World::tile(Position where) const {
	return mapTable[where.x][where.y];
}

Tile &World::getChangableTile(int x, int y) {
	return mapTable[x][y];
}
/*
void World::printWorld() {
	for (u_int y = 0; y < mapTable[0].size(); y++) {
		for (auto &x : mapTable) {
			if (x[y].wall) {
				cout << "#";
			} else {
				cout << x[y].marks;
			}
		}
		cout << "\n";
	}
}
*/
SecondLineParam World::doFirstTwoLines(std::istream &world) {
	string tempStr;
	getline(world, tempStr);
	vector<string> numbers = StaticTools::splitter(tempStr);
	if (numbers.size() != 2 || !StaticTools::isNumber(numbers[0]) || !StaticTools::isNumber(numbers[1])) {
		Complain::invalidWorld(1, "first row");
	}

	auto width = static_cast<u_int>(stoi(numbers[0]));
	auto height = static_cast<u_int>(stoi(numbers[1]));

	if (width < 1 || height < 1) {
		Complain::invalidWorld(1, "not positive width or height");
	}

	tempStr = "";
	getline(world, tempStr);

	vector<string> secondRow = StaticTools::splitter(tempStr);

	if (secondRow.size() != 3 || !StaticTools::isNumber(secondRow[0]) || !StaticTools::isNumber(secondRow[1]) || secondRow[2].size() != 1) {
		Complain::invalidWorld(2, "second row");
	}

	int x = stoi(secondRow[0]);
	int y = stoi(secondRow[1]);

	if (x < 0 || static_cast<size_t>(x) >= width || y < 0 || static_cast<size_t>(y) >= height) {
		Complain::invalidWorld(2, "robot out of map");
	}

	setWidth(width);
	setHeight(height);
	return { x, y, getDirFromChar(secondRow[2][0]) };
}

Direction World::getDirFromChar(char x) {
	switch (x) {
	case 'n':
		return Direction::North;
	case 's':
		return Direction::South;
	case 'e':
		return Direction::East;
	case 'w':
		return Direction::West;
	default:
		Complain::invalidWorld(2, "wrong direction");
	}
}
