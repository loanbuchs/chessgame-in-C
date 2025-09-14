#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "gamestate.h"
#include "checklogic.h"

int move_white_en_passant(gamestate *game, int src_col)
{
	char totest[8][8];

	memcpy(totest, game->chessboard, sizeof totest);
	if (game->chessboard[game->en_passant_target[0] - 1][src_col] != 'P')
	{
		printf("Invalid move, try again.\n");
		return (0);
	}
	totest[game->en_passant_target[0] - 1][src_col] = ' ';
	totest[game->en_passant_target[0] - 1][game->en_passant_target[1]] = ' ';
	totest[game->en_passant_target[0]][game->en_passant_target[1]] = 'P';
	if (is_check(totest, NULL, '0', game->white_king_position, "white"))
	{
		printf("This movement is not allowed because it would put you in a check situation, try again.\n");
		return(0);
	}
	game->chessboard[game->en_passant_target[0] - 1][src_col] = ' ';
	game->chessboard[game->en_passant_target[0] - 1][game->en_passant_target[1]] = ' ';
	game->chessboard[game->en_passant_target[0]][game->en_passant_target[1]] = 'P';
	return (1);
}

int	move_white_pawn_take(gamestate *game, int src_col, char *move)
{
	int 	row;
	int		dest_col = 0;

	dest_col = move[2] - 'a';
	row = move[3] - '1';
	if(dest_col == game->en_passant_target[1] && row == game->en_passant_target[0])
		return (move_white_en_passant(game, src_col));
	if (game->chessboard[row - 1][src_col] != 'P' || !strchr("prqnbk", game->chessboard[row][dest_col]))
	{
		printf("Invalid move, try again.\n");
		return (0);
	}
	int	movement[4];
	movement[0] = src_col;
	movement[1] = row;
	movement[2] = dest_col;
	movement[3] = row;
	if (is_check(game->chessboard, movement, 'P', game->white_king_position, "white"))
	{
		printf("This movement is not allowed because it would put you in a check situation, try again.\n");
		return(0);
	}
	if (row != 7)
		game->chessboard[row][dest_col] = 'P';
	else
	{
		if (strlen(move) == 6)
		{
			if (!strchr("RNBQ", move[5]))
			{
				printf("Invalid promotion, try again.");
				return (0);
			}
			game->chessboard[row][dest_col] = move[5];
		}
		else
			game->chessboard[row][dest_col] = 'Q';
	}
	game->chessboard[row - 1][src_col] = ' ';
	return (1);
}

int	move_white_pawn(gamestate *game, char *move)
{
	int		row;
	int		col = 0;
	int		start_row;

	if (!strchr("345678x",move[1]))
	{
		printf("Invalid move, try again.\n");
		return (0);
	}
	col = move[0] - 'a';
	if (move[1] != 'x')
		row	= move[1] - '1';
	else
		return (move_white_pawn_take(game, col, move));
	if (game->chessboard[row - 1][col] == 'P' && game->chessboard[row][col] == ' ')
		start_row = row - 1;
	else if (game->chessboard[row - 2][col] == 'P' && row == 3 &&
			game->chessboard[row][col] == ' ' && game->chessboard[row - 1][col] == ' ')
		start_row = row - 2;
	else
	{
		printf("Invalid move, try again.\n");
		return (0);
	}
	int	movement[4];
	movement[0] = col;
	movement[1] = start_row;
	movement[2] = col;
	movement[3] = row;
	if (is_check(game->chessboard, movement, 'P', game->white_king_position, "white"))
	{
		printf("This movement is not allowed because it would put you in a check situation, try again.\n");
		return(0);
	}
	if (row != 7)
		game->chessboard[row][col] = 'P';
	else
		{
		if (strlen(move) == 4)
		{
			if (!strchr("RNBQ", move[3]))
			{
				printf("Invalid promotion, try again.");
				return (0);
			}
			game->chessboard[row][col] = move[3];
		}
		else
			game->chessboard[row][col] = 'Q';
	}
	game->chessboard[start_row][col] = ' ';
	if (row - start_row == 2)
	{
		game->en_passant_target[0] = row - 1;
		game->en_passant_target[1] = col;
		game->change_ept++;
	}
	return (1);
}


