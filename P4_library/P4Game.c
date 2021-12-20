#include <stdlib.h>
#include <string.h>
#include "P4Game.h"
//comtest

/*  on veux réserver la place d'un type game et initialiser les composants
entree : p4PlayerType_t p1Type , p4PlayerType_t p2Type
sortie : p4Game_t*  game
*/
p4Game_t *NewP4Game(p4PlayerType_t p1Type, p4PlayerType_t p2Type)
{
    // malloc : reservation de memoire pour le jeu
    p4Game_t *pGame = malloc(sizeof(p4Game_t));

    // initialisation

    // board init des cellules a 0
    for (int c = 0; c < BOARD_COLUMN; c++)
    {
        for (int l = 0; l < BOARD_RAW; l++)
        {
            // mettre à 0 dans chaque cellule
            pGame->board[l][c] = p4_PlayerNone;
        }
    }
    // set p1 and p2 type
    pGame->p1Type = p1Type;
    pGame->p2Type = p2Type;

    // player to first play
    pGame->currentPlayer = p4_Player1;

    // status
    pGame->status = 0; // todo etat du jeu

    return pGame;
}

/* retourne le type du joueur qui est entrain de jouer 
entree : p4Game_t *pGame
sortie : p4PlayerType_t  p4pt_human ou  p4pt_machine ou p4pt_online
*/
p4PlayerType_t p4Game_getCurrentPlayerType(p4Game_t *pGame)
{
    //creation d'une variable resultat
    p4PlayerType_t result;

    if (pGame->currentPlayer == p4_Player1)
        result = pGame->p1Type;
    else
        result = pGame->p2Type;

    return result;
}

/*
Permet de retourner True si la colonne est jouable

*/
Bool isColumnPlayable(p4Game_t *pGame, int column)
{
    if (!((0 <= column) && (column < BOARD_COLUMN)))
        return False;

    for (int l = 0; l < BOARD_RAW; l++)
        if (pGame->board[l][column] == p4_PlayerNone)
            return True;

    return False;
}

/*
Permet de retourner l'indice jouable d'une colonne
*/
int getLignePlayable(p4Game_t *pGame, int column)
{
    int indice_res = 0;
    for (int l = 0; l < BOARD_RAW; l++)
    {
        if (pGame->board[l][column] == p4_PlayerNone)
            return indice_res;
        indice_res++;
    }
    return indice_res;
}

/* Permet de savoir si le tableau est plein
*/
Bool isBoardFull(p4Game_t *pGame)
{
    // On parcour tous le tableau ...
    for (int c = 0; c < BOARD_COLUMN; c++)
        /* ...ON VEUT MONTRER QU'IL EST FAUX */
        // ... si une cellule de la derniere ligne (la ligne BOARD_RAW-1 ) est  vide.
        if (pGame->board[BOARD_RAW - 1][c] == p4_PlayerNone)
            return False;

    /* LE RESULTAT EST VRAI SINON. */
    return True;
}

/* Permet de savoir qui a gagner
sortie :
    - p4tr_player1Turn  => si c'est le tour player1 de jouer
    - p4tr_player2Turn  => si c'est le tour player2 de jouer
    - p4tr_player1Win   => si le tour player1 a gagne
    - p4tr_player2Win   => si le tour player2 a gagne
    - p4tr_draw         => si o
*/
p4Player_t whoWin3(p4Game_t *pGame)
{
    // etat du jeu
    p4Player_t quiGagne = p4_PlayerNone;
    // les jetons
    p4Player_t coin1, coin2, coin3, coin4;
    /* WIN WITH ... */

    for (int c = 0; c < BOARD_COLUMN; c++)
    {
        for (int l = 0; l < BOARD_RAW; l++)
        {
            //* ... 4 COIN LINE
            if (c <= (BOARD_COLUMN - 4))
            {
                coin1 = pGame->board[l][c];
                coin2 = pGame->board[l][c + 1];
                coin3 = pGame->board[l][c + 2];
                coin4 = pGame->board[l][c + 3];
                if ((p4_PlayerNone != coin1) && (coin1 == coin2) && (coin2 == coin3) && (coin3 == coin4))
                {
                    quiGagne = coin1;
                    break;
                }
            }
            //* ... 4 COIN COLUMN
            if (l <= (BOARD_RAW - 4))
            {
                coin1 = pGame->board[l][c];
                coin2 = pGame->board[l + 1][c];
                coin3 = pGame->board[l + 2][c];
                coin4 = pGame->board[l + 3][c];
                if ((p4_PlayerNone != coin1) && (coin1 == coin2) && (coin2 == coin3) && (coin3 == coin4))
                {
                    quiGagne = coin1;
                    break;
                }
            }
            //* ... 4 COIN DIAGONAL BOTTOM-LEFT TO TOP-RIGHT
            if ((l <= (BOARD_RAW - 4)) && (c <= (BOARD_COLUMN - 4)))
            {
                coin1 = pGame->board[l][c];
                coin2 = pGame->board[l + 1][c + 1];
                coin3 = pGame->board[l + 2][c + 2];
                coin4 = pGame->board[l + 3][c + 3];
                if ((p4_PlayerNone != coin1) && (coin1 == coin2) && (coin2 == coin3) && (coin3 == coin4))
                {
                    quiGagne = coin1;
                    break;
                }
            }
            //* ... 4 COIN DIAGONAL TOP-LEFT TO BOTTOM-RIGHT
            if ((l <= (BOARD_RAW - 4)) && (4 <= c))
            {
                coin1 = pGame->board[l][c];
                coin2 = pGame->board[l + 1][c - 1];
                coin3 = pGame->board[l + 2][c - 2];
                coin4 = pGame->board[l + 3][c - 3];
                if ((p4_PlayerNone != coin1) && (coin1 == coin2) && (coin2 == coin3) && (coin3 == coin4))
                {
                    quiGagne = coin1;
                    break;
                }
            }
        }
    }
    return quiGagne;
}