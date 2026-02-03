#include "types.hpp"
#include "bitboard.hpp"

/*
    TODO:

    - Generate attack tables for pawns
    - Lookup tables for knight moves

*/

// Scheme: TABLE[SIDE][SQUARE]
// Side  : WHITE or BLACK (0, 1)
// Square: a1-h8 (0 - 63); necessarily 63 squares to allow consistent indexing

Bitboard W_PAWN_ATTACKS[2][64]; 
Bitboard B_PAWN_ATTACKS[2][64];

// Does not depend on side
Bitboard KNIGHT_MOVES[64];