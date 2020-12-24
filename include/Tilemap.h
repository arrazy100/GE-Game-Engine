#ifndef TILEMAP_H
#define TILEMAP_H

#include <SDL2/SDL.h>
//#include <sqlite3.h>
#include <string>
#include <vector>

#include "Sprite.h"

namespace GE
{
	class Tilemap
	{
	private:
        int _tilewidth = 32, _tileheight = 32;
		std::vector<GE::Sprite> _tilesets;
        std::vector<GE::Sprite> _tiles;

	public:
		Tilemap(std::string map_db, int map_id);
        void loadMap(std::string map_db, int map_id);
        void draw();
	};
} // namespace GE

#endif