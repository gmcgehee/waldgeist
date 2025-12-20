#include <iostream>
#include <cstdint>

#ifndef BITBOARD_H
#define BITBOARD_H

using u8 = uint8_t;
using u16 = uint16_t;
using i64 = uint64_t;

struct BoardState {

    // Bitboard of 1s and 0s showing every current position of pieces

    i64 wPawn;
    i64 wKnight;
    i64 wBishop;
    i64 wRook;
    i64 wQueen;
    i64 wKing;

    i64 bPawn;
    i64 bKnight;
    i64 bBishop;
    i64 bRook;
    i64 bQueen;
    i64 bKing;

    u8 castlingRights; // ok... nice
    
    u8 halfMoves; // one byte can hold up to 255, which is easily in a uint8
                                  // but this means 50 plays per side

    u16 fullMoves;        // 16 bits is enough; games can go on for very long, but not 2 ^ 32 - 1 = 4294967295 moves

    bool sideToPlay; // white is 0, black is 1

    u8 enPassantSquare; // number 0-63 will be every move  

};
class GameState
{

    BoardState state;

public:
    GameState(std::string fen = "");

    void setupDefaultBoard();

    BoardState loadFromFen(std::string fen);

    std::string exportFen();

    void printBoardState(BoardState state);
};


#endif