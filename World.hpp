#ifndef KAREL_WORLD_HPP
#define KAREL_WORLD_HPP

#include "StaticTools.hpp"
#include "defs.hpp"
#include "errors.hpp"
#include <iostream>

struct SecondLineParam {
	int x;
	int y;
	Direction d;
};

class World {
private:
	size_t x_width = 0;
	size_t x_height = 0;
	std::vector<std::vector<Tile>> mapTable;

public:
	SecondLineParam doFirstTwoLines(std::istream& world);
	Direction getDirFromChar(char x);
	bool createWorld(std::istream& world);
	bool isWallOrEnd(int x, int y);
	Tile& getChangableTile(int x, int y);
	/**
     * Returns the width of the world, in tiles.
     */
	size_t width() const;

	/**
     * Returns the height of the world, in tiles.
     */
	size_t height() const;
	void setWidth(size_t w);
	void setHeight(size_t h);
	/**
     * Returns the tile on position tile.
     *
     * \param   where       the position of the desired tile.
     * \note    The return value is undefined if the \p where
     *          parameter is not a valid position in the world.
     */
	Tile tile(Position where) const;
};

#endif //KAREL_WORLD_HPP
