#include <Tilemap.h>

struct UserData
{
    std::string name;
};

GE::Tilemap::Tilemap(SDL_Renderer* renderer, std::string file)
{
    _renderer = renderer;
	_map = tmx_load(file.c_str());
}

GE::Tilemap::~Tilemap()
{
    _renderer = NULL;
    tmx_map_free(_map);
    _layer = NULL;
    for (int i = 0; i < _tiles.size(); i++)
    {
        delete(_tiles[i]);
    }
    _tiles.clear();
    for (int i = 0; i < _objects.size(); i++)
    {
        delete(_objects[i]);
    }
    _objects.clear();
    _removable_objects.clear();
}

void GE::Tilemap::setColor(int color)
{
    tmx_col_bytes col = tmx_col_to_bytes(color);
    SDL_SetRenderDrawColor(_renderer, col.r, col.g, col.b, col.a);
}

void GE::Tilemap::addImageLayer(tmx_image* image)
{
    GE::Sprite* image_layer = new GE::Sprite(_renderer, image->source);
    _tiles.push_back(image_layer);
}

void GE::Tilemap::addTile(GE::Sprite* tile)
{
    _tiles.push_back(tile);
}

void GE::Tilemap::addLayer(tmx_layer* layer)
{
    unsigned long i, j;
    unsigned int gid, x, y, w, h, flags;
    float op;
    tmx_tileset* ts;
    tmx_image* im;
    GE::Sprite* image = NULL;
    op = layer->opacity;
    for (i = 0; i < _map->height; i++) {
        for (j = 0; j < _map->width; j++) {
            gid = (layer->content.gids[(i * _map->width) + j]) & TMX_FLIP_BITS_REMOVAL;
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
                flags = (layer->content.gids[(i * _map->width) + j]) & ~TMX_FLIP_BITS_REMOVAL;
                double rect[4] = {(double)x, (double)y, (double)w, (double)h};
                image->setPosition((double)j * ts->tile_width, (double)i * ts->tile_height);
                image->setClip(rect);
                addTile(image);
            }
        }
    }
}

void GE::Tilemap::addLayerToWorld(b2World* world, tmx_layer* layer, bool is_static, bool is_sensor)
{
    unsigned long i, j;
    unsigned int gid, x, y, w, h, flags;
    float op;
    tmx_tileset* ts;
    tmx_image* im;
    std::string img = "";
    op = layer->opacity;
    for (i = 0; i < _map->height; i++) {
        for (j = 0; j < _map->width; j++) {
            gid = (layer->content.gids[(i * _map->width) + j]) & TMX_FLIP_BITS_REMOVAL;
            if (_map->tiles[gid] != NULL) {
                ts = _map->tiles[gid]->tileset;
                im = _map->tiles[gid]->image;
                x  = _map->tiles[gid]->ul_x;
                y  = _map->tiles[gid]->ul_y;
                w  = ts->tile_width;
                h  = ts->tile_height;
                if (im) {
                    img = im->source;
                }
                else {
                    img = ts->image->source;
                }
                flags = (layer->content.gids[(i * _map->width) + j]) & ~TMX_FLIP_BITS_REMOVAL;
                double rect[4] = {(double)x, (double)y, (double)w, (double)h};
                std::shared_ptr<GE::Sprite> image(new GE::Sprite(_renderer, img));
                image->setPosition((double)j * ts->tile_width, (double)i * ts->tile_height);
                image->setClip(rect);
                std::string name(layer->name);
                std::shared_ptr<GE::Box2D> box(new GE::Box2D(world, image->getPositionX(), image->getPositionY(), rect[2], rect[3], is_static, is_sensor, name));
                _removable_objects.push_back(std::make_pair(image, box));
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
        if (head->visible) {
            if (head->obj_type == OT_SQUARE) {
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
    for (int i = 0; i < _tiles.size(); i++)
    {
        _tiles[i]->render(dt);
    }
    for (auto it = _removable_objects.begin(); it != _removable_objects.end(); it++)
    {
        it->first->render(dt);
    }
}

void GE::Tilemap::addNormalLayer(std::string layer_name)
{
    tmx_layer* layer = _map->ly_head;
    while (layer)
    {
        if (layer->visible)
        {
            if (layer->type == L_LAYER)
            {
                std::string name(layer->name);
                if (name == layer_name)
                    addLayer(layer);
            }
        }
        layer = layer->next;
    }
}

void GE::Tilemap::addPhysicsFromObject(b2World* world, std::string object_name)
{
    tmx_layer* layer = _map->ly_head;
    while (layer)
    {
        if (layer->visible)
        {
            if (layer->type == L_OBJGR)
            {
                tmx_object_group* object_group = layer->content.objgr;
                tmx_object* head = object_group->head;
                while (head) {
                    std::string object_name(head->name);
                    if (object_name == object_name)
                    {
                        _objects.push_back(new GE::Box2D(world, head->x, head->y, head->width, head->height, true, false, object_name));
                    }
                    head = head->next;
                }
            }
        }
        layer = layer->next;
    }
}

void GE::Tilemap::addRemovableObjectToWorld(b2World* world, std::string layer_name, bool is_static, bool is_sensor)
{
    tmx_layer* layer = _map->ly_head;
    while (layer)
    {
        if (layer->visible)
        {
            if (layer->type == L_LAYER)
            {
                std::string name(layer->name);
                if (name == layer_name)
                    addLayerToWorld(world, layer, is_static, is_sensor);
            }
        }
        layer = layer->next;
    }
}

std::vector<std::pair<std::shared_ptr<GE::Sprite>, std::shared_ptr<GE::Box2D>>>& GE::Tilemap::getRemovableObjects()
{
    return _removable_objects;
}

std::vector<GE::Box2D*>& GE::Tilemap::getObjects()
{
    return _objects;
}