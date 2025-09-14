#ifndef MOVE_H
# define MOVE_H
#include "gamestate.h"

int	move_correct(gamestate *game, char *move, char *turn);
int	move_rook_get_src(char chessboard[8][8], char *move, char *turn, int parsed_move[4]);
int	move_bishop_get_src(char chessboard[8][8], char *move, char *turn, int parsed_move[4]);
int	move_knight_get_src(char chessboard[8][8], char *move, char *turn, int *parsed_move);
int	move_king_get_src(char chessboard[8][8], char *move, char *turn, int *parsed_move);

#endif