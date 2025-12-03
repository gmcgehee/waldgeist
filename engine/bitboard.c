#include <stdbool.h>
#include

typedef unsigned long long u64;

typedef struct {

    // Bitboard of 1s and 0s showing every current position of pieces
    u64 wPawn;
    u64 wKnight;
    u64 wBishop;
    u64 wRook;
    u64 wQueen;
    u64 wKing;

    u64 bPawn;
    u64 bKnight;
    u64 bBishop;
    u64 bRook;
    u64 bQueen;
    u64 bKing;

    unsigned char castlingRights;
    unsigned int reversibleMoves; // ints are 32-bit, so we are NOT concerned about overflow with a long game, especially because this maxes out at 50
                                  // but this means 50 plays per side
    unsigned int totalMoves;        

    bool sideToPlay; // white is 0, black is 1

    unsigned int enPassantSquare; // number 0-63 will be every move

} boardState;

// we can set up default values for just white and lshift by 8 * rows (5) for pawns and 8 * rows (7) for other pieces

u64 emptyBoard = 0x0000000000000000ULL;

/*
0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0
*/

u64 wPawnDefault = 0xFF00ULL;

/*
0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0
1 1 1 1 1 1 1 1
0 0 0 0 0 0 0 0
*/


u64 wKnightDefault;

/*
0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0
0 1 0 0 0 0 1 0
*/

u64 wBishopDefault;

/*
0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0
0 0 1 0 0 1 0 0
*/

u64 wRookDefault;

/*
0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0
1 0 0 0 0 0 0 1
*/

u64 wQueenDefault;

/*
0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0
0 0 0 1 0 0 0 0
*/

u64 wKingDefault;

/*
0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0
0 0 0 0 1 0 0 0
*/

void setupDefaultBoard() {

}