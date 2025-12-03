
#include <stdbool.h>

#ifndef BITBOARD_H
#define BITBOARD_H

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

void setupDefaultBoard();


#endif