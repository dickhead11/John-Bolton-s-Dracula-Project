////////////////////////////////////////////////////////////////////////
// COMP2521 19t0 ... the Fury of Dracula
// dracula_view.c: the DraculaView ADT implementation
//
// 2014-07-01	v1.0	Team Dracula <cs2521@cse.unsw.edu.au>
// 2017-12-01	v1.1	Team Dracula <cs2521@cse.unsw.edu.au>
// 2018-12-31	v2.0	Team Dracula <cs2521@cse.unsw.edu.au>

#include <assert.h>
#include <err.h>
#include <stdbool.h>
#include <stdlib.h>
#include <sysexits.h>

#include "dracula_view.h"
#include "game.h"
#include "game_view.h"
#include "map.h"

typedef struct characters
{
        int health;
        location_t location;
        location_t trail[TRAIL_SIZE];
} characters;

typedef characters *Characters;

typedef struct dracula_view
{
        round_t round;
	int turn;
        int score;
	int vamps[TRAIL_SIZE];
	int traps[TRAIL_SIZE]; // Disappear after 6 moves
        int curr_player;
        Characters hts[5];

} dracula_view;

dracula_view *dv_new (char *past_plays, player_message messages[])
{
	dracula_view *new = malloc (sizeof *new);
	if (new == NULL) err (EX_OSERR, "couldn't allocate DraculaView");

	for (int k = 0; k < 5; k++)
	{
		new->hts[k] = malloc (sizeof (characters));
		if (new->hts[k] == NULL) err (EX_OSERR, "coulnd't allocate characters field");
	}

	// Initialise for safety
	for (int i = 0; i < TRAIL_SIZE; i++) { new->vamps[i] = 0; new->traps[i] = 0; }
	
	// Following put into dracula_view
	int turn = 0;
	new->curr_player = PLAYER_LORD_GODALMING;
	int score = GAME_START_SCORE;
	
	// Following used for every turn
	char location[2] = { '?', '?' }; // Of current read player
	int player = 0; // To keep compiler happy
	for (int i = 0; i < 5; i++)
	{
		new->hts[i]->health = GAME_START_HUNTER_LIFE_POINTS;
		new->hts[i]->location = UNKNOWN_LOCATION; //location_find_by_abbrev ("JM");
		for (int j = 0; j < TRAIL_SIZE; j++) new->hts[i]->trail[j] = UNKNOWN_LOCATION; // Also initialise h. trails
	}
	// And Dracula
	new->hts[4]->health = GAME_START_BLOOD_POINTS;
	new->hts[4]->location = UNKNOWN_LOCATION; //location_find_by_abbrev ("CD");

	int n = 1; // Char count
	for (char *c = past_plays; /**c != '\0'*/; c++)
	{
		if (n == 1 && *c == '\0') break;
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
                                if (player != PLAYER_DRACULA)
				{
					if (location_find_by_abbrev (location) == new->hts[player]->trail[(turn / NUM_PLAYERS ) % TRAIL_SIZE])
						new->hts[player]->health += LIFE_GAIN_REST; // If hts rested in city
					if (new->hts[player]->health > 9) new->hts[player]->health = 9; // Retain HP cap
				}

				break;
			
			case (0) :
                                if (player != PLAYER_DRACULA)
                                {
                                        if (new->hts[player]->health <= 0)
                                        {
                                                new->hts[player]->health = 9;
                                                location[0] = 'J'; location[1] = 'M'; // Htr teleported to hospital
                                        }
					
					if (location[0] == '?')
					{
						new->hts[player]->location = UNKNOWN_LOCATION;
						new->hts[player]->trail[(turn / NUM_PLAYERS) % TRAIL_SIZE] = UNKNOWN_LOCATION; 
					}

					else
					{
						new->hts[player]->location = location_find_by_abbrev (location); // Update hunters' locations
	                                        new->hts[player]->trail[(turn / NUM_PLAYERS) % TRAIL_SIZE] = location_find_by_abbrev (location); // Update trail, overwriting oldest entry if full
					}
                                }
				else
				{
                                	score -= SCORE_LOSS_DRACULA_TURN; // Happens after dracula finishes his turn
					
					/*if (location[0] == 'T' && location [1] == 'P') new->hts[player]->location = location_find_by_abbrev ("CD");
					else if (location[0] == 'D' && location[1] >= '1' && location[1] <= '9')
					{
						int index = (turn / NUM_PLAYERS) % TRAIL_SIZE - location[1] + 48; 
						new->hts[player]->location = new->hts[player]->trail[index + 6 * (index < 0)]; // Wrap around if index < 0
						if (new->hts[player]->trail[index + 6 * (index < 0)] == SEA_UNKNOWN) new->hts[player]->health -= LIFE_LOSS_SEA;
					}
					else if (location[0] == 'H' && location[1] == 'I')
					{
						int index = (turn / NUM_PLAYERS) % TRAIL_SIZE - 1;
						new->hts[player]->location = new->hts[player]->trail[index - 1 + 6 * (index < 0)];
						if (new->hts[player]->trail[index - 1 + 6 * (index < 0)] == SEA_UNKNOWN) new->hts[player]->health -= LIFE_LOSS_SEA;
					}
					else*/ new->hts[player]->location = location_find_by_abbrev (location);
					
					new->hts[player]->trail[(turn / NUM_PLAYERS) % TRAIL_SIZE] = new->hts[player]->location;
					if (location[0] == 'C' && location[1] == 'D') new->hts[player]->health += LIFE_GAIN_CASTLE_DRACULA;
					if (valid_location_p (new->hts[player]->location) && location_get_type (new->hts[player]->location) == SEA)
						new->hts[player]->health -= LIFE_LOSS_SEA;
				}
                                turn++; // ' ' = turn in past_plays

                                break;
			
			default :
                                if (player == PLAYER_DRACULA)
				{
					if ((n % 8 == 4 || n % 8 == 5))
					{
						int i;
						for (i = 0; i < TRAIL_SIZE; i++) // If Drac returns to location already in trail, don't reset minion count
						{
							if (i == (turn / NUM_PLAYERS) % TRAIL_SIZE) continue; // Don't count what is now a location fallen off the trail
							if (new->hts[player]->trail[i] == location_find_by_abbrev (location)) break;
						}
						if (i == (turn / 5) % TRAIL_SIZE)
						{
							new->vamps[(turn / NUM_PLAYERS) % TRAIL_SIZE] = 0; // Else reset
							new->traps[(turn / NUM_PLAYERS) % TRAIL_SIZE] = 0;
						}

						if (*c == 'T') new->traps[(turn / NUM_PLAYERS) % TRAIL_SIZE]++;
						if (*c == 'V') new->vamps[(turn / NUM_PLAYERS) % TRAIL_SIZE]++;
					}
					else
					{
						if (*c == 'V') new->score -= SCORE_LOSS_VAMPIRE_MATURES; // Vampire matured
					}
				}
				else
				{
					if (*c == 'T') new->hts[player]->health -= LIFE_LOSS_TRAP_ENCOUNTER;
					if (new->hts[player]->health <= 0) score -= SCORE_LOSS_HUNTER_HOSPITAL;
				
					//if (*c == 'V') vampire = 0;
				
					if (*c == 'D')
					{
						new->hts[player]->health -= LIFE_LOSS_DRACULA_ENCOUNTER;
						new->hts[PLAYER_DRACULA]->health -= LIFE_LOSS_HUNTER_ENCOUNTER;
					}
					if (new->hts[player]->health <= 0) score -= SCORE_LOSS_HUNTER_HOSPITAL;
				}
		}

		new->curr_player = ((turn) % 5); // +1 as curr_plyr is yet to make a move
		if (*c == '\0') break;
		n++;
	}
	new->score = score;
	new->turn = turn;
	new->round = (turn)/ 5; // +1 as next turn yet to be had
	
	return new;		
}

