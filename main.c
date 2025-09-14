#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "gamestate.h"
#include "move.h"
#include "checklogic.h"

void	set_chessboard(char board[8][8])
{
	char	*black_back_rank = "rnbqkbnr";
	char	*white_back_rank = "RNBQKBNR";

	for (int i = 0; i < 8; ++i)
	{
		board[0][i] = white_back_rank[i];
		board[1][i] = 'P';
		board[6][i] = 'p';
		board[7][i] = black_back_rank[i];
	}
	for (int row = 2; row <= 5; ++row)
		for (int col = 0; col < 8; ++col)
			board[row][col] = ' ';
}

void 	set_gamestate(gamestate *game)
{
	game->white_can_castle_kside = 0;
	game->white_can_castle_qside = 0;
	game->black_can_castle_kside = 0;
	game->black_can_castle_qside = 0;
	game->halfmove_clock = 1;
	game->fullmove_number = 0;
	game->end = 1;
	game->change_ept = 1;

	game->change_ept -= 1;
	game->end -= 1;
	game->white_can_castle_kside += 1;
	game->white_can_castle_qside += 1;
	game->black_can_castle_kside += 1;
	game->black_can_castle_qside += 1;
	game->halfmove_clock -= 1;
	game->fullmove_number += 1;
	game->black_king_position[0] = 7;
	game->black_king_position[1] = 4;
	game->white_king_position[0] = 0;
	game->white_king_position[1] = 4;
}

void	print_chessboard(char chessboard[8][8])		//add abcdefg and 12345678 on the sides of the board to make it more readable and playable
{
	for (int row = 7; row >= 0; --row)
	{
		for (int col = 0; col < 8; ++col)
		{
			write(1, &chessboard[row][col], 1);
			write(1, "|", 1);
		}
		write(1, "\n", 1);
	}
}

void	white_turn(gamestate *game)
{
	char	move[10];

	for (int i = 0; i < 10; i++)
		move[i] = 0;
	if (is_check(game->chessboard, NULL, '0', game->white_king_position, "white"))
	{
		if (is_checkmate_for_white(game))
		{
			printf("Blacks win!");
			game->end++;
			game->end++;
			return;
		}
		printf("Check!\n");
	}
	memcpy(game->king_position, game->white_king_position, sizeof(game->king_position));
	print_chessboard(game->chessboard);
	printf("It's white's turn to play.\n");
	printf("%d", game->en_passant_target[0]);
	printf("%d", game->en_passant_target[1]);
	while (!move_correct(game, move, "white"))
		scanf("%s", move);
	memcpy(game->white_king_position, game->king_position, sizeof(game->white_king_position));
	if (game->change_ept)
		game->change_ept--;
	else
	{
		game->en_passant_target[0] = -1;
		game->en_passant_target[1] = -1;
	}
	return;
}

void	black_turn(gamestate *game)
{
	char	move[10];

	for (int i = 0; i < 10; i++)
		move[i] = 0;
	if (is_check(game->chessboard, NULL, '0', game->black_king_position, "black"))
	{
		if (is_checkmate_for_black(game))
		{
			printf("Whites win!");
			game->end++;
			return;
		}
		printf("Check!\n");
	}
	memcpy(game->king_position, game->black_king_position, sizeof(game->king_position));
	print_chessboard(game->chessboard);
	printf("%d", game->en_passant_target[0]);
	printf("%d", game->en_passant_target[1]);
	printf("It's black's turn to play.\n");
	while (!move_correct(game, move, "black"))
		scanf("%s", move);
	memcpy(game->black_king_position, game->king_position, sizeof(game->black_king_position));
	if (game->change_ept)
		game->change_ept--;
	else
	{
		game->en_passant_target[0] = -1;
		game->en_passant_target[1] = -1;
	}
	return;
}

int	partie(void)
{
	gamestate	*game = malloc(sizeof(gamestate));

	set_gamestate(game);
	set_chessboard(game->chessboard);
	while (!game->end)
	{
		white_turn(game);
		if(!game->end)
			black_turn(game);
	}
	free(game);
	return (0);
}

int	main(void)
{
	partie();
	return (0);
}
