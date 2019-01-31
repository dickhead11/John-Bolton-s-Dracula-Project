////////////////////////////////////////////////////////////////////////
// COMP2521 19t0 ... the Fury of Dracula
// hunter.c: your "Fury of Dracula" hunter AI.
//
// 2014-07-01	v1.0	Team Dracula <cs2521@cse.unsw.edu.au>
// 2017-12-01	v1.1	Team Dracula <cs2521@cse.unsw.edu.au>
// 2018-12-31	v2.0	Team Dracula <cs2521@cse.unsw.edu.au>

#include "game.h"
#include "hunter.h"
#include "hunter_view.h"
#include "map.h"

static const char *first_moves (HunterView);
static location_t known_location (HunterView hv);
static location_t known_minions (HunterView hv);
static const char *random_move (HunterView hv);

void decide_hunter_move (HunterView hv)
{
	const char *move = NULL;
	size_t n = 0;
	location_t *array = hv_get_dests (hv, &n, true, true, true);
	location_t trail[6];
    hv_get_trail (hv, hv_get_player (hv), trail);
    location_t d_trail[6];
    hv_get_trail (hv, PLAYER_DRACULA, d_trail);
	
	if (!hv_get_round (hv)) move = first_moves (hv); // Spread out
	else if (known_location (hv) <= 70 && // Make way closer to Dracula;
	         trail[1] != known_location (hv)) // if encountered Dracula last turn, rest
	    move = fastest_way (array, n, trail[0], known_location (hv));
    else if (known_minions (hv) != CITY_UNKNOWN &&
             trail[0] != known_minions (hv))
        move = fastest_way (array, n, trail[0], known_minions (hv));
	else if (d_trail[0] == SEA_UNKNOWN || d_trail[1] == SEA_UNKNOWN)
	    move = fastest_way (array, n, trail[0], SEA_UNKNOWN); // Move to nearest port
    else if (hv_get_round (hv) == 1) // Prevents Dracula shadowing
	    move = location_get_abbrev (trail[0]);
    else if (hv_get_round (hv) >= TRAIL_SIZE) // If nil known, research
        move = location_get_abbrev (trail[0]);
    /*else if (strcmp (first_moves (hv),
             location_get_abbrev (hv->locHistory[hv->currPlayer][0]))) // Move to starting position
        move = fastest_way (array, n, hv->locHistory[hv->currPlayer][0],
               location_find_by_abbrev (first_moves));
    */else // Default move
        move = random_move (hv);
	
	register_best_play (move, "");
}

// Spreads our hunters out for the first round
static const char *first_moves (HunterView hv)
{
    if (!hv_get_player (hv)) return "MI";
    if (hv_get_player (hv) == 1) return "MA";
    if (hv_get_player (hv) == 2) return "BU";
    if (hv_get_player (hv) == 3) return "SZ";
    
    return "??";
}

// Searches for any known location in Dracula's trail to head for
static location_t known_location (HunterView hv)
{
    location_t trail[TRAIL_SIZE];
    hv_get_trail (hv, PLAYER_DRACULA, trail);
    for (int i = 0; i < TRAIL_SIZE; i++)
        if (trail[i] <= 70)
            return trail[i];
    return CITY_UNKNOWN;
}

// Finds the latest known trap or vampire
static location_t known_minions (HunterView hv)
{
    location_t minion = CITY_UNKNOWN;
    //(hv_get_player (hv) + 4) % 5
    
    return minion;
}

// Chooses a random move -- unpredictable to Dracula
static const char *random_move (HunterView hv)
{
    size_t n = 0;
    location_t *array = hv_get_dests (hv, &n, true, true, true);
    int choice = (int) (((unsigned int) hv_get_score (hv) + (unsigned int) hv_get_round (hv) * (unsigned int) hv_get_round (hv)
                + hv_get_player (hv)) % (unsigned int) n);
    if (array[choice] == hv_get_location (hv, hv_get_player (hv))) // Don't choose current position
        choice = (choice + 1) % (int) n;
    return location_get_abbrev (array[choice]);
}
