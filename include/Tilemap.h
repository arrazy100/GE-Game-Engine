#ifndef TILEMAP_H
#define TILEMAP_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string>
#include <tmx.h>
#include <vector>

#include "Sprite.h"
#include "Shape.h"
#include "Box2D.h"

namespace GE
{
	class Tilemap
	{
	private:
		SDL_Renderer* _renderer = NULL;
		tmx_map* _map = NULL;
		tmx_layer* _layer = NULL;
		std::vector<GE::Sprite*> _tile_resources;
		std::vector<GE::Shape*> _object_resources;
	public:
		Tilemap(SDL_Renderer* renderer, std::string file);
		~Tilemap();
		void setColor(int color);
		void addImageLayer(tmx_image* image);
		void addTile(GE::Sprite* tile);
		void addLayer(tmx_layer* layer);
		void addObject(tmx_object_group* object_group);
		void addAllLayer();
		void render(double dt);
		void addObjectToWorld(b2World* world, std::string object_name);
	};
} // namespace GE

#endif