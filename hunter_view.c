////////////////////////////////////////////////////////////////////////
// COMP2521 19t0 ... the Fury of Dracula
// hunter_view.c: the HunterView ADT implementation
//
// 2014-07-01	v1.0	Team Dracula <cs2521@cse.unsw.edu.au>
// 2017-12-01	v1.1	Team Dracula <cs2521@cse.unsw.edu.au>
// 2018-12-31	v2.0	Team Dracula <cs2521@cse.unsw.edu.au>

#include <assert.h>
#include <err.h>
#include <stdbool.h>
#include <stdlib.h>
#include <sysexits.h>

#include <string.h>

#include "game.h"
#include "game_view.h"
#include "hunter_view.h"
// #include "map.h" ... if you decide to use the Map ADT

typedef struct characters {
	int health;
	location_t location;
} characters;

typedef characters *Characters;

typedef struct hunter_view {
	int turn;
	int score;
	
	int curr_player;
	Characters hts[4];
} hunter_view;

hunter_view *hv_new (char *past_plays, player_message messages[])
{
	hunter_view *new = malloc (sizeof *new);
	if (new == NULL) err (EX_OSERR, "couldn't allocate HunterView");

	// Following put into hunter_view
	int turn = 1;
	int score = 366;
	int vampire = 1; // True/false for immature vampire
	
	// Following used for every turn
	char location[2];
	char prevloc[2] = { '0', '0' }; // Hunter's prev location
	int player = -1; // To keep compiler happy
	for (int i = 0; i < 4; i++) new->hts[i]->health = 9; // All hts begin w/ 9 HP

	int n = 1; // Char count
	for (char *c = past_plays; *c != '\0'; c++)
	{
		switch (n % 8) // Read past_plays byte by byte b4 fully initialising hunter_view
		{
			case (0) :
				turn++; // ' ' = turn in past_plays
				score--; // Happens after dracula finishes his turn
				
				if (turn % 13 == 1) vampire = 1; // D left behind immature v.
				else if (vampire) vampire++;
				if (vampire == 7) { score -= 13; vampire = 0; } // V. matures
				
				if (player != PLAYER_DRACULA)
				{
					new->hts[player]->location = location_find_by_abbrev (location); // Update hunters' locations
					if (new->hts[player]->health <= 0)
					{
						new->hts[player]->health = 9;
						location[0] = 'J'; location[1] = 'M'; // Htr teleported to hospital
					}
				}
				
				prevloc[0] = location[0]; prevloc[1] = location[1]; // Redefine prev location

				break;
			case (1) :
				switch (*c)
				{
					case ('G') :
						player = PLAYER_LORD_GODALMING;
						break;
					case ('S') :
						player = PLAYER_DR_SEWARD;
						break;
					case ('H') :
						player = PLAYER_VAN_HELSING;
						break;
					case ('M') :
						player = PLAYER_MINA_HARKER;
						break;
					default :
						player = PLAYER_DRACULA;
				}
				break;
			case (2) :
				location[0] = *c;
				break;
			case (3) :
				location[1] = *c;
                                if (!strcmp (location, prevloc))
					new->hts[player]->health += 3; // If hts rested in city
				if (new->hts[player]->health > 9) new->hts[player]->health = 9; // Retain HP cap

				break;
			default :
                                if (player == PLAYER_DRACULA) break;
				
				if (*c == 'T') new->hts[player]->health -= 2;
				if (new->hts[player]->health <= 0) score -= 6;
				
				if (*c == 'V') vampire = 0;
				
				if (*c == 'D') new->hts[player]->health -= 4;
				if (new->hts[player]->health <= 0) score -= 6;
		}

		
		n++;
	}

	new->curr_player = ((turn - 1) % 5);
	new->turn = turn;
	new->score = score;

	return new;
}

void hv_drop (hunter_view *hv)
{
	/// @todo REPLACE THIS WITH YOUR OWN IMPLEMENTATION
	free (hv);
}

round_t hv_get_round (hunter_view *hv)
{
	/// @todo REPLACE THIS WITH YOUR OWN IMPLEMENTATION
	return 0;
}

enum player hv_get_player (hunter_view *hv)
{
	/// @todo REPLACE THIS WITH YOUR OWN IMPLEMENTATION
	return 0;
}

int hv_get_score (hunter_view *hv)
{
	/// @todo REPLACE THIS WITH YOUR OWN IMPLEMENTATION
	return 0;
}

int hv_get_health (hunter_view *hv, enum player player)
{
	/// @todo REPLACE THIS WITH YOUR OWN IMPLEMENTATION
	return 0;
}

location_t hv_get_location (hunter_view *hv, enum player player)
{
	/// @todo REPLACE THIS WITH YOUR OWN IMPLEMENTATION
	return 0;
}

void hv_get_trail (
	hunter_view *hv, enum player player,
	location_t trail[TRAIL_SIZE])
{
	/// @todo REPLACE THIS WITH YOUR OWN IMPLEMENTATION
}

location_t *hv_get_dests (
	hunter_view *hv, size_t *n_locations,
	bool road, bool rail, bool sea)
{
	/// @todo REPLACE THIS WITH YOUR OWN IMPLEMENTATION
	*n_locations = 0;
	return NULL;
}

location_t *hv_get_dests_player (
	hunter_view *hv, size_t *n_locations, enum player player,
	bool road, bool rail, bool sea)
{
	/// @todo REPLACE THIS WITH YOUR OWN IMPLEMENTATION
	*n_locations = 0;
	return NULL;
}
