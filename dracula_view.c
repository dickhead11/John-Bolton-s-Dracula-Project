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
// #include "map.h" ... if you decide to use the Map ADT
typedef struct drac_Char
{
	int health;
	location_t location;
} drac_Char;
typedef drac_Char *Drac_Char;
typedef struct dracula_view {
	round_t turn; // current turn, starting at 0
	int score;
	//use enums to determine current player
	int currPlayer;
	Drac_Char ch[5];
} dracula_view;
//TO DO: needs editing!
dracula_view *dv_new (char *past_plays, player_message messages[])
{
	/// @todo REPLACE THIS WITH YOUR OWN IMPLEMENTATION
	dracula_view *new = malloc (sizeof *new);
	if (new == NULL) err (EX_OSERR, "couldn't allocate DraculaView");

	return new;
}

void dv_drop (dracula_view *dv)
{
	int i = 0;
	while (i < 5) {
		free(dv->ch[i]);
		i++;
	}
	free (dv);
}

round_t dv_get_round (dracula_view *dv)
{

	return dv.turn;
}

int dv_get_score (dracula_view *dv)
{
	return dv.score;
}

int dv_get_health (dracula_view *dv, enum player player)
{
	return dv->ch[player].health;
}

location_t dv_get_location (dracula_view *dv, enum player player)
{
	return dv->ch[player]->location;
}
//TO DO: needs editing!
void dv_get_player_move (
	dracula_view *dv, enum player player,
	location_t *start, location_t *end)
{
	//generate move data in dracview
	return;
}
//TO DO: needs editing!
void dv_get_locale_info (
	dracula_view *dv, location_t where,
	int *n_traps, int *n_vamps)
{
	if (where != validLocation) {
		&n_traps = 0;
		&n_vamps = 0;//figure out shit
	}

	return;
}
//TO DO: needs editing!
void dv_get_trail (
	dracula_view *dv, enum player player,
	location_t trail[TRAIL_SIZE])
{
	int i = 0;
	while (i < TRAIL_SIZE) {//generate move data in dracview
		if (MOVE_NOT_PLAYED) {
			trail[i] = -1;
		}
		else {	
			trail[i] = MOVE;
		}
		i++;
	}
}

//TO DO: needs editing!
location_t *dv_get_dests (
	dracula_view *dv, size_t *n_locations, bool road, bool sea)
{
	location_t arr[&n_locations];
	if (road == true) {
		// find all road connections for dracula and add to arr[]
	}
	if (sea == true) {
		// find all sea connections for dracula and add to arr[]
	}
	return arr;
}
//TO DO: needs editing!
location_t *dv_get_dests_player (
	dracula_view *dv, size_t *n_locations, enum player player,
	bool road, bool rail, bool sea)
{
	if (player == PLAYER_DRACULA) return dv_get_dests(dv, n_locations, road, sea);
	location_t arr[&n_locations];
	if (road == true) {
		// find all road connections for player and add to arr[]
	}
	if (sea == true) {
		// find all sea connections for player and add to arr[]
	}
	if (rail == true) {
		// find all rail connections for player and add to arr[]
	}
	return arr;
}
