
////////////////////////////////////////////////////////////////////////
// COMP2521 19t0 ... the Fury of Dracula
// game_view.c: GameView ADT implementation
//
// 2014-07-01	v1.0	Team Dracula <cs2521@cse.unsw.edu.au>
// 2017-12-01	v1.1	Team Dracula <cs2521@cse.unsw.edu.au>
// 2018-12-31	v2.0	Team Dracula <cs2521@cse.unsw.edu.au>

#include <assert.h>
#include <err.h>
#include <stdbool.h>
#include <stdlib.h>
#include <sysexits.h>

#include "game.h"
#include "game_view.h"
// #include "map.h" ... if you decide to use the Map ADT
// use enums to specify which character struct to access
typedef struct characters
{
	int health;
	location_t location;
} characters;

typedef characters *Characters;
typedef struct game_view
{
	int turn; // current turn, starting at 0
	int score;
	//use enums to determine current player
	int currPlayer;
	Characters ch[5];
	
} game_view;

game_view *gv_new (char *past_plays, player_message messages[])
{
	/// @todo REPLACE THIS WITH YOUR OWN IMPLEMENTATION
	game_view *new = malloc (sizeof *new);
	if (new == NULL) err (EX_OSERR, "couldn't allocate GameView");

	return new;
}

void gv_drop (game_view *gv)
{
	int player = 0;
	while (i < 5) {
		free(gv->ch[player]);
		player++;
	}
	free (gv);
}

round_t gv_get_round (game_view *gv)
{
	return gv.turn;
}

enum player gv_get_player (game_view *gv)
{
	return gv.currPlayer;
}

int gv_get_score (game_view *gv)
{
	return gv.score;
}

int gv_get_health (game_view *gv, enum player player)
{
	return gv->ch[player].health;
}

location_t gv_get_location (game_view *gv, enum player player)
{
	return gv->ch[player].location;
}
//TO DO: needs editing!
void gv_get_history (
	game_view *gv, enum player player,
	location_t trail[TRAIL_SIZE])
{
	int moveCount = 0;
	while (i < TRAIL_SIZE) {//generate move data in gameview
		if (MOVE_NOT_PLAYED) {
			trail[moveCount] = -1;
		}
		else {	
			trail[moveCount] = MOVE;
		}
		moveCount++;
	}
}
//TO DO: needs editing!
location_t *gv_get_connections (
	game_view *gv, size_t *n_locations,
	location_t from, enum player player, round_t round,
	bool road, bool rail, bool sea)
{
	location_t arr[&n_locations];
	if (player == PLAYER_DRACULA) {
		if (road == true) {
			// find all road connections for dracula and add to arr[]
		}
		if (sea == true) {
			// find all sea connections for dracula and add to arr[]
		}
		return arr;
	}
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


