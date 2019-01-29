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
void decide_dracula_move (DraculaView dv)
{	
	size_t num_locations = 0;
	location_t *yo = dv_get_dests(dv, &numLocations, true, true);

	for (int i = 0; i < 4; i++) {
		location_t g = dv_get_location(dv, i);
		if ((strcmp(location_get_abbrev (g), "KL") == 0) || 
			strcmp(location_get_abbrev (g), "GA") == 0)) {
			break;
			if (dv_get_health(dv, 4) <= 16 && i == 3) { 
				register_best_play ("CD", "coup averted");
				return;
			}
		}
	}
	if (dv_get_health(dv, 4) < 3) register_best_play ("CD", "coup averted");
	char reachableLocHunters[100];
	int j = 0;
	for (int i = 0; i < 4; i++) {
		location_t g = dv_get_location(dv, i);
		int z = (i + dv_get_round(DraculaView dv)) % 4;
		location_t *hunterLocations = reachable_locations 
			(connections, &num_locations, g, false, z, true, true);
		int numPotentialLoc = strlen(hunterLocations);
		for (int h = 0, j; h <= numPotentialLoc; j++, h++) {
			strcpy(reachableLocHunter[j], hunterLocations[h]);
		}
	}
	yo = dv_get_dests(dv, &num_Locations, true, true);
	// code that moves dracula to the first location that is unreachable to the hunters
	// if no unreachable location, flies to the castle
	for (int i = 0; i < numLocations; i++); {
		for (int z = 0; z < strlen(reachableLocHunter), z++) {
			if (strcmp(yo[i], reachableLocHunter[z]) == 0) break;
			if (z == strlen(reachableLocHunter) - 1) {
				register_best_play (location_get_abbrev (yo[i]), "tricked");
				return;
			}
		}
		if (i == numLocations - 1;) {
			register_best_play ("CD", "coup averted");
			return;
		}
	}
	// random move
	/*location_t *gay = dv_get_dests(dv, num_locations, true, true);
	const char *meh = location_get_abbrev (&gay[0]);
	register_best_play (meh, "tricked"); */
}
