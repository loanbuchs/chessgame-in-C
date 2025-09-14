#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "gamestate.h"
#include "move.h"

int	case_attacked_by_black(char chessboard[8][8], int target_row, int target_col)
{
	int			movement[4] = {-1, -1, -1, -1};
	const int	dest_col = 2;
	const int	dest_row = 3;

	movement[dest_col] = target_col;
	movement[dest_row] = target_row;
	printf("%d", target_row);
	printf("%d\n",target_col);
	if (move_knight_get_src(chessboard, "N", "black", movement)){
		printf("knight");
		return (1);}
	if (move_bishop_get_src(chessboard, "B", "black", movement)){
		printf("bishop");
		return (1);}
	if (move_rook_get_src(chessboard, "R", "black", movement)){
		printf("rook");
		return (1);}
	if (move_bishop_get_src(chessboard, "Q", "black", movement)){
		printf("queenb");
		return (1);}
	if (move_rook_get_src(chessboard, "Q", "black", movement)){
		printf("queenr");
		return (1);}
	if (target_row + 1 < 8 && target_col + 1 < 8 && target_col - 1 >= 0 && (chessboard[target_row + 1][target_col - 1] == 'p' ||
		chessboard[target_row + 1][target_col + 1] == 'p')){
		printf("pawn");
		return (1);}
	if (move_king_get_src(chessboard, "K", "black", movement)){
		printf("king");
		return (1);}
	return (0);
}

int	case_attacked_by_white(char chessboard[8][8], int target_row, int target_col)
{
	int			movement[4] = {-1, -1, -1, -1};
	const int	dest_col = 2;
	const int	dest_row = 3;

	movement[dest_col] = target_col;
	movement[dest_row] = target_row;
	if (move_knight_get_src(chessboard, "N", "white", movement)){
		printf("knighta");
		return (1);}
	if (move_bishop_get_src(chessboard, "B", "white", movement)){
		printf("bishopa");
		return (1);}
	if (move_rook_get_src(chessboard, "R", "white", movement)){
		printf("rooka");
		return (1);}
	if (move_bishop_get_src(chessboard, "Q", "white", movement)){
		printf("queenba");
		return (1);}
	if (move_rook_get_src(chessboard, "Q", "white", movement)){
		printf("queenra");
		return (1);}
	if (target_row - 1 >= 0 && target_col + 1 < 8 && target_col - 1 >= 0 && (chessboard[target_row + 1][target_col - 1] == 'P' ||
		chessboard[target_row + 1][target_col + 1] == 'P')){
		printf("pawna");
		return (1);}
	if (move_king_get_src(chessboard, "K", "white", movement)){
		printf("kinga");
		return (1);}
	return (0);
}

int	is_check(char original_chessboard[8][8], int move[4], char piece, int kingpos[2], char *turn)
{
	const int	src_col = 0;
	const int	src_row = 1;
	const int	dest_col = 2;
	const int	dest_row = 3;
	char		chessboard[8][8];

	memcpy(chessboard, original_chessboard, sizeof(chessboard));
	if (move)
	{
		chessboard[move[dest_row]][move[dest_col]] = piece;
		chessboard[move[src_row]][move[src_col]] = ' ';
		if (!strcmp(turn, "black"))
			chessboard[move[dest_row]][move[dest_col]] += 32;
	}
	if ((!strcmp(turn, "white") && case_attacked_by_black(chessboard, kingpos[0], kingpos[1])) ||
		(!strcmp(turn, "black") && case_attacked_by_white(chessboard, kingpos[0], kingpos[1])))
		return (1);
	return (0);
}

int white_pawn_can_block(gamestate *game, char original_chessboard[8][8], int movement[4])
{
	const int	src_col = 0;
	const int	src_row = 1;
	const int	dest_col = 2;
	const int	dest_row = 3;
	char		chessboard[8][8];

	memcpy(chessboard, original_chessboard, sizeof(chessboard));
	if (movement[dest_row] - 1 >= 0 && chessboard[movement[dest_row] - 1][movement[dest_col]] == 'P')
	{
		movement[src_col] = movement[dest_col];
		movement[src_row] = movement[dest_row] - 1;
		if (!is_check(chessboard, movement, 'P', game->white_king_position, "white"))
			return (1);
	}
	if (movement[dest_row] - 1 >= 0 && movement[dest_col] + 1 < 8 && chessboard[movement[dest_row] - 1][movement[dest_col ] + 1] == 'P' && strchr("prbnqk", chessboard[movement[dest_row]][movement[dest_col]]))
	{
		movement[src_col] = movement[dest_col] + 1;
		movement[src_row] = movement[dest_row] - 1;
		if (!is_check(chessboard, movement, 'P', game->white_king_position, "white"))
			return (1);
	}
	if (movement[dest_row] - 1 >= 0 && movement[dest_col] - 1 >= 0 && chessboard[movement[dest_row] - 1][movement[dest_col] - 1] == 'P'  && strchr("prbnqk", chessboard[movement[dest_row]][movement[dest_col]]))
	{
		movement[src_col] = movement[dest_col] - 1;
		movement[src_row] = movement[dest_row] - 1;
		if (!is_check(chessboard, movement, 'P', game->white_king_position, "white"))
			return (1);
	}
	return (0);
}