int move_black_en_passant(gamestate *game, int src_col)
{
	char totest[8][8];

	memcpy(totest, game->chessboard, sizeof totest);
	if (game->chessboard[game->en_passant_target[0] + 1][src_col] != 'p')
	{
		printf("Invalid move, try again.\n");
		return (0);
	}
	totest[game->en_passant_target[0] + 1][src_col] = ' ';
	totest[game->en_passant_target[0] + 1][game->en_passant_target[1]] = ' ';
	totest[game->en_passant_target[0]][game->en_passant_target[1]] = 'p';
	if (is_check(totest, NULL, '0', game->black_king_position, "black"))
	{
		printf("This movement is not allowed because it would put you in a check situation, try again.\n");
		return(0);
	}
	game->chessboard[game->en_passant_target[0] + 1][src_col] = ' ';
	game->chessboard[game->en_passant_target[0] + 1][game->en_passant_target[1]] = ' ';
	game->chessboard[game->en_passant_target[0]][game->en_passant_target[1]] = 'p';
	return (1);
}

int	move_black_pawn_take(gamestate *game, int src_col, char *move)
{
	int 	row;
	int		dest_col = 0;

	dest_col = move[2] - 'a';
	row = move[3] - '1';
	if(dest_col == game->en_passant_target[1] && row == game->en_passant_target[0])
		return (move_black_en_passant(game, src_col));
	if (game->chessboard[row + 1][src_col] != 'p' || !strchr("PRQNBK", game->chessboard[row][dest_col]))
	{
		printf("Invalid move, try again.\n");
		return (0);
	}
	int	movement[4];
	movement[0] = src_col;
	movement[1] = row;
	movement[2] = dest_col;
	movement[3] = row;
	if (is_check(game->chessboard, movement, 'P', game->black_king_position, "black"))
	{
		printf("This movement is not allowed because it would put you in a check situation, try again.\n");
		return(0);
	}
	game->chessboard[row + 1][src_col] = ' ';
	if (row != 7)
		game->chessboard[row][dest_col] = 'p';
	else
	{
		if (strlen(move) == 6)
		{
			if (!strchr("RNBQ", move[5]))
			{
				printf("Invalid promotion, try again.");
				return (0);
			}
			game->chessboard[row][dest_col] = move[5];
		}
		else
			game->chessboard[row][dest_col] = 'Q';
	}
	return (1);
}

int	move_black_pawn(gamestate *game, char *move)
{
	int		row;
	int		col = 0;
	int		start_row;

	if (!strchr("123456x",move[1]))
	{
		printf("Invalid move, try again.\n");
		return (0);
	}
	col = move[0] - 'a';
	if (move[1] != 'x')
		row	= move[1] - '1';
	else
		return (move_black_pawn_take(game, col, move));
	if (game->chessboard[row + 1][col] == 'p' && game->chessboard[row][col] == ' ')
		start_row = row + 1;
	else if (game->chessboard[row + 2][col] == 'p' && row == 4 &&
			game->chessboard[row][col] == ' ' && game->chessboard[row + 1][col] == ' ')
		start_row = row + 2;
	else
	{
		printf("Invalid move, try again.\n");
		return (0);
	}
	int	movement[4];
	movement[0] = col;
	movement[1] = start_row;
	movement[2] = col;
	movement[3] = row;
	if (is_check(game->chessboard, movement, 'P', game->black_king_position, "black"))
	{
		printf("This movement is not allowed because it would put you in a check situation, try again.\n");
		return(0);
	}
	if (row != 7)
		game->chessboard[row][col] = 'p';
	else
		{
		if (strlen(move) == 4)
		{
			if (!strchr("RNBQ", move[3]))
			{
				printf("Invalid promotion, try again.");
				return (0);
			}
			game->chessboard[row][col] = move[3];
		}
		else
			game->chessboard[row][col] = 'Q';
	}
	game->chessboard[start_row][col] = ' ';
	if (start_row - row == 2)
	{
		game->en_passant_target[0] = row + 1;
		game->en_passant_target[1] = col;
		game->change_ept++;
	}
	return (1);
}

