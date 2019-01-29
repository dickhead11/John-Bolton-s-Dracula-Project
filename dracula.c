////////////////////////////////////////////////////////////////////////
// COMP2521 19t0 ... the Fury of Dracula
// dracula.c: your "Fury of Dracula" Dracula AI
//
// 2014-07-01	v1.0	Team Dracula <cs2521@cse.unsw.edu.au>
// 2017-12-01	v1.1	Team Dracula <cs2521@cse.unsw.edu.au>
// 2018-12-31	v2.0	Team Dracula <cs2521@cse.unsw.edu.au>

#include "dracula.h"
#include "dracula_view.h"
#include "game.h"
#include <string.h>
#include "map.h"
#include "places.h"

void decide_dracula_move (DraculaView dv)
{	
	Map map = map_new ();
	size_t numLocations = 0;
	location_t *yo = dv_get_dests(dv, &numLocations, true, true);

	for (int i = 0; i < 4; i++) {
		location_t g = dv_get_location(dv, i);
		if ((strcmp(location_get_abbrev (g), "KL") == 0) || 
			(strcmp(location_get_abbrev (g), "GA") == 0)) {
			break;
			if (dv_get_health(dv, 4) <= 16 && i == 3) { 
				register_best_play ("CD", "coup averted");
				return;
			}
		}
	}
	if (dv_get_health(dv, 4) < 3) register_best_play ("CD", "coup averted");
	// code that gathers all reachable locations to hunters into a single array
	location_t reachableLocHunter[100];
	int j = 0, numPotentialLoc;
	for (int i = 0; i < 4; i++) {
		location_t g = dv_get_location(dv, i);
		int z = (i + dv_get_round(dv)) % 4;
		location_t *hunterLocations = reachable_locations (map, &numLocations, g, false, z, true, true);
		numPotentialLoc = (int) numLocations;
		for (int h = 0, j; h < numPotentialLoc; j++, h++) {
			reachableLocHunter[j] = hunterLocations[h];
		}
	}
	// code that moves dracula to the first location that is unreachable to the hunters
	// if no unreachable location, flies to the castle if castle is not surrounded
	yo = dv_get_dests(dv, &numLocations, true, true);
	for (int i = 0; i < numLocations; i++) {
		for (int z = 0, i; z < numPotentialLoc; z++) {
			if (yo[i] == reachableLocHunter[z]) break;
			if (z == numPotentialLoc - 1) {
				register_best_play (location_get_abbrev (yo[i]), "too rich to be caught");
				return;
			}
		}
		if (i == (int) numLocations - 1) {
			for (int i = 0; i < 4; i++) {
				location_t g = dv_get_location(dv, i);
				if ((strcmp(location_get_abbrev (g), "KL") == 0) || 
				(strcmp(location_get_abbrev (g), "GA") == 0))
				break;
				if (i == 3) { 
					register_best_play ("CD", "coup averted");
					return;
				}
			}
		// we are surrounded, but hunters are camping outside castle dracula 
		// and we still have plenty of blood points, so just make a normal move
			register_best_play (location_get_abbrev (yo[0]), "too rich to be caught");
		}
	}
	// random move
	/*location_t *gay = dv_get_dests(dv, &num_locations, true, true);
	char *meh = location_get_abbrev (&gay[0]);
	register_best_play (meh, "tricked"); */
}
