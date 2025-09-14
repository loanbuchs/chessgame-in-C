#ifndef CHECKLOGIC_H
# define CHECKLOGIC_H
#include "gamestate.h"

int	case_attacked_by_black(char chessboard[8][8], int target_row, int target_col);
int	case_attacked_by_white(char chessboard[8][8], int target_row, int target_col);
int	is_check(char chessboard[8][8], int move[4], char piece, int kingpos[2], char *turn);
int	is_checkmate_for_white(gamestate *game);
int	is_checkmate_for_black(gamestate *game);

#endif