int	is_checkmate_for_white(gamestate *game)
{
	int			movement[4] = {-1, -1, -1, -1};
	const int	src_col = 0;
	const int	src_row = 1;
	const int	dest_col = 2;
	const int	dest_row = 3;
	char		chessboard[8][8];
	memcpy(chessboard, game->chessboard, sizeof(chessboard));

	for (int row = 0; row < 8; row++)
	{
		for (int col = 0; col < 8; col++)
		{
			int change = 0;

			movement[dest_col] = col;
			movement[dest_row] = row;
			if (move_knight_get_src(chessboard, "N", "white", movement))
			{
				if (!is_check(chessboard, movement, 'N', game->white_king_position, "white"))
					return (0);
				chessboard[movement[src_row]][movement[src_col]] = 'T';
				change = 1;
			}
			if (move_bishop_get_src(chessboard, "B", "white", movement))
			{
				if (!is_check(chessboard, movement, 'B', game->white_king_position, "white"))
					return (0);
				chessboard[movement[src_row]][movement[src_col]] = 'T';
				change = 1;
			}
			if (move_rook_get_src(chessboard, "R", "white", movement))
			{
				if (!is_check(chessboard, movement, 'R', game->white_king_position, "white"))
					return (0);
				chessboard[movement[src_row]][movement[src_col]] = 'T';
				change = 1;
			}
			if (move_bishop_get_src(chessboard, "Q", "white", movement))
			{
				if (!is_check(chessboard, movement, 'Q', game->white_king_position, "white"))
					return (0);
				chessboard[movement[src_row]][movement[src_col]] = 'T';
				change = 1;
			}
			if (move_rook_get_src(chessboard, "Q", "white", movement))
			{
				if (!is_check(chessboard, movement, 'Q', game->white_king_position, "white"))
					return (0);
				chessboard[movement[src_row]][movement[src_col]] = 'T';
				change = 1;
			}
			if (white_pawn_can_block(game, chessboard, movement))
				return (0);
			if (change)
			{
				col--;
				change = 0;
			}
			else		//remplacer les boucle par un memcopy
			{
				for (int i = 0; i < 8; i++)
					for (int j = 0; j < 8; j++)
						chessboard[i][j] = game->chessboard[i][j];	
			}
		}
	}
	return (1);
}

int black_pawn_can_block(gamestate *game, char original_chessboard[8][8], int movement[4])
{
	const int	src_col = 0;
	const int	src_row = 1;
	const int	dest_col = 2;
	const int	dest_row = 3;
	char		chessboard[8][8];

	memcpy(chessboard, original_chessboard, sizeof(chessboard));
	if (movement[dest_row] + 1 < 8 && chessboard[movement[dest_row] + 1][movement[dest_col]] == 'p')
	{
		movement[src_col] = movement[dest_col];
		movement[src_row] = movement[dest_row] + 1;
		if (!is_check(chessboard, movement, 'P', game->black_king_position, "black"))
			return (1);
	}
	if (movement[dest_row] + 1 < 8 && movement[dest_col] + 1 < 8 && chessboard[movement[dest_row] + 1][movement[dest_col] + 1] == 'p' && strchr("PRNBQK", chessboard[movement[dest_row]][movement[dest_col]]))
	{
		movement[src_col] = movement[dest_col] + 1;
		movement[src_row] = movement[dest_row] + 1;
		if (!is_check(chessboard, movement, 'P', game->black_king_position, "black"))
			return (1);
	}
	if (movement[dest_row] + 1 < 8 && movement[dest_col] - 1 >= 0 && chessboard[movement[dest_row] + 1][movement[dest_col] - 1] == 'p'  && strchr("PRNBQK", chessboard[movement[dest_row]][movement[dest_col]]))
	{
		movement[src_col] = movement[dest_col] - 1;
		movement[src_row] = movement[dest_row] + 1;
		if (!is_check(chessboard, movement, 'P', game->black_king_position, "black"))
			return (1);
	}
	return (0);
}

int	is_checkmate_for_black(gamestate *game)
{
	int			movement[4] = {-1, -1, -1, -1};
	const int	src_col = 0;
	const int	src_row = 1;
	const int	dest_col = 2;
	const int	dest_row = 3;
	char		chessboard[8][8];

	memcpy(chessboard, game->chessboard, sizeof(chessboard));
	for (int row = 0; row < 8; row++)
	{
		for (int col = 0; col < 8; col++)
		{
			int change = 0;

			movement[dest_col] = col;
			movement[dest_row] = row;
			if (move_knight_get_src(chessboard, "N", "black", movement))
			{
				if (!is_check(chessboard, movement, 'N', game->black_king_position, "black"))
					return (0);
				chessboard[movement[src_row]][movement[src_col]] = 't';
				change = 1;
			}
			if (move_bishop_get_src(chessboard, "B", "black", movement))
			{
				if (!is_check(chessboard, movement, 'B', game->black_king_position, "black"))
					return (0);
				chessboard[movement[src_row]][movement[src_col]] = 't';
				change = 1;
			}
			if (move_rook_get_src(chessboard, "R", "black", movement))
			{
				if (!is_check(chessboard, movement, 'R', game->black_king_position, "black"))
					return (0);
				chessboard[movement[src_row]][movement[src_col]] = 't';
				change = 1;
			}
			if (move_bishop_get_src(chessboard, "Q", "black", movement))
			{
				if (!is_check(chessboard, movement, 'Q', game->black_king_position, "black"))
					return (0);
				chessboard[movement[src_row]][movement[src_col]] = 't';
				change = 1;
			}
			if (move_rook_get_src(chessboard, "Q", "black", movement))
			{
				if (!is_check(chessboard, movement, 'Q', game->black_king_position, "black"))
					return (0);
				chessboard[movement[src_row]][movement[src_col]] = 't';
				change = 1;
			}
			if (black_pawn_can_block(game, chessboard, movement))
				return (0);
			if (change)
			{
				col--;
				change = 0;
			}
			else
			{
				for (int i = 0; i < 8; i++)
					for (int j = 0; j < 8; j++)
						chessboard[i][j] = game->chessboard[i][j];	
			}
		}
	}
	return (1);
}