int move_parse_with_desambiguation(gamestate *game, char *move, char *turn, int	parsed_result[4])
{
	const int	src_col = 0;
	const int	src_row = 1;
	const int	dest_col = 2;
	const int	dest_row = 3;

	if (strchr("abcdefgh", move[1]))
		parsed_result[src_col] = move[1] - 'a';
	else if (strchr("12345678", move[1]))
		parsed_result[src_row] = move[1] - '1';
	else
	{
		printf("Invalid move, try again.\n");
		return (0);
	}
	if (parsed_result[src_row] >= 0)
	{
		parsed_result[src_col] = 0;
		while ((!(game->chessboard[parsed_result[src_row]][parsed_result[src_col]] == move[0] && !strcmp(turn, "white")) &&
		!(game->chessboard[parsed_result[src_row]][parsed_result[src_col]] == move[0] + 32 && !strcmp(turn, "black"))) && parsed_result[src_col] < 8)
			parsed_result[src_col]++;
	}
	else if (parsed_result[src_col] >= 0)
	{
		parsed_result[src_row] = 0;
		while ((!(game->chessboard[parsed_result[src_row]][parsed_result[src_col]] == move[0] && !strcmp(turn, "white")) &&
		!(game->chessboard[parsed_result[src_row]][parsed_result[src_col]] == move[0] + 32 && !strcmp(turn, "black"))) && parsed_result[src_row] < 8)			
			parsed_result[src_row]++;
	}
	if (!(game->chessboard[parsed_result[src_row]][parsed_result[src_col]] == move[0] && !strcmp(turn, "white")) &&
		!(game->chessboard[parsed_result[src_row]][parsed_result[src_col]] == move[0] + 32 && !strcmp(turn, "black")))
	{
		printf("Invalid move, try again.");
		return (0);
	}
	if (strlen(move) == 4)
	{
		parsed_result[dest_col] = move[2] - 'a';
		parsed_result[dest_row] = move[3] - '1';
	}
	else
	{
		parsed_result[dest_col] = move[3] - 'a';
		parsed_result[dest_row] = move[4] - '1';
	}
	return (1);
}

int move_parse(gamestate *game, char *move, char* turn, int	parsed_result[4])
{
	const int	dest_col = 2;
	const int	dest_row = 3;

	if ((strlen(move) == 4 && move[1] != 'x') || strlen(move) == 5)
		return (move_parse_with_desambiguation(game, move, turn, parsed_result));
	else
	{
		if (strlen(move) == 4)
		{
			parsed_result[dest_col] = move[2] - 'a';
			parsed_result[dest_row] = move[3] - '1';
		}
		else
		{
			parsed_result[dest_col] = move[1] - 'a';
			parsed_result[dest_row] = move[2] - '1';
		}
	}
	if (parsed_result[dest_col] >= 0 && parsed_result[dest_col] < 8 &&
		parsed_result[dest_row] >= 0 && parsed_result[dest_row] < 8)
		return (1);
	printf("Invalid move, try again.\nparse");
	return (0);
}

int	move_rook_get_src(char chessboard[8][8], char *move, char *turn, int parsed_move[4])
{
	const int	src_col = 0;
	const int	src_row = 1;
	const int	dest_col = 2;
	const int	dest_row = 3;
	const int	step[4][2] = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};

	for (int i = 0; i < 4; i++)
	{
		parsed_move[src_row] = parsed_move[dest_row] + step[i][0];
		parsed_move[src_col] = parsed_move[dest_col] + step[i][1];
		while (parsed_move[src_col] + step[i][1] <= 7 && parsed_move[src_col] + step[i][1] >= 0 && parsed_move[src_row] + step[i][0] <= 7 &&
			parsed_move[src_row] + step[i][0] >= 0 && chessboard[parsed_move[src_row]][parsed_move[src_col]] == ' ')
		{
				parsed_move[src_row] += step[i][0];
				parsed_move[src_col] += step[i][1];
		}
	
		if ((!strcmp(turn, "white") && chessboard[parsed_move[src_row]][parsed_move[src_col]] == move[0]) ||
		(!strcmp(turn, "black") && chessboard[parsed_move[src_row]][parsed_move[src_col]] == move[0] + 32))
			return (1);
	}
	return (0);
}

