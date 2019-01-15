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
#include "map.h" 

typedef struct characters {
	int health;
	location_t location;// Curr location
	location_t trail[TRAIL_SIZE]; // Last 6
} characters;

typedef characters *Characters;

typedef struct hunter_view {
	round_t round;
	int score;
	
	int curr_player;
	Characters hts[4];
} hunter_view;

hunter_view *hv_new (char *past_plays, player_message messages[])
{
	hunter_view *new = malloc (sizeof *new);
	if (new == NULL) err (EX_OSERR, "couldn't allocate HunterView");

	// Following put into hunter_view
	int turn = 0;
	int score = GAME_START_SCORE;
	int vampire = 0; // True/false for immature vampire
	
	// Following used for every turn
	char location[2];
	char prevloc[2] = { '0', '0' }; // Hunter's prev location
	int player = -1; // To keep compiler happy
	for (int i = 0; i < 4; i++)
	{
		new->hts[i]->health = GAME_START_HUNTER_LIFE_POINTS;
		for (int j = 0; j < TRAIL_SIZE; j++) new->hts[i]->trail[j] = -1; // Also initialise h. trails
	}

	int n = 1; // Char count
	for (char *c = past_plays; *c != '\0'; c++)
	{
		switch (n % 8) // Read past_plays byte by byte b4 fully initialising hunter_view
		{
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
					new->hts[player]->health += LIFE_GAIN_REST; // If hts rested in city
				if (new->hts[player]->health > 9) new->hts[player]->health = 9; // Retain HP cap

				break;
			
			case (0) :
                                turn++; // ' ' = turn in past_plays
                                score -= SCORE_LOSS_DRACULA_TURN; // Happens after dracula finishes his turn

                                if ((turn / 5) % 13 == 0 &&
                                                player == PLAYER_DRACULA && location[0] == 'C') vampire = 1; // D left behind immature v.
                                else if (vampire) vampire++;
                                if (vampire == 7) { score -= SCORE_LOSS_VAMPIRE_MATURES; vampire = 0; } // V. matures

                                if (player != PLAYER_DRACULA)
                                {
                                        if (new->hts[player]->health <= 0)
                                        {
                                                new->hts[player]->health = 9;
                                                location[0] = 'J'; location[1] = 'M'; // Htr teleported to hospital
                                        }
                                        new->hts[player]->location = location_find_by_abbrev (location); // Update hunters' locations
                                        new->hts[player]->trail[(turn / 5) % 6] = location_by_abbrev (location); // Update trail, overwriting oldest entry if full
                                }

                                prevloc[0] = location[0]; prevloc[1] = location[1]; // Redefine prev location

                                break;
			
			default :
                                if (player == PLAYER_DRACULA) break;
				
				if (*c == 'T') new->hts[player]->health -= LIFE_LOSS_TRAP_ENCOUNTER;
				if (new->hts[player]->health <= 0) score -= SCORE_LOSS_HUNTER_HOSPITAL;
				
				if (*c == 'V') vampire = 0;
				
				if (*c == 'D') new->hts[player]->health -= LIFE_LOSS_DRACULA_ENCOUNTER;
				if (new->hts[player]->health <= 0) score -= SCORE_LOSS_HUNTER_HOSPITAL;
		}

		
		n++;
	}

	new->curr_player = ((turn + 1) % 5); // +1 as curr_plyr is yet to make a move
	new->round = turn / 5;
	new->score = score;

	return new;
}

void hv_drop (hunter_view *hv)
{
	for (int i = 0; i < 4; i ++) free (hts[i]);
	free (hv);
}

round_t hv_get_round (hunter_view *hv)
{
	return hv->round;
}

enum player hv_get_player (hunter_view *hv)
{
	return hv->curr_player;
}

int hv_get_score (hunter_view *hv)
{
	return hv->score;
}

int hv_get_health (hunter_view *hv, enum player player)
{
	return hv->hts[player]->health;
}

location_t hv_get_location (hunter_view *hv, enum player player)
{
	return hv->hts[player]->location;
}

void hv_get_trail (
	hunter_view *hv, enum player player,
	location_t trail[TRAIL_SIZE])
{
	for (int i = 0; i < 6; i++) trail[i] = hv->hts[player]->trail[i];
	return;
}

location_t *hv_get_dests (
	hunter_view *hv, size_t *n_locations,
	bool road, bool rail, bool sea)
{
	return hv_get_dests_player (hv, n_locations, hv->curr_player, road, rail, sea);
}

location_t *hv_get_dests_player (
	hunter_view *hv, size_t *n_locations, enum player player,
	bool road, bool rail, bool sea)
{
	
	*n_locations = 0;
	return NULL;
}
