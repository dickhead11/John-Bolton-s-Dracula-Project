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
	int turn;
	
	int curr_player;
	Characters hts[5];
} hunter_view;

hunter_view *hv_new (char *past_plays, player_message messages[])
{
	hunter_view *new = malloc (sizeof *new);
	if (new == NULL) err (EX_OSERR, "couldn't allocate HunterView");

	for (int k = 0; k < 5; k++)
	{
		new->hts[k] = malloc (sizeof (characters));
		if (new->hts[k] == NULL) err (EX_OSERR, "coulnd't allocate characters field");
	}

	// Following put into hunter_view
	int turn = 0;
	new->curr_player = PLAYER_LORD_GODALMING;
	int score = GAME_START_SCORE;
	int vampire = 0; // True/false for immature vampire
	
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
                                if (player == PLAYER_DRACULA) score -= SCORE_LOSS_DRACULA_TURN; // Happens after dracula finishes his turn

                                if ((turn / NUM_PLAYERS) % 13 == 0 &&
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
					if (location[0] == 'S' && location[1] == '?')
					{
						new->hts[player]->health -= LIFE_LOSS_SEA;
						new->hts[player]->location = SEA_UNKNOWN;
					}
					else if (location[0] ==  'C' && location[1] == '?') new->hts[player]->location = CITY_UNKNOWN;
					else if (location[0] == 'T' && location [1] == 'P') new->hts[player]->location = location_find_by_abbrev ("CD");
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
					else new->hts[player]->location = location_find_by_abbrev (location);
					
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
                                        if (!(n % 8 == 4 || n % 8 == 5))
                                                if (*c == 'V') new->score -= SCORE_LOSS_VAMPIRE_MATURES; // Vampire matured
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

void hv_drop (hunter_view *hv)
{
	for (int i = 0; i < 4; i ++) free (hv->hts[i]);
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
	int j = hv->round % 6 - (hv->turn % 5 <= player);//(hv->round - (((hv->round * 5 - 1)) % 5 < player)) % 6;
	if (j < 0) j+= 6;
	//for (j = 0; j < 6; j++) if (hv->hts[player]->trail[j] == hv->hts[player]->location) break;
	for (int c = 0; c < 6; j--, c++) // Sort array into desired order
	{
		if (j < 0) j = 5; // Wrap around array
		trail[c] = hv->hts[player]->trail[j];
	}
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
	Map g = map_new ();
	location_t *adjacent = NULL;
	if (hv->hts[player]->location != CITY_UNKNOWN && hv->hts[player]->location != SEA_UNKNOWN)
		adjacent = find_adj (g, hv->hts[player]->location, ((unsigned int) hv->round + player) % 4, (player == PLAYER_DRACULA), road, rail, sea); // Our list to return
	
	size_t i = 0;
	for (; adjacent[i] != CITY_UNKNOWN; i++);
	*n_locations = i;
	
	return adjacent;
}