int	move_rook_possible(gamestate *game, int parsed_move[4])
{	
	const int	src_col = 0;
	const int	src_row = 1;
	const int	dest_col = 2;
	const int	dest_row = 3;

	if (parsed_move[dest_col] == parsed_move[src_col])
	{
		if (parsed_move[src_row] < parsed_move[dest_row])
		{
			for (int i = parsed_move[src_row] + 1; i < parsed_move[dest_row]; i++)
				if (game->chessboard[i][parsed_move[dest_col]] != ' ')
					return (0);
		}
		else
			for (int i = parsed_move[src_row] - 1; i > parsed_move[dest_row]; i--)
				if (game->chessboard[i][parsed_move[dest_col]] != ' ')
					return (0);
	}
	else if (parsed_move[dest_row] == parsed_move[src_row])
	{
		if (parsed_move[src_col] < parsed_move[dest_col])
		{
			for (int i = parsed_move[src_col] + 1; i < parsed_move[dest_col]; i++)
				if (game->chessboard[parsed_move[dest_row]][i] != ' ')
					return (0);
		}
		else
			for (int i = parsed_move[src_col] - 1; i > parsed_move[dest_col]; i--)
				if (game->chessboard[parsed_move[dest_row]][i] != ' ')
					return (0);
	}
	else
		return (0);
	return (1);
}

int	move_rook(gamestate *game, char *move, char *turn)
{
	int			parsed_move[4] = {-1, -1, -1, -1};
	const int	src_col = 0;
	const int	src_row = 1;
	const int	dest_col = 2;
	const int	dest_row = 3;

	if (!move_parse(game, move, turn, parsed_move))
		return (0);
	if (parsed_move[src_row] == -1)
	{
		if (!move_rook_get_src(game->chessboard, move, turn, parsed_move))
		{
			printf("Invalid move, try again.");
			return (0);
		}
	}
	if (!move_rook_possible(game, parsed_move))
	{
			printf("Invalid move, try again.\n");
			return (0);
	}
	if ((!strcmp(turn, "black") && game->chessboard[parsed_move[dest_row]][parsed_move[dest_col]] > 'a') ||
		((!strcmp(turn, "white") && game->chessboard[parsed_move[dest_row]][parsed_move[dest_col]] < 'a') &&
		game->chessboard[parsed_move[dest_row]][parsed_move[dest_col]] != ' '))
	{
		printf("Invalid move, this case is already occupied by one of your pieces, try again.\n");
		return (0);
	}
	if (is_check(game->chessboard, parsed_move, 'R', game->king_position, turn))
	{
		printf("This movement is not allowed because it would put you in a check situation, try again.\n");
		return(0);
	}
	game->chessboard[parsed_move[dest_row]][parsed_move[dest_col]] = 'R';
	game->chessboard[parsed_move[src_row]][parsed_move[src_col]] = ' ';
	if (!strcmp(turn, "black"))
	{
		game->chessboard[parsed_move[dest_row]][parsed_move[dest_col]] += 32;
		if (parsed_move[src_col] == 0 && parsed_move[src_row] == 7)
			game->black_can_castle_qside = 0;
		if (parsed_move[src_col] == 7 && parsed_move[src_row] == 7)
			game->black_can_castle_kside = 0;
	}
	else
	{
		if (parsed_move[src_col] == 0 && parsed_move[src_row] == 0)
			game->white_can_castle_qside = 0;
		if (parsed_move[src_col] == 7 && parsed_move[src_row] == 0)
			game->white_can_castle_kside = 0;
	}
	return (1);
}

