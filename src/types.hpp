
#ifndef TYPES_HPP
#define TYPES_HPP

typedef uint8_t u8;
typedef uint64_t Bitboard;
typedef uint8_t Square;
typedef uint16_t Move;

enum class Side : bool {
    WHITE = false, BLACK = true
};

enum class BoardSquares : Square {
    a1, b1, c1, d1, e1, f1, g1, h1,
    a2, b2, c2, d2, e2, f2, g2, h2,
    a3, b3, c3, d3, e3, f3, g3, h3,
    a4, b4, c4, d4, e4, f4, g4, h4,
    a5, b5, c5, d5, e5, f5, g5, h5,
    a6, b6, c6, d6, e6, f6, g6, h6,
    a7, b7, c7, d7, e7, f7, g7, h7,
    a8, b8, c8, d8, e8, f8, g8, h8
};

struct BoardState
{

    // Bitboard of 1s and 0s showing every current position of pieces

    Bitboard wPawn = 0ULL;
    Bitboard wKnight = 0ULL;
    Bitboard wBishop = 0ULL;
    Bitboard wRook = 0ULL;
    Bitboard wQueen = 0ULL;
    Bitboard wKing = 0ULL;

    Bitboard bPawn = 0ULL;
    Bitboard bKnight = 0ULL;
    Bitboard bBishop = 0ULL;
    Bitboard bRook = 0ULL;
    Bitboard bQueen = 0ULL;
    Bitboard bKing = 0ULL;

    u8 castlingRights = 0U; // ok... nice

    u8 enPassantSquare = 0U; // number 0-63; 0 is the 0th bit, meaning LSB

    u8 halfMoves = 0U; // one byte can hold up to 255, which is easily in a uint8
                       // but this means 50 plays per side

    uint16_t fullMoves = 0U; // 16 bits is enough; games can go on for very long, but not 2 ** 32 - 1 = 4294967295 moves

    Side sideToPlay = Side::WHITE; // white is 0, black is 1
};

#endif