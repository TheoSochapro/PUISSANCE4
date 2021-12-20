#ifndef P4GAME_H
#define P4GAME_H

#define BOARD_RAW       6
#define BOARD_COLUMN    7

typedef enum {
    False,
    True
} Bool;

typedef enum {
    p4_PlayerNone = 0,
    p4_Player1 = 1,
    p4_Player2 = 2
} p4Player_t;

typedef enum {
    p4tr_player1Turn,
    p4tr_player2Turn,
    p4tr_player1Win,
    p4tr_player2Win,
    p4tr_draw

}p4TurnResult_e;

typedef enum {
    p4pt_human,
    p4pt_machine,
    p4pt_online
}p4PlayerType_t;

typedef struct P4_game {

    int board[BOARD_RAW][BOARD_COLUMN];
    p4Player_t currentPlayer;
    int status;

    p4PlayerType_t p1Type, p2Type;

} p4Game_t;


p4Game_t *NewP4Game(p4PlayerType_t p1Type, p4PlayerType_t p2Type);
p4TurnResult_e p4Game_nextTurn(p4Game_t *pGame, int column);
p4PlayerType_t p4Game_getCurrentPlayerType(p4Game_t *pGame);


Bool isColumnPlayable(p4Game_t *pGame, int column); 
int getLignePlayable(p4Game_t *pGame, int column); 
Bool isBoardFull(p4Game_t *pGame)
#endif // P4GAME_H