int	move_bishop_get_src(char chessboard[8][8], char *move, char *turn, int parsed_move[4])
{
	const int	src_col = 0;
	const int	src_row = 1;
	const int	dest_col = 2;
	const int	dest_row = 3;
	int			found = 0;

	for (int i = 1; parsed_move[dest_row] + i < 8 && parsed_move[dest_col] + i < 8; i++)
	{
		if (chessboard[parsed_move[dest_row] + i][parsed_move[dest_col] + i] != ' ')
		{
			parsed_move[src_row] = parsed_move[dest_row] + i;
			parsed_move[src_col] = parsed_move[dest_col] + i;
			found = 1;
			break;
		}
	}
	if (found)
		if ((chessboard[parsed_move[src_row]][parsed_move[src_col]] == move[0] && !strcmp(turn, "white")) ||
			(chessboard[parsed_move[src_row]][parsed_move[src_col]] == move[0] + 32 && !strcmp(turn, "black")))
			return (1);
	found = 0;
	for (int i = 1; parsed_move[dest_row] + i < 8 && parsed_move[dest_col] - i >= 0; i++)
	{
		if (chessboard[parsed_move[dest_row] + i][parsed_move[dest_col] - i] != ' ')
		{
			parsed_move[src_row] = parsed_move[dest_row] + i;
			parsed_move[src_col] = parsed_move[dest_col] - i;
			found = 1;
			break;
		}
	}
	if (found)
		if ((chessboard[parsed_move[src_row]][parsed_move[src_col]] == move[0] && !strcmp(turn, "white")) ||
			(chessboard[parsed_move[src_row]][parsed_move[src_col]] == move[0] + 32 && !strcmp(turn, "black")))
			return (1);
	found = 0;
	for (int i = 1; parsed_move[dest_row] - i >= 0 && parsed_move[dest_col] + i < 8; i++)
	{
		if (chessboard[parsed_move[dest_row] - i][parsed_move[dest_col] + i] != ' ')
		{
			parsed_move[src_row] = parsed_move[dest_row] - i;
			parsed_move[src_col] = parsed_move[dest_col] + i;
			found = 1;
			break;
		}
	}
	if (found)
		if ((chessboard[parsed_move[src_row]][parsed_move[src_col]] == move[0] && !strcmp(turn, "white")) ||
			(chessboard[parsed_move[src_row]][parsed_move[src_col]] == move[0] + 32 && !strcmp(turn, "black")))
			return (1);
	found = 0;
	for (int i = 1; parsed_move[dest_row] - i >= 0 && parsed_move[dest_col] - i >= 0; i++)
	{
		if (chessboard[parsed_move[dest_row] - i][parsed_move[dest_col] - i] != ' ')
		{
			parsed_move[src_row] = parsed_move[dest_row] - i;
			parsed_move[src_col] = parsed_move[dest_col] - i;
			found = 1;
			break;
		}
	}
	if (found)
		if ((chessboard[parsed_move[src_row]][parsed_move[src_col]] == move[0] && !strcmp(turn, "white")) ||
			(chessboard[parsed_move[src_row]][parsed_move[src_col]] == move[0] + 32 && !strcmp(turn, "black")))
			return (1);
	return (0);
}

int	move_bishop_possible(gamestate *game, int parsed_move[4])
{
	const int	src_col = 0;
	const int	src_row = 1;
	const int	dest_col = 2;
	const int	dest_row = 3;

	if (abs(parsed_move[src_col] - parsed_move[dest_col]) != abs(parsed_move[src_row] - parsed_move[dest_row]) ||
		parsed_move[src_col] == parsed_move[dest_col])
		return (0);
	int	row_step = (parsed_move[dest_row] > parsed_move[src_row]) ? 1 : -1;
	int	col_step = (parsed_move[dest_col] > parsed_move[src_col]) ? 1 : -1;
	int	row = parsed_move[src_row] + row_step;
	int	col = parsed_move[src_col] + col_step;
	while (row != parsed_move[dest_row] && col != parsed_move[dest_col])
	{
		if (game->chessboard[row][col] != ' ')
			return 0;
		row += row_step;
		col += col_step;
	}
	return (1);
}

int	move_bishop(gamestate *game, char *move, char *turn)
{
	int	parsed_move[4] = {-1, -1, -1, -1};
	const int src_col = 0;
	const int src_row = 1;
	const int dest_col = 2;
	const int dest_row = 3;

	if (!move_parse(game, move, turn, parsed_move))
		return (0);
	if (parsed_move[src_row] == -1)
	{
		if (!move_bishop_get_src(game->chessboard, move, turn, parsed_move))
		{
			printf("Invalid move, try again.1");
			return (0);
		}
	}
	else if (!move_bishop_possible(game, parsed_move))
	{
		printf("Invalid move, try again.2");
		return (0);
	}
	if ((!strcmp(turn, "black") && game->chessboard[parsed_move[dest_row]][parsed_move[dest_col]] > 'a') ||
		((!strcmp(turn, "white") && game->chessboard[parsed_move[dest_row]][parsed_move[dest_col]] < 'a') &&
		game->chessboard[parsed_move[dest_row]][parsed_move[dest_col]] != ' '))
	{
		printf("Invalid move, this case is already occupied by one of your pieces, try again.\n");
		return (0);
	}
	if (is_check(game->chessboard, parsed_move, 'P', game->king_position, turn))
	{
		printf("This movement is not allowed because it would put you in a check situation, try again.\n");
		return(0);
	}
	game->chessboard[parsed_move[dest_row]][parsed_move[dest_col]] = move[0];
	game->chessboard[parsed_move[src_row]][parsed_move[src_col]] = ' ';
	if (!strcmp(turn, "black"))
		game->chessboard[parsed_move[dest_row]][parsed_move[dest_col]] += 32;
	return (1);
}

