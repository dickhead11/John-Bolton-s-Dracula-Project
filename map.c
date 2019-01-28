////////////////////////////////////////////////////////////////////////
// COMP2521 19t0 ... the Fury of Dracula
// map.c: an implementation of a Map type
//
// Code by TheGroup, COMP1927 14s2

#include <assert.h>
#include <err.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <sysexits.h>

#include "map.h"
#include "places.h"

typedef struct map_adj map_adj;
typedef struct map {
	size_t n_vertices, n_edges;

	struct map_adj {
		location_t v;  // ALICANTE, etc
		transport_t type; // ROAD, RAIL, BOAT
		struct map_adj *next; // link to next node
	} *connections[NUM_MAP_LOCATIONS]; // array of lists
} map;

static void add_connections (map *);
static void add_connection (map *, location_t, location_t, transport_t);
static inline bool is_sentinel_edge (connection);

static map_adj *adjlist_insert (map_adj *, location_t, transport_t);
static bool adjlist_contains (map_adj *, location_t, transport_t);

static void include_reachable_by_rail (
	Map map, bool reachable[NUM_MAP_LOCATIONS], location_t from, int rail_len);
	
static int memb (location_t elem, location_t *array, int size);
static int sea_connections (map *, location_t);

// Create a new empty graph (for a map)
// #Vertices always same as NUM_PLACES
map *map_new (void)
{
	map *g = malloc (sizeof *g);
	if (g == NULL) err (EX_OSERR, "couldn't allocate Map");

	(*g) = (map) {
		.n_vertices = NUM_MAP_LOCATIONS,
		.n_edges = 0,
		.connections = { NULL }
	};

	add_connections (g);
	return g;
}

// Remove an existing graph
void map_drop (map *g)
{
	assert (g != NULL);

	for (size_t i = 0; i < g->n_vertices; i++) {
		map_adj *curr = g->connections[i];
		while (curr != NULL) {
			map_adj *next = curr->next;
			free (curr);
			curr = next;
		}
	}
	free (g);
}

// Display content of Map/Graph
void map_show (map *g)
{
	assert (g != NULL);

	printf ("V=%zu, E=%zu\n", g->n_vertices, g->n_edges);
	for (size_t i = 0; i < g->n_vertices; i++)
		for (map_adj *curr = g->connections[i];
			 curr != NULL; curr = curr->next)
			printf (
				"%s connects to %s by %s\n",
				location_get_name ((location_t) i),
				location_get_name (curr->v),
				transport_to_s (curr->type)
			);
}

// Return count of nodes
size_t map_nv (map *g)
{
	assert (g != NULL);
	return g->n_vertices;
}

// Return count of edges of a particular type
size_t map_ne (map *g, transport_t type)
{
	assert (g != NULL);
	assert (valid_transport_p (type) || type == ANY);

	size_t n_edges = 0;

	for (size_t i = 0; i < g->n_vertices; i++)
		for (map_adj *curr = g->connections[i];
			 curr != NULL; curr = curr->next)
			if (curr->type == type || type == ANY)
				n_edges++;

	return n_edges;
}

/// Add edges to Graph representing map of Europe
static void add_connections (map *g)
{
	assert (g != NULL);

	for (size_t i = 0; ! is_sentinel_edge (CONNECTIONS[i]); i++)
		add_connection (
			g,
			CONNECTIONS[i].v, CONNECTIONS[i].w,
			CONNECTIONS[i].t
		);
}


/// Add a new edge to the Map/Graph
static void add_connection (
	map *g,
	location_t start,
	location_t end,
	transport_t type)
{
	assert (g != NULL);
	assert (start != end);
	assert (valid_location_p (start));
	assert (valid_location_p (end));
	assert (valid_transport_p (type));

	// don't add edges twice
	if (adjlist_contains (g->connections[start], end, type)) return;

	g->connections[start] = adjlist_insert (g->connections[start], end, type);
	g->connections[end]   = adjlist_insert (g->connections[end], start, type);
	g->n_edges++;
}

/// Is this the magic 'sentinel' edge?
static inline bool is_sentinel_edge (connection x)
{
	return x.v == -1 && x.w == -1 && x.t == ANY;
}

/// Insert a node into an adjacency list.
static map_adj *adjlist_insert (map_adj *list, location_t v, transport_t type)
{
	assert (valid_location_p (v));
	assert (valid_transport_p (type));

	map_adj *new = malloc (sizeof *new);
	if (new == NULL) err (EX_OSERR, "couldn't allocate map_adj");
	(*new) = (map_adj){ .v = v, .type = type, .next = list };
	return new;
}

/// Does this adjacency list contain a particular value?
static bool adjlist_contains (map_adj *list, location_t v, transport_t type)
{
	assert (valid_location_p (v));
	assert (valid_transport_p (type));

	for (map_adj *curr = list; curr != NULL; curr = curr->next)
		if (curr->v == v && curr->type == type)
			return true;
	return false;
}

