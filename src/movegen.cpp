#include <vector>
#include "movegen.hpp"
#include "bitboard.hpp"
#include "types.hpp"

// From official stockfish code:

/// A move needs 16 bits to be stored
///
/// bit  0- 5: destination square (from 0 to 63)
/// bit  6-11: origin square (from 0 to 63)
/// bit 12-13: promotion piece type - 2 (from KNIGHT-2 to QUEEN-2)
/// bit 14-15: special move flag: promotion (1), en passant (2), castling (3)
/// NOTE: EN-PASSANT bit is set only when a pawn can be captured

std::vector<Move> MoveGeneration::generatePawnMoves(Bitboard &empty, Bitboard &p_state, Side to_play, Square en_passant_square)
{

    std::vector<Move> move_list = {};

    Bitboard one_push;
    Bitboard two_push;
    Bitboard captures;

    if (to_play == Side::WHITE)
    {
        one_push = north(p_state) & empty;
        two_push = north(one_push & RANK_3) & empty; // & with RANK_3 to verify that the pieces that have moved by 1 are actually able to double push
    }
    else
    {
        one_push = south(p_state) & empty;
        two_push = south(one_push & RANK_6) & empty;
    }

    


    return move_list;
}