int	move_queen(gamestate *game, char *move, char *turn)
{
	int			parsed_move[4] = {-1, -1, -1, -1};
	const int	src_col = 0;
	const int	src_row = 1;
	const int	dest_col = 2;
	const int	dest_row = 3;

	if (!move_parse(game, move, turn, parsed_move))
		return (0);
	if (!move_rook_get_src(game->chessboard, move, turn, parsed_move))
		if (!move_bishop_get_src(game->chessboard, move, turn, parsed_move))
		{
			printf("Invalid move, try again.");
			return (0);
		}
	if (parsed_move[src_col] == parsed_move[dest_col] || parsed_move[src_row] == parsed_move[dest_row])
	{
		if (!move_rook_possible(game, parsed_move))
		{
			printf("Invalid move, try again.");
			return (0);
		}
	}
	else if (!move_bishop_possible(game, parsed_move))
	{
		printf("Invalid move, try again.");
		return (0);
	}
	if ((!strcmp(turn, "black") && game->chessboard[parsed_move[dest_row]][parsed_move[dest_col]] > 'a') ||
		((!strcmp(turn, "white") && game->chessboard[parsed_move[dest_row]][parsed_move[dest_col]] < 'a') &&
		game->chessboard[parsed_move[dest_row]][parsed_move[dest_col]] != ' '))
	{
		printf("Invalid move, this case is already occupied by one of your pieces, try again.\n");
		return (0);
	}
	if (is_check(game->chessboard, parsed_move, 'Q', game->king_position, turn))
	{
		printf("This movement is not allowed because it would put you in a check situation, try again.\n");
		return(0);
	}
	game->chessboard[parsed_move[dest_row]][parsed_move[dest_col]] = move[0];
	game->chessboard[parsed_move[src_row]][parsed_move[src_col]] = ' ';
	if (!strcmp(turn, "black"))
		game->chessboard[parsed_move[dest_row]][parsed_move[dest_col]] += 32;
	return (1);
}

int	move_knight_get_src(char chessboard[8][8], char *move, char *turn, int *parsed_move)
{
	const int	src_col = 0;
	const int	src_row = 1;
	const int	dest_col = 2;
	const int	dest_row = 3;
	int			moves[8][2] = {{2, 1}, {1, 2}, {-1, 2}, {-2, 1},{-2, -1}, {-1, -2}, {1, -2}, {2, -1}};

	for (int i = 0; i < 8; i++)
	{
		if (parsed_move[dest_row] + moves[i][0] < 0 || parsed_move[dest_row] + moves[i][0] > 7 ||
			parsed_move[dest_col] + moves[i][1] < 0 || parsed_move[dest_col] + moves[i][1] > 7)
			continue;
		if ((chessboard[parsed_move[dest_row] + moves[i][0]][parsed_move[dest_col] + moves[i][1]] == move[0] && !strcmp(turn, "white")) ||
			(chessboard[parsed_move[dest_row] + moves[i][0]][parsed_move[dest_col] + moves[i][1]] == move[0] + 32 && !strcmp(turn, "black")))
		{
			parsed_move[src_row] = parsed_move[dest_row] + moves[i][0];
			parsed_move[src_col] = parsed_move[dest_col] + moves[i][1];
			return (1);
		}
	}
	return (0);
}