void dv_drop (dracula_view *dv)
{
	for (int i = 0; i < 4; i ++) free (dv->hts[i]);
	free (dv);
}

round_t dv_get_round (dracula_view *dv)
{
	return dv->round;
}

int dv_get_score (dracula_view *dv)
{
	return dv->score;
}

int dv_get_health (dracula_view *dv, enum player player)
{
	return dv->hts[player]->health;
}

location_t dv_get_location (dracula_view *dv, enum player player)
{
	return dv->hts[player]->location;
}

void dv_get_player_move (
	dracula_view *dv, enum player player,
	location_t *start, location_t *end)
{
	int index = dv->round % 6 - (dv->turn % 5 <= player); //(dv->round - (((dv->round * 5 - 1)) % 5 < player)) % 6;
	index = (index - 1 < 0) * 6 + (index - 1); // Find second last location
	*start = dv->hts[player]->trail[index];
	*end = dv->hts[player]->location;
	return;
}

void dv_get_locale_info (
	dracula_view *dv, location_t where,
	int *n_traps, int *n_vamps)
{
	// We must start reading most recent entry first to avoid counting an earlier version of same location
	int index = dv->round % 6 - (dv->turn % 5 <= PLAYER_DRACULA); if (index < 0) index += 6; 
	
	for (int c = 0; c < TRAIL_SIZE; c++, index--)
	{
		if (index < 0) index += TRAIL_SIZE;
		if (dv->hts[PLAYER_DRACULA]->trail[index] == where)
		{
			*n_traps = dv->traps[index];
			*n_vamps = dv->vamps[index];
			return;
		}
	}
	*n_traps = 0; *n_vamps = 0;
	return;
}

void dv_get_trail (
	dracula_view *dv, enum player player,
	location_t trail[TRAIL_SIZE])
{
	int j = dv->round % 6 - (dv->turn % 5 <= player); if (j < 0) j += 6;
        for (int c = 0; c < 6; j--, c++) // Sort array into desired order
        {
                if (j < 0) j = 5; // Wrap around array
                trail[c] = dv->hts[player]->trail[j];
        }
        return;
}

location_t *dv_get_dests (
	dracula_view *dv, size_t *n_locations, bool road, bool sea)
{
	return dv_get_dests_player (dv, n_locations, PLAYER_DRACULA, road, 0, sea);
}

location_t *dv_get_dests_player (
	dracula_view *dv, size_t *n_locations, enum player player,
	bool road, bool rail, bool sea)
{
	Map g = map_new ();
        location_t *adjacent = NULL;

        if (dv->hts[player]->location != CITY_UNKNOWN && dv->hts[player]->location != SEA_UNKNOWN)
                adjacent = find_adj (g, dv->hts[player]->location, ((unsigned int) dv->round + player) % 4, (player == PLAYER_DRACULA), road, rail, sea); // Our list to return

        size_t i = 0;
        for (; adjacent[i] != CITY_UNKNOWN; i++);
        *n_locations = i;

        return adjacent;
}
