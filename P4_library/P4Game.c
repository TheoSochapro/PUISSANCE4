#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "P4Game.h"

/*  Initialiser les composants d'une partie de puissance4
entrees :  p1Type,  p2Type, les types des joueurs
sorties :  un pointeur vers une nouvelle partie
*/
p4Game_t *NewP4Game(p4PlayerType_t p1Type, p4PlayerType_t p2Type)
{
    // malloc : reservation de memoire pour le jeu
    p4Game_t *pGame = malloc(sizeof(p4Game_t));

    // Initialisation ...

    // ... de la grille
    for (int c = 0; c < BOARD_COLUMN; c++)
    {
        for (int l = 0; l < BOARD_RAW; l++)
        {
            // mettre chaque case a l'etat 'jeton vide'
            pGame->board[l][c] = p4_PlayerNone;
        }
    }
    // ... des types des joueurs
    pGame->p1Type = p1Type;
    pGame->p2Type = p2Type;

    // ... du premier joueur a commencer
    pGame->currentPlayer = p4_Player1;

    // ... de l'etat du jeu
    pGame->status = p4tr_player1Turn;

    // ... de la graine qui va generer les nombres aleatoires
    srand(time(NULL));

    return pGame;
}

/* Retourne le type du joueur qui est entrain de jouer 
entrees : pGame, pointeur d'une partie
sorties : p4pt_human ou  p4pt_machine ou p4pt_online
*/
p4PlayerType_t p4Game_getCurrentPlayerType(p4Game_t *pGame)
{
    p4PlayerType_t result;

    if (pGame->currentPlayer == p4_Player1)
        result = pGame->p1Type;
    else
        result = pGame->p2Type;

    return result;
}

/* Permet de savoir si la colonne est jouable
entrees : pGame, pointeur d'une partie, 
          column, une colonne 
sorties : vrai si la colonne est jouable, 
          faux sinon
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

/* Permet de retourner l'indice de la ligne jouable d'une colonne
entrees : pGame, pointeur d'une partie
          column, une colonne 
sorties : l'indice de la ligne jouable d'une colonne 
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

/* Permet de savoir si la grille est pleine
entrees : pGame, pointeur d'une partie
sorties : vrai si la grille est pleine, 
         faux sinon
*/
Bool isBoardFull(p4Game_t *pGame)
{
    // ON PARCOURT TOUT LE TABLEAU ...
    for (int c = 0; c < BOARD_COLUMN; c++)
        //...ON VEUT MONTRER QU'IL N'EST PAS PLEIN,
        // SI UNE CASE DE LA DERNIERE LIGNE (LA LIGNE BOARD_RAW-1 ) EST  VIDE.
        if (pGame->board[BOARD_RAW - 1][c] == p4_PlayerNone)
            return False;

    /* LE TABLEAU EST PLEIN SINON. */
    return True;
}