int move_knight(gamestate *game, char *move, char *turn)
{
	int			parsed_move[4] = {-1, -1, -1, -1};
	const int	src_col = 0;
	const int	src_row = 1;
	const int	dest_col = 2;
	const int	dest_row = 3;

	if (!move_parse(game, move, turn, parsed_move))
		return (0);
	if (parsed_move[src_row] == -1)
	{
		if (!move_knight_get_src(game->chessboard, move, turn, parsed_move))
		{
			printf("Invalid move, try again.1\n");
			return (0);
		}
	}
	if ((!strcmp(turn, "black") && game->chessboard[parsed_move[dest_row]][parsed_move[dest_col]] > 'a') ||
		((!strcmp(turn, "white") && game->chessboard[parsed_move[dest_row]][parsed_move[dest_col]] < 'a') &&
		game->chessboard[parsed_move[dest_row]][parsed_move[dest_col]] != ' '))
	{
		printf("Invalid move, this case is already occupied by one of your pieces, try again.\n");
		return (0);
	}
	if (abs(parsed_move[src_col] - parsed_move[dest_col]) + abs(parsed_move[src_row] - parsed_move[dest_row]) != 3 ||
		abs(abs(parsed_move[src_col] - parsed_move[dest_col]) - abs(parsed_move[src_row] - parsed_move[dest_row])) != 1 )
	{
		printf("Invalid move, try again.2");
		return (0);
	}
	if (is_check(game->chessboard, parsed_move, 'N', game->king_position, turn))
	{
		printf("This movement is not allowed because it would put you in a check situation, try again.\n");
		return(0);
	}
	game->chessboard[parsed_move[dest_row]][parsed_move[dest_col]] = move[0];
	game->chessboard[parsed_move[src_row]][parsed_move[src_col]] = ' ';
	if (!strcmp(turn, "black"))
		game->chessboard[parsed_move[dest_row]][parsed_move[dest_col]] += 32;
	return (1);
}

int	move_king_get_src(char chessboard[8][8], char *move, char *turn, int *parsed_move)
{
	const int	src_col = 0;
	const int	src_row = 1;
	const int	dest_col = 2;
	const int	dest_row = 3;
	int			moves[8][2] = {{1, 1}, {1, -1}, {1, 0}, {-1, 1},{-1, -1}, {-1, 0}, {0, 1}, {0, -1}};

	for (int i = 0; i < 8; i++)
	{
		if (parsed_move[dest_row] + moves[i][0] < 0 || parsed_move[dest_row] + moves[i][0] > 7 ||
			parsed_move[dest_col] + moves[i][1] < 0 || parsed_move[dest_col] + moves[i][1] > 7)
			continue;
		if ((chessboard[parsed_move[dest_row] + moves[i][0]][parsed_move[dest_col] + moves[i][1]] == move[0] && !strcmp(turn, "white")) ||
			(chessboard[parsed_move[dest_row] + moves[i][0]][parsed_move[dest_col] + moves[i][1]] == move[0] + 32 && !strcmp(turn, "black")))
		{
			parsed_move[src_row] = parsed_move[dest_row] + moves[i][0];
			parsed_move[src_col] = parsed_move[dest_col] + moves[i][1];
			return (1);
		}
	}
	return (0);
}

int move_king(gamestate *game, char *move, char *turn)
{
	int			parsed_move[4] = {-1, -1, -1, -1};
	const int	src_col = 0;
	const int	src_row = 1;
	const int	dest_col = 2;
	const int	dest_row = 3;

	if (!move_parse(game, move, turn, parsed_move))
		return (0);
	if (parsed_move[src_row] == -1)
	{
		if (!move_king_get_src(game->chessboard, move, turn, parsed_move))
		{
			printf("Invalid move, try again.\n");
			return (0);
		}
	}
	if ((!strcmp(turn, "black") && game->chessboard[parsed_move[dest_row]][parsed_move[dest_col]] > 'a') ||
		((!strcmp(turn, "white") && game->chessboard[parsed_move[dest_row]][parsed_move[dest_col]] < 'a') &&
		game->chessboard[parsed_move[dest_row]][parsed_move[dest_col]] != ' '))
	{
		printf("Invalid move, this case is already occupied by one of your pieces, try again.\n");
		return (0);
	}
	if (is_check(game->chessboard, parsed_move, 'K', game->king_position, turn))
	{
		printf("This movement is not allowed because it would put you in a check situation, try again.\n");
		return(0);
	}
	game->chessboard[parsed_move[dest_row]][parsed_move[dest_col]] = move[0];
	game->chessboard[parsed_move[src_row]][parsed_move[src_col]] = ' ';
	game->king_position[0] = parsed_move[dest_row];
	game->king_position[1] = parsed_move[dest_col];
	if (!strcmp(turn, "black"))
	{
		game->chessboard[parsed_move[dest_row]][parsed_move[dest_col]] += 32;
		game->black_can_castle_kside = 0;
		game->black_can_castle_qside = 0;
	}
	else
	{
		game->white_can_castle_kside = 0;
		game->white_can_castle_qside = 0;
	}
	return (1);
}

