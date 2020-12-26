#include "Tilemap.h"

GE::Tilemap::Tilemap(SDL_Renderer* renderer, std::string file)
{
    _renderer = renderer;
	_map = tmx_load(file.c_str());
    _layer = _map->ly_head;
    addAllLayer();
}

GE::Tilemap::~Tilemap()
{
    _renderer = NULL;
    tmx_map_free(_map);
    _layer = NULL;
    for (int i = 0; i < _tile_resources.size(); i++)
    {
        delete(_tile_resources[i]);
    }
    _tile_resources.clear();
    for (int i = 0; i < _object_resources.size(); i++)
    {
        delete(_object_resources[i]);
    }
    _object_resources.clear();
    for (int i = 0; i < _collision_objects.size(); i++)
    {
        delete(_collision_objects[i]);
    }
    _collision_objects.clear();
}

void GE::Tilemap::setColor(int color)
{
    tmx_col_bytes col = tmx_col_to_bytes(color);
    SDL_SetRenderDrawColor(_renderer, col.r, col.g, col.b, col.a);
}

void GE::Tilemap::addImageLayer(tmx_image* image)
{
    GE::Sprite* image_layer = new GE::Sprite(_renderer, image->source);
    _tile_resources.push_back(image_layer);
}

void GE::Tilemap::addTile(GE::Sprite* tile)
{
    _tile_resources.push_back(tile);
}

void GE::Tilemap::addLayer(tmx_layer* layer)
{
    unsigned long i, j;
    unsigned int gid, x, y, w, h, flags;
    float op;
    tmx_tileset* ts;
    tmx_image* im;
    GE::Sprite* image = NULL;
    op = _layer->opacity;
    for (i = 0; i < _map->height; i++) {
        for (j = 0; j < _map->width; j++) {
            gid = (_layer->content.gids[(i * _map->width) + j]) & TMX_FLIP_BITS_REMOVAL;
            if (_map->tiles[gid] != NULL) {
                ts = _map->tiles[gid]->tileset;
                im = _map->tiles[gid]->image;
                x  = _map->tiles[gid]->ul_x;
                y  = _map->tiles[gid]->ul_y;
                w  = ts->tile_width;
                h  = ts->tile_height;
                if (im) {
                    image = new GE::Sprite(_renderer, im->source);
                }
                else {
                    image = new GE::Sprite(_renderer, ts->image->source);
                }
                flags = (_layer->content.gids[(i * _map->width) + j]) & ~TMX_FLIP_BITS_REMOVAL;
                double rect[4] = {(double)x, (double)y, (double)w, (double)h};
                image->setPosition((double)j * ts->tile_width, (double)i * ts->tile_height);
                image->setClip(rect);
                addTile(image);
            }
        }
    }
}

void GE::Tilemap::addObject(tmx_object_group* object_group)
{
    SDL_Rect rect;
    setColor(object_group->color);
    tmx_object* head = object_group->head;
    while (head) {
        std::string object_name(head->name);
        if (head->visible) {
            if (object_name == "collision")
            {
                rect.x = head->x;
                rect.y = head->y;
                rect.w = head->width;
                rect.h = head->height;
                GE::Physics* physic = new GE::Physics(true);
                physic->setBody(rect);
                _collision_objects.push_back(physic);
            }
            else if (head->obj_type == OT_SQUARE) {
                /*
                rect.x = head->x;
                rect.y = head->y;
                rect.w = head->width;
                rect.h = head->height;
                SDL_RenderDrawRect(_renderer, &rect);
                */
            }
            else if (head->obj_type  == OT_POLYGON) {
                //drawPolygon(head->content.shape->points, head->x, head->y, head->content.shape->points_len);
            }
            else if (head->obj_type == OT_POLYLINE) {
                //draw_polyline(head->content.shape->points, head->x, head->y, head->content.shape->points_len);
            }
            else if (head->obj_type == OT_ELLIPSE) {
                /* FIXME: no function in SDL2 */
            }
        }
        head = head->next;
    }
}

void GE::Tilemap::addAllLayer()
{
    while (_layer) {
        if (_layer->visible) {
            if (_layer->type == L_GROUP) {
                addAllLayer(); // recursive call
            }
            else if (_layer->type == L_OBJGR) {
                addObject(_layer->content.objgr);
            }
            else if (_layer->type == L_IMAGE) {
                addImageLayer(_layer->content.image); // Function to be implemented
            }
            else if (_layer->type == L_LAYER) {
                addLayer(_layer); // Function to be implemented
            }
        }
        _layer = _layer->next;
    }
}

void GE::Tilemap::render(double dt)
{
    setColor(_map->backgroundcolor);
    for(int i = 0; i < _tile_resources.size(); i++)
    {
        _tile_resources[i]->draw(dt);
    }
}

std::vector<GE::Physics*> GE::Tilemap::getObjectPhysics()
{
    return _collision_objects;
}