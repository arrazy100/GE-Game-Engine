#ifndef TILEMAP_H
#define TILEMAP_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <memory>
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
		std::vector<GE::Sprite*> _tiles;
		std::vector<GE::Box2D*> _objects;
		std::vector<std::pair<std::shared_ptr<GE::Sprite>, std::shared_ptr<GE::Box2D>>> _removable_objects;
	public:
		Tilemap(SDL_Renderer* renderer, std::string file);
		~Tilemap();
		void setColor(int color);
		void addImageLayer(tmx_image* image);
		void addTile(GE::Sprite* tile);
		void addLayer(tmx_layer* layer);
		void addLayerToWorld(b2World* world, tmx_layer* layer, bool is_static, bool is_sensor);
		void addObject(tmx_object_group* object_group);
		void addAllLayer();
		void render(double dt);
		void addNormalLayer(std::string layer_name);
		void addPhysicsFromObject(b2World* world, std::string object_name);
		void addRemovableObjectToWorld(b2World* world, std::string layer_name, bool is_static, bool is_sensor);
		std::vector<std::pair<std::shared_ptr<GE::Sprite>, std::shared_ptr<GE::Box2D>>>& getRemovableObjects();
		std::vector<GE::Box2D*>& getObjects();
	};
} // namespace GE

#endif