int	move_castle(gamestate *game, char *move, char *turn)
{
	if (!strcmp(turn, "white"))
	{
		if (!strcmp(move, "O-O") && game->white_can_castle_kside && game->chessboard[0][6] == ' ' && game->chessboard[0][5] == ' ' &&
			!case_attacked_by_black(game->chessboard, 0, 7) && !case_attacked_by_black(game->chessboard, 0, 4) &&
			!case_attacked_by_black(game->chessboard, 0, 6) && !case_attacked_by_black(game->chessboard, 0, 5))
		{
			game->chessboard[0][5] = 'R';
			game->chessboard[0][7] = ' ';
			game->chessboard[0][6] = 'K';
			game->chessboard[0][4] = ' ';
			return (1);
		}
		if (!strcmp(move, "O-O-O") && game->white_can_castle_qside && game->chessboard[0][1] == ' ' && game->chessboard[0][2] == ' ' && game->chessboard[0][3] == ' ' &&
			!case_attacked_by_black(game->chessboard, 0, 0) && !case_attacked_by_black(game->chessboard, 0, 1) &&
			!case_attacked_by_black(game->chessboard, 0, 2) && !case_attacked_by_black(game->chessboard, 0, 3) && !case_attacked_by_black(game->chessboard, 0, 4))
		{
			game->chessboard[0][2] = 'K';
			game->chessboard[0][0] = ' ';
			game->chessboard[0][3] = 'R';
			game->chessboard[0][4] = ' ';
			return (1);
		}
	}
	else
	{
		if (!strcmp(move, "O-O") && game->black_can_castle_kside && game->chessboard[7][6] == ' ' && game->chessboard[7][5] == ' ' &&
			!case_attacked_by_white(game->chessboard, 7, 7) && !case_attacked_by_white(game->chessboard, 7, 4) &&
			!case_attacked_by_white(game->chessboard, 7, 6) && !case_attacked_by_white(game->chessboard, 7, 5))
		{
			game->chessboard[7][5] = 'r';
			game->chessboard[7][7] = ' ';
			game->chessboard[7][6] = 'k';
			game->chessboard[7][4] = ' ';
			return (1);
		}
		if (!strcmp(move, "O-O-O") && game->black_can_castle_qside && game->chessboard[7][1] == ' ' && game->chessboard[7][2] == ' ' && game->chessboard[7][3] == ' ' &&
			!case_attacked_by_white(game->chessboard, 7, 0) && !case_attacked_by_white(game->chessboard, 7, 1) &&
			!case_attacked_by_white(game->chessboard, 7, 2) && !case_attacked_by_white(game->chessboard, 7, 3) && !case_attacked_by_white(game->chessboard, 7, 4))
		{
			game->chessboard[7][2] = 'k';
			game->chessboard[7][0] = ' ';
			game->chessboard[7][3] = 'r';
			game->chessboard[7][4] = ' ';
			return (1);
		}
	}
	printf("Invalid castle, try again.");
	return (0);
}

int	move_correct(gamestate *game, char *move, char *turn)
{
	if (!move[0])
		return (0);
	if (strchr("abcdefgh",move[0]))
	{
		if (strcmp(turn, "white") == 0)
			return (move_white_pawn(game, move));
		else
			return move_black_pawn(game, move);
	}
	if (move[0] == 'R')
		return (move_rook(game, move, turn));
	if (move[0] == 'Q')
		return (move_queen(game, move, turn));
	if (move[0] == 'N')
		return (move_knight(game, move, turn));
	if (move[0] == 'B')
		return (move_bishop(game, move, turn));
	if (move[0] == 'K')
		return (move_king(game, move, turn));
	if (move[0] == 'O')
		return (move_castle(game, move, turn));
	printf("Invalid input, try again.\n");
	return (0);
}