/* Permet de denner le joueur qui a gagne sur la grille actuelle
entrees : pGame, pointeur d'une partie
sorties :
    - p4_Player1    => si le player1 a gagne
    - p4_Player2    => si le player2 a gagne
    - p4_PlayerNone => si aucun n'a gagne
*/
p4Player_t whoWin3(p4Game_t *pGame)
{
    // AU DEBUT PERSONNE N'A GAGNER
    p4Player_t quiGagne = p4_PlayerNone;
    // LES JETONS
    p4Player_t coin1, coin2, coin3, coin4;

    /******************************************** GAGNER LE JEU AVEC ... ********************************************/
    for (int c = 0; c < BOARD_COLUMN; c++)
    {
        for (int l = 0; l < BOARD_RAW; l++)
        {
            /**************************************** ... 4 JETONS EN LIGNE ****************************************/
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
            /********************************* ... 4 JETONS EN COLONNE *********************************/
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
            /******************** ... 4 JETONS EN DIAGONAL BAS-GAUCHE VERS HAUT-DROITE ***************/
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
            /******************** ... 4 JETONS EN DIAGONALE BAS-DROITE VERS HAUT-GAUCHE  ***************/
            if ((l <= (BOARD_RAW - 4)) && ((BOARD_COLUMN - 4) <= c))
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

/* Permet de jouer un coup et définir l'état du jeu apres le coup
entrees : pGame, pointeur d'une partie
          column, la colonne a jouer
sorties :
    - p4tr_player1Turn  => si c'est le tour player1 de jouer
    - p4tr_player2Turn  => si c'est le tour player2 de jouer
    - p4tr_player1Win   => si le tour player1 a gagne
    - p4tr_player2Win   => si le tour player2 a gagne
    - p4tr_draw         => si match nul
*/
p4TurnResult_e p4Game_nextTurn(p4Game_t *pGame, int column)
{
    // SI ON NE PEUT PAS JOUER LA COLONNE DONNER EN PARAMETRE
    if (!isColumnPlayable(pGame, column))
        return pGame->status;

    /******************************************************/
    /**************** JOUER LE COUP DONNER ****************/

    // SAUVEGARDER LE JOUEUR COURANT
    p4Player_t quiJoue = pGame->currentPlayer;

    // DETERMINER LA LIGNE OU LE JETON DOIT ETRE JOUER
    int ligneJouable = getLignePlayable(pGame, column);

    // PLACER LE JETON DU JOUEUR SUR LA GRILLE
    pGame->board[ligneJouable][column] = quiJoue;

    /******************************************************/
    /************ DONNER LE NOUVEL ETAT DU JEU ************/

    // SAUVEGARDER L'ETAT DU JEU
    p4TurnResult_e etatDuJeu = pGame->status;

    switch (whoWin3(pGame)) // DETERMINER QUI A GAGNER SUR LA NOUVELLE GRILLE
    {
    case p4_Player1: // LE JOUEUR 1 A GAGNE
        etatDuJeu = p4tr_player1Win;
        break;

    case p4_Player2: // LE JOUEUR 2 A GAGNE
        etatDuJeu = p4tr_player2Win;
        break;

    case p4_PlayerNone:         // AUCUN JOUEUR N'A GAGNE
        if (isBoardFull(pGame)) // SI LA GRILLE EST PLEINE
        {
            etatDuJeu = p4tr_draw;
            break;
        }

        // DESIGNER QUI VA JOUER : ON SWITCH DE JOUEUR
        if (quiJoue == p4_Player1)
        {
            pGame->currentPlayer = p4_Player2;
            etatDuJeu = p4tr_player2Turn;
        }
        else if (quiJoue == p4_Player2)
        {
            pGame->currentPlayer = p4_Player1;
            etatDuJeu = p4tr_player1Turn;
        }
    }

    pGame->status = etatDuJeu;

    return etatDuJeu;
}

/* Determiner le prochain coup de la machine (AI), avec une simple implementation d'un algorithme de Monte-Carlo
entrees : pGame, pointeur d'une partie
sorties : l'indice de la colonne a jouer
*/
int nextComputerTurn(p4Game_t *pGame)
{
    const int MAX_SCORE_COL = 1000;
    int score_col[BOARD_COLUMN] = {0};           // SCORE DE CHAQUE COLONNE INITIALISER A 0
    p4Player_t AI_Joueur = pGame->currentPlayer; // SAUVEGARDER LE JOUEUR QUI EST LA MACHINE (L'AI)

    for (int col_jouer = 0; col_jouer < BOARD_COLUMN; col_jouer++)
    {
        /*******************************************************************************************/
        /************************************ JOUER LE 1er COUP ************************************/

        // COPIER LE JEU POUR LE 1er COUP
        p4Game_t *game_copy1 = malloc(sizeof(p4Game_t));
        memcpy(game_copy1, pGame, sizeof(p4Game_t));

        // SI ON PEUT JOUER LA COLONNE 'col_jouer'
        if (!isColumnPlayable(game_copy1, col_jouer))
            continue;

        // JOUER LE COUP et CHANGER L'ETAT DU JEU COPIER
        p4Game_nextTurn(game_copy1, col_jouer);
        // SI ON GAGNE AU PREMIER COUP
        if ((game_copy1->status == p4tr_player1Win) || (game_copy1->status == p4tr_player2Win))
        {
            // LE SCORE DE LA COLONNE EST 'MAX_SCORE_COL'
            score_col[col_jouer] = MAX_SCORE_COL;
            continue;
        }
        else
            score_col[col_jouer]++; // DONNER DU POID AU COLONNE JOUABLE

        for (int i = 0; i < MAX_SCORE_COL; i++)
        {
            /*******************************************************************************************/
            /****************************** JOUER UNE PARTIE ENTIEREMENT ******************************/

            // COPIER LE JEU POUR JOUER PARTIE ENTIEREMENT
            p4Game_t *game_copy2 = malloc(sizeof(p4Game_t));
            memcpy(game_copy2, game_copy1, sizeof(p4Game_t));

            int coup_random;
            while ((game_copy2->status == p4tr_player1Turn) || (game_copy2->status == p4tr_player2Turn))
            {
                // GENERER UN COUP ALEATOIRE ENTRE 0 ET 6 (BOARD_COLUMN-1)
                coup_random = rand() % BOARD_COLUMN;

                if (!isColumnPlayable(game_copy1, coup_random))
                    continue;

                // JOUER LE COUP et CHANGER L'ETAT DU JEU COPIER
                p4Game_nextTurn(game_copy2, coup_random);

                // SI QUELQU'UN GAGNE AU N'ieme COUP
                if ((game_copy2->status == p4tr_player1Win) || (game_copy2->status == p4tr_player2Win))
                {
                    // SI ON GAGNE AU N'ieme COUP
                    if (AI_Joueur == game_copy2->currentPlayer)
                    {
                        //  LE SCORE DE LA COLONNE EST INCREMENTER DE 1
                        score_col[col_jouer]++;
                        break;
                    }
                }
            }
            free(game_copy2);
        }
        free(game_copy1);
    }

    /**************************************/
    /***** DETERMINER LE COUP A JOUER *****/

    // DETERMINER LA COLONNE AVEC LE PLUS HAUT SCORE
    int coup_a_jouer = 0;
    int first_score = score_col[0];

    for (int i = 0; i < BOARD_COLUMN; i++)
        if (first_score <= score_col[i])
        {
            coup_a_jouer = i;
            first_score = score_col[i];
        }

    return coup_a_jouer;
}