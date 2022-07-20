/*============================LIBRARIES && MACROS =================================*/
#include <assert.h> /*assert*/
#include <time.h>	/*time*/

#include "knights_tour.h"
#include "bitarray.h"

#define ON 1
#define OFF 0
#define LEGAL 1
#define BRUTE 0
#define VISITED 1
#define BOARD_LEN 8
#define NOT_LEGAL 0
#define FIVE_MIN 300
#define WARNSDORFF 1
#define BOARD_SIZE 64
#define NUM_OF_MOVES 8
#define INVALID_PLACE (100)
#define GET_INDEX(x, y) ((BOARD_LEN * (y)) + (x))
#define IS_VISITED(visited_places, pos) (BitArrayGetVal(visited_places, pos))
#define IS_MOVE_LEGAL(x, y) (0 < (x + 1) && BOARD_LEN > (x) && 0 < (y + 1) && BOARD_LEN > (y))
/*=========================== FUNCTION DECLARATION ================================*/
static int RunKnightsTour(int curr_pos, unsigned char *path, int moves_lut[][NUM_OF_MOVES], bitarray_t visited_places, time_t start_time, int is_warnsdorff);
static return_status_t MutliKnightsTour(unsigned char x_pos, unsigned char y_pos, unsigned char *path, int is_warnsdorff);
static void SortByNumOfPossibleMoves(int curr_pos, int moves_lut[BOARD_SIZE][NUM_OF_MOVES], bitarray_t visited_places);
static void ChangeOnVisit(bitarray_t *visited_places, unsigned char *path, int pos, int set_bits, int bit);
static void InsertionSortSpecial(int curr_pos, int *counter, int moves_lut[BOARD_SIZE][NUM_OF_MOVES]);
static void InitMoveLut(int moves_lut[BOARD_SIZE][NUM_OF_MOVES], int is_initialized);

/*=========================== FUNCTION DEFINITION =================================*/
return_status_t KnightsTour(unsigned char x_pos, unsigned char y_pos, unsigned char *path)
{
	return (MutliKnightsTour(x_pos, y_pos, path, BRUTE));
}

return_status_t WarnsdorffKnightsTour(unsigned char x_pos, unsigned char y_pos, unsigned char *path)
{
	return (MutliKnightsTour(x_pos, y_pos, path, WARNSDORFF));
}

static return_status_t MutliKnightsTour(unsigned char x_pos, unsigned char y_pos, unsigned char *path, int is_warnsdorff)
{
	static int moves_lut[BOARD_SIZE][NUM_OF_MOVES];
	bitarray_t visited_places = 0;
	assert(NULL != path);
	assert(IS_MOVE_LEGAL(x_pos, y_pos));
	InitMoveLut(moves_lut, is_warnsdorff);

	ChangeOnVisit(&visited_places, path, GET_INDEX(x_pos, y_pos), 0, ON);

	return (RunKnightsTour(GET_INDEX(x_pos, y_pos), path, moves_lut, visited_places, time(0), is_warnsdorff));
}

static int RunKnightsTour(int curr_pos, unsigned char *path, int moves_lut[][NUM_OF_MOVES], bitarray_t visited_places, time_t start_time, int is_warnsdorff)
{
	int move = 0;
	return_status_t status = FALSE;
	int count = BitArrayCountOnNoLoop(visited_places); /*IN PLACE NO NEED FOR PASSING IN THE REC*/
	int next_pos = 0;
	if ((time(0) - start_time) > FIVE_MIN)
	{
		return (TIMEOUT);
	}
	if (BOARD_SIZE == count)
	{
		return (TRUE);
	}

	if (is_warnsdorff)
	{
		SortByNumOfPossibleMoves(curr_pos, moves_lut, visited_places);
	}

	for (move = 0; move < NUM_OF_MOVES; ++move)
	{
		next_pos = moves_lut[curr_pos][move];
		if (INVALID_PLACE != next_pos && !IS_VISITED(visited_places, next_pos))
		{
			ChangeOnVisit(&visited_places, path, next_pos, count, ON);
			status = RunKnightsTour(next_pos, path, moves_lut, visited_places, start_time, is_warnsdorff);
			if (FALSE == status)
			{
				ChangeOnVisit(&visited_places, path, next_pos, count, OFF);
			}
			else
			{
				return (status);
			}
		}
	}
	return (FALSE);
}

static void InitMoveLut(int moves_lut[BOARD_SIZE][NUM_OF_MOVES], int is_initialized)
{
	static const int xMove[NUM_OF_MOVES] = {2, 1, -1, -2, -2, -1, 1, 2};
	static const int yMove[NUM_OF_MOVES] = {1, 2, 2, 1, -1, -2, -2, -1};
	static int did_initialize = 0;
	int x_move = 0;
	int y_move = 0;
	if (!is_initialized || !did_initialize)
	{
		int y = 1, x = 1, move = 0;
		for (y = 0; y < 8; ++y)
		{
			for (x = 0; x < 8; ++x)
			{
				for (move = 0; move < 8; ++move)
				{
					x_move = x + xMove[move];
					y_move = y + yMove[move];
					moves_lut[GET_INDEX(x, y)][move] = IS_MOVE_LEGAL(x_move, y_move) ? GET_INDEX(x_move, y_move) : INVALID_PLACE;
				}
			}
		}
		did_initialize = 1;
	}
}

static void SortByNumOfPossibleMoves(int curr_pos, int moves_lut[BOARD_SIZE][NUM_OF_MOVES], bitarray_t visited_places)
{
	int i = 0, j = 0;
	int counter[NUM_OF_MOVES] = {0};
	for (i = 0; i < NUM_OF_MOVES; ++i)
	{
		if (INVALID_PLACE == moves_lut[curr_pos][i] || IS_VISITED(visited_places, moves_lut[curr_pos][i]))
		{
			counter[i] = INVALID_PLACE;
			continue;
		}
		for (j = 0; j < NUM_OF_MOVES; ++j)
		{
			char next_next = moves_lut[moves_lut[curr_pos][i]][j];
			if (INVALID_PLACE != next_next && !IS_VISITED(visited_places, next_next))
			{
				++counter[i];
			}
		}
	}
	InsertionSortSpecial(curr_pos, counter, moves_lut);
}

static void InsertionSortSpecial(int curr_pos, int *counter, int moves_lut[BOARD_SIZE][NUM_OF_MOVES])
{
	int i = 0, j = 0, tmp1 = 0, tmp2 = 0;
	for (i = 1; i < NUM_OF_MOVES; ++i)
	{
		tmp1 = counter[i];
		tmp2 = moves_lut[curr_pos][i];
		j = i - 1;
		while (counter[j] > tmp1 && 0 <= j)
		{
			counter[j + 1] = counter[j];
			moves_lut[curr_pos][j + 1] = moves_lut[curr_pos][j];
			j--;
		}
		counter[j + 1] = tmp1;
		moves_lut[curr_pos][j + 1] = tmp2;
	}
}

static void ChangeOnVisit(bitarray_t *visited_places, unsigned char *path, int pos, int set_bits, int bit)
{
	if (bit)
	{
		*visited_places = BitArraySetOn(*visited_places, pos);
		path[set_bits] = pos;
	}
	else
	{
		*visited_places = BitArraySetOff(*visited_places, pos);
		path[set_bits] = 0;
	}
}