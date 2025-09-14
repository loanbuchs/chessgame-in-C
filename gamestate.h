#ifndef GAMESTATE_H
#define GAMESTATE_H

typedef struct s_gamestate
{
	char	chessboard[8][8];
	int		en_passant_target[2];
	int		white_king_position[2];
	int		black_king_position[2];
	int		king_position[2];
	int		white_can_castle_kside;
	int		white_can_castle_qside;
	int		black_can_castle_kside;
	int		black_can_castle_qside;
	int		halfmove_clock;
	int		fullmove_number;
	int		end;
	int		change_ept;
} gamestate;


#endif