////////////////////////////////////////////////////////////////////////

location_t *reachable_locations (
	Map map, size_t *num_locations, location_t from,
	bool drac, int rail_len, bool road, bool sea)
{
    //a boolean for each location, if it is reachable
	bool reachable[NUM_MAP_LOCATIONS] = { false };

	//setting the 'from' location as reachable
	reachable[from] = 1;

    //for each connection that is by ROAD or SEA,
	//set it to reachable if road or sea is set to true
	for (map_adj *curr = map->connections[from];
		 curr != NULL; curr = curr->next)
		if ((curr->type == ROAD && road) ||
			(curr->type == BOAT && sea))
			reachable[curr->v] = 1;

	//include the places reachable by rail
	include_reachable_by_rail (map, reachable, from, rail_len);

	//going through and putting every reachable location into an array
	location_t *locations = malloc (NUM_MAP_LOCATIONS * sizeof (location_t));
	size_t index = 0;
	for (location_t loc = MIN_MAP_LOCATION; loc <= MAX_MAP_LOCATION; loc++)
		if (reachable[loc])
			//don't allow dracula to go to the hospital
			if (! (drac && loc == ST_JOSEPH_AND_ST_MARYS))
				locations[index++] = loc;

	//setting the number of locations actually returned
	*num_locations = index;

	return locations;
}

static void include_reachable_by_rail (
	Map map, bool reachable[NUM_MAP_LOCATIONS],
	location_t from, int rail_len)
{
	assert(rail_len >= 0);

	reachable[from] = 1;

	if (rail_len <= 0) return;

	for (map_adj *curr = map->connections[from];
		 curr != NULL; curr = curr->next)
		if (curr->type == RAIL)
			include_reachable_by_rail (map, reachable, curr->v, rail_len - 1);
}

// Does BFS to find shortest path from a to b
const char *fastest_way (location_t *array, size_t n, location_t from, location_t dest)
{
    if (from == dest) return location_get_abbrev (from);
    
    Map g = map_new ();
    location_t pre[NUM_MAP_LOCATIONS];
    int dist[NUM_MAP_LOCATIONS];
    for (int i = 0; i < NUM_MAP_LOCATIONS; i++) { pre[i] = -1; dist[i] = -1; }
    
    location_t curr = from; dist[curr] = 0; pre[curr] = -2;
    while (1)
    {
        if (curr == from)
        {
            int m;
            for (m = 0; m < n; m++)
            {
                if (dist[array[m]] == -1) dist[array[m]] = dist[curr] + 1;
                if ((dest == SEA_UNKNOWN && sea_connections (g, array[m])) || // Target ports
                    array[m] == dest)
                {
                    pre[array[m]] = curr;
                    break;
                }
            }
            if (m != n)
            {
                int j;
                for (j = array[m]; pre[j] != from; j = pre[j]);
                map_drop (g);
                return location_get_abbrev (j);// First connection
            }
        }
        else
        {
            map_adj *close;
            for (close = g->connections[curr]; close != NULL; close = close->next)
            {
                if (dist[close->v] == -1) dist[close->v] = dist[curr] + 1;
                if ((dest == SEA_UNKNOWN && sea_connections (g, close->v)) || // Target ports
                    close->v == dest)
                {
                    pre[close->v] = curr;
                    break;
                }
            }
            
            if (close != NULL)
            {
                int j;
                for (j = close->v; pre[j] != from; j = pre[j]);
                map_drop (g);
                return location_get_abbrev (j);// First connection
            }
        }
        
        // closest is arbitrarily big
        int closest = NUM_MAP_LOCATIONS * NUM_MAP_LOCATIONS, cl_index = -1;
        for (int i = 0; i < NUM_MAP_LOCATIONS; i++)
            if (dist[i] != -1  && dist[i] < closest && pre[i] == -1)
            {
                closest = dist[i];
                cl_index = i;
            }
            
        if (cl_index == -1) // Should never get here
        {
            map_drop (g);
            return location_get_abbrev (-1);//(g->connections[from]->next->v);
        }
        
        for (map_adj *k = g->connections[cl_index]; k != NULL; k = k->next)
        {
            if (dist[k->v] == dist[cl_index] && memb (k->v, array, n)) pre[cl_index] = from;
            if (dist[k->v] == dist[cl_index] - 1) pre[cl_index] = k->v;
        }
        
        curr = cl_index;
    }
}

// Determines if element member of array
static int memb (location_t elem, location_t *array, int size)
{
    for (int i = 0; i < size; i++)
        if (elem == array[i]) return 1;
        
    return 0;
}

// Determines if a locations is a port
static int sea_connections (map *g, location_t v)
{
    for (map_adj *curr = g->connections[v]; curr != NULL; curr = curr->next)
        if (curr->type == BOAT) return 1;
        
    return 0;
}
