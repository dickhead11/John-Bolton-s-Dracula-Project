////////////////////////////////////////////////////////////////////////
// COMP2521 19t0 ... the Fury of Dracula
// game_view.c: GameView ADT implementation
//
// 2014-07-01	v1.0	Team Dracula <cs2521@cse.unsw.edu.au>
// 2017-12-01	v1.1	Team Dracula <cs2521@cse.unsw.edu.au>
// 2018-12-31	v2.0	Team Dracula <cs2521@cse.unsw.edu.au>

#include <assert.
#include <err.h>
#include <stdbool.h>
#include <stdlib.h>
#include <sysexits.h>

#include "game.h"
#include "game_view.h"
// #include "map.h" ... if you decide to use the Map ADT

typedef struct characters
{
	int G_health;
	int G_score;
	char *G_location;
	int S_health;
	int S_score;
	char *S_location;
	int H_health;
	int H_score;
	char *H_location;
	int M_health;
	int M_score;
	char *M_location;
	int D_health;
	int D_score;
	char * D_location;
} characters;

typedef characters *Characters;

typedef struct game_view
{
	int turn; // current turn, starting at 0
	int score;
	char *curr_player;
	Characters ch;
	
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
	/// @todo REPLACE THIS WITH YOUR OWN IMPLEMENTATION
	free (gv);
}

round_t gv_get_round (game_view *gv)
{
	/// @todo REPLACE THIS WITH YOUR OWN IMPLEMENTATION
	return 0;
}

enum player gv_get_player (game_view *gv)
{
	/// @todo REPLACE THIS WITH YOUR OWN IMPLEMENTATION
	return 0;
}

int gv_get_score (game_view *gv)
{
	/// @todo REPLACE THIS WITH YOUR OWN IMPLEMENTATION
	return 0;
}

int gv_get_health (game_view *gv, enum player player)
{
	/// @todo REPLACE THIS WITH YOUR OWN IMPLEMENTATION
	return 0;
}

location_t gv_get_location (game_view *gv, enum player player)
{
	/// @todo REPLACE THIS WITH YOUR OWN IMPLEMENTATION
	return 0;
}

void gv_get_history (
	game_view *gv, enum player player,
	location_t trail[TRAIL_SIZE])
{
	/// @todo REPLACE THIS WITH YOUR OWN IMPLEMENTATION
}

location_t *gv_get_connections (
	game_view *gv, size_t *n_locations,
	location_t from, enum player player, round_t round,
	bool road, bool rail, bool sea)
{
	/// @todo REPLACE THIS WITH YOUR OWN IMPLEMENTATION
	*n_locations = 0;
	return NULL;
}
