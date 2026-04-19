#pragma once

#include <cstdint>
#include <array>

typedef uint8_t u8;
typedef uint64_t Bitboard;
typedef uint8_t Square;
typedef uint16_t Move;

enum Side : int
{
    WHITE = 0,
    BLACK = 1
};

enum SpecialMoveFlag : int
{
    NONSPECIAL = 0,
    PROMOTION = 1,
    PAWN_DOUBLE_PUSH = 2,
    CASTLING = 3
};

enum PieceType : int
{
    EMPTY = -1,
    PAWN = 0,
    KNIGHT = 1,
    BISHOP = 2,
    ROOK = 3,
    QUEEN = 4,
    KING = 5
};

// enum PromotionPiece : int {
//     TO_NONE, TO_KNIGHT, TO_BISHOP, TO_ROOK, TO_QUEEN
// };

enum BoardSquares : Square
{
    a1,
    b1,
    c1,
    d1,
    e1,
    f1,
    g1,
    h1,
    a2,
    b2,
    c2,
    d2,
    e2,
    f2,
    g2,
    h2,
    a3,
    b3,
    c3,
    d3,
    e3,
    f3,
    g3,
    h3,
    a4,
    b4,
    c4,
    d4,
    e4,
    f4,
    g4,
    h4,
    a5,
    b5,
    c5,
    d5,
    e5,
    f5,
    g5,
    h5,
    a6,
    b6,
    c6,
    d6,
    e6,
    f6,
    g6,
    h6,
    a7,
    b7,
    c7,
    d7,
    e7,
    f7,
    g7,
    h7,
    a8,
    b8,
    c8,
    d8,
    e8,
    f8,
    g8,
    h8
};

struct Piece
{
    PieceType piece_type;
    Side color;
    Bitboard *piece_bb;
};

typedef std::array<Piece, 64> Mailbox;

struct BoardState
{

    Bitboard pieces[2][6] = {
        {0ULL, 0ULL, 0ULL, 0ULL, 0ULL, 0ULL}, // white: pawn, knight, bishop, rook, queen, king
        {0ULL, 0ULL, 0ULL, 0ULL, 0ULL, 0ULL}};

    u8 castlingRights = 0U; // 0000 - KQkq

    Square enPassantSquare = 0U;

    u8 halfMoves = 0U;

    uint16_t fullMoves = 0U;

    Side sideToPlay = Side::WHITE;

};

struct Undo
{
    Move move;
    Piece captured;
    Square en_passant_square;
    u8 castling_rights;
    u8 half_moves;
    uint16_t full_moves;
    // Bitboard zobrist;
};

struct MoveList {
    Move moves[256]{};
    int count = 0;
};