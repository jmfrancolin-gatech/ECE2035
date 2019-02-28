#include "map.h"
#include "mbed.h"
#include "globals.h"
#include "graphics.h"

/**
 * The Map structure. This holds a HashTable for all the MapItems, along with
 * values for the width and height of the Map.
 */
struct Map {
    HashTable* items;
    int w, h;
};

/**
 * Storage area for the maps.
 * This is a global variable, but can only be access from this file because it
 * is static.
 */
static Map map;
static int active_map;

/**
 * The first step in HashTable access for the map is turning the two-dimensional
 * key information (x, y) into a one-dimensional unsigned integer.
 * This function should uniquely map (x,y) onto the space of unsigned integers.
 */
static unsigned XY_KEY(int X, int Y) {
    
    return X *(map.h) + Y;
}

/**
 * This is the hash function actually passed into createHashTable. It takes an
 * unsigned key (the output of XY_KEY) and turns it into a hash value (some
 * small non-negative integer).
 */
unsigned map_hash(unsigned key)
{
    return key % NUM_BUCKETS;
}

void maps_init()
{  
    // Initialize hash table
    map.items = createHashTable(map_hash,  NUM_BUCKETS);
    // Set width & height
    map.h = HEIGHT;
    map.w = WIDTH;
}

Map* get_active_map()
{
    // There's only one map
    return &map;
}

Map* set_active_map(int m)
{
    active_map = m;
    return &map;
}

void print_map()
{
    // As you add more types, you'll need to add more items to this array.
    char lookup[] = {'W', 'P', 'K', 'O', 'L', 'D', 'X'};
    for(int y = 0; y < map_height(); y++)
    {
        for (int x = 0; x < map_width(); x++)
        {
            MapItem* item = get_here(x,y);
            if (item) pc.printf("%c", lookup[item->type]);
            else pc.printf(" ");
        }
        pc.printf("\r\n");
    }
}

int map_width()
{
    Map* map = get_active_map();
    return map -> w;
}

int map_height()
{
    Map* map = get_active_map();
    return map -> h;
}

int map_area()
{
    Map* map = get_active_map();
    return (map ->h * map ->w);
}

MapItem* get_north(int x, int y)
{
    Map* map = get_active_map();
    int key = XY_KEY(x, y - 1);
    return (MapItem*)getItem(map -> items, key);
}

MapItem* get_south(int x, int y)
{
    Map* map = get_active_map();
    int key = XY_KEY(x, y + 1);
    return (MapItem*)getItem(map -> items, key);
}

MapItem* get_east(int x, int y)
{
    Map* map = get_active_map();
    int key = XY_KEY(x + 1, y);
    return (MapItem*)getItem(map -> items, key);
}

MapItem* get_west(int x, int y)
{
    Map* map = get_active_map();
    int key = XY_KEY(x - 1, y);
    return (MapItem*)getItem(map -> items, key);
}

MapItem* get_here(int x, int y)
{
    Map* map = get_active_map();
    int key = XY_KEY(x, y);
    return (MapItem*)getItem(map -> items, key);
}


void map_erase(int x, int y)
{
    Map* map = get_active_map();
    int key = XY_KEY(x, y);
    deleteItem(map -> items, key);
}

void add_wall(int x, int y, int dir, int len)
{
    for(int i = 0; i < len; i++)
    {
        MapItem* w1 = (MapItem*) malloc(sizeof(MapItem));
        w1->type = WALL;
        w1->draw = draw_wall;
        w1->walkable = omnipotent;
        w1->data = false;
        unsigned key = (dir == HORIZONTAL) ? XY_KEY(x+i, y) : XY_KEY(x, y+i);
        void* val = insertItem(get_active_map()->items, key, w1);
        if (val) free(val); // If something is already there, free it
    }
}

void add_plant(int x, int y)
{
    MapItem* w1 = (MapItem*) malloc(sizeof(MapItem));
    w1->type = PLANT;
    w1->draw = draw_plant;
    w1->walkable = omnipotent;
    w1->data = false;
    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), w1);
    if (val) free(val); // If something is already there, free it
}

void add_key(int x, int y)
{
    MapItem* w1 = (MapItem*) malloc(sizeof(MapItem));
    w1->type = KEY;
    w1->draw = draw_key;
    w1->walkable = omnipotent;
    w1->data = false;
    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), w1);
    if (val) free(val); // If something is already there, free it
}

void add_old_man(int x, int y)
{
    MapItem* w1 = (MapItem*) malloc(sizeof(MapItem));
    w1->type = OLD_MAN;
    w1->draw = draw_old_man;
    w1->walkable = omnipotent;
    w1->data = false;
    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), w1);
    if (val) free(val); // If something is already there, free it
}

void add_leef(int x, int y)
{
    MapItem* w1 = (MapItem*) malloc(sizeof(MapItem));
    w1->type = LEEF;
    w1->draw = draw_leef;
    w1->walkable = omnipotent;
    w1->data = false;
    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), w1);
    if (val) free(val); // If something is already there, free it
}

void add_danger(int x, int y)
{
    MapItem* w1 = (MapItem*) malloc(sizeof(MapItem));
    w1->type = DANGER;
    w1->draw = draw_danger;
    w1->walkable = omnipotent;
    w1->data = false;
    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), w1);
    if (val) free(val); // If something is already there, free it
}

void add_door(int x, int y)
{
    MapItem* w1 = (MapItem*) malloc(sizeof(MapItem));
    w1->type = XDOOR;
    w1->draw = draw_door;
    w1->walkable = omnipotent;
    w1->data = false;
    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), w1);
    if (val) free(val); // If something is already there, free it
}

void add_batman(int x, int y)
{
    MapItem* w1 = (MapItem*) malloc(sizeof(MapItem));
    w1->type = BATMAN;
    w1->draw = draw_batman;
    w1->walkable = omnipotent;
    w1->data = false;
    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), w1);
    if (val) free(val); // If something is already there, free it
}

void add_mushroom(int x, int y)
{
    MapItem* w1 = (MapItem*) malloc(sizeof(MapItem));
    w1->type = MUSHROOM;
    w1->draw = draw_mushroom;
    w1->walkable = omnipotent;
    w1->data = false;
    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), w1);
    if (val) free(val); // If something is already there, free it
}