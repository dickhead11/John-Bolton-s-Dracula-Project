////////////////////////////////////////////////////////////////////////
// COMP2521 19t0 ... the Fury of Dracula
// map.h: an interface to a Map data type
//
// 2017-11-30   v1.0    Team Dracula <cs2521@cse.unsw.edu.au>
// 2018-12-31   v2.0    Team Dracula <cs2521@cse.unsw.edu.au>

#include <stdbool.h>
#include <stdlib.h>

#include "places.h"

#ifndef FOD__MAP_H_
#define FOD__MAP_H_

typedef struct edge {
    location_t start, end;
    transport_t type;
} edge;

// graph representation is hidden
typedef struct map *Map;
typedef struct map_adj *Adj;

/** Create a new Map. */
Map map_new (void);
/** Release resources associated with a Map. */
void map_drop (Map);
/** Print a Map to `stdout`. */
void map_show (Map);
/** Get the number of vertices. */
size_t map_nv (Map);
/** Get the number of edges. */
size_t map_ne (Map, transport_t);

// A function finding all reachable destinations for a player
location_t *find_adj (Map g, location_t location, int railpts, int drac,
        bool road, bool rail, bool sea);

// A recursive function to count all cities reachable by rail
void how_many_railpts (int railpts, Map g, Adj curr, location_t *adjacent);

#endif // !defined(FOD__MAP_H_)
