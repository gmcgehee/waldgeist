#pragma once

#include <vector>
#include "bitboard.hpp"
#include "types.hpp"
#include "tables.hpp"

namespace MoveGeneration
{
    // From official Stockfish code:
    /// A move needs 16 bits to be stored
    ///
    /// bit  0- 5: destination square (from 0 to 63)
    /// bit  6-11: origin square (from 0 to 63)
    /// bit 12-13: promotion piece type - 2 (from KNIGHT-2 to QUEEN-2)
    /// bit 14-15: special move flag: promotion (1), en passant (2), castling (3)
    /// NOTE: EN-PASSANT bit is set only when a pawn can be captured
    inline Move convertToMove(Square destination, Square origin, PromotionPiece promotion_piece = TO_NONE, SpecialMoveFlag flag = NONSPECIAL)
    {

        Move move = 0;

        move |= destination;
        move |= origin << 6;
        move |= promotion_piece << 12;
        move |= flag << 14;

        return move;
    }

    std::vector<Move> generatePawnMoves(Bitboard our_state, Bitboard their_state, Bitboard empty, Bitboard p_state, Side us, Square en_passant_square)
    {
        // how is it labelled that there will be an en passant square?
        std::vector<Move> move_list = {};
        move_list.reserve(32);

        Bitboard one_push;
        Bitboard two_push;
        Bitboard captures;

        if (us == Side::WHITE)
        {
            one_push = north(p_state) & empty;
            two_push = north(one_push & RANK_3) & empty; // & with RANK_3 to verify that the pieces that have moved by 1 are actually able to double push

            Square square;
            Move move;
            while (one_push)
            {
                square = pop_lsb(one_push);
                move = convertToMove((Square)(square), (Square)(square - 8));
                move_list.push_back(move);
            }

            while (two_push)
            {
                square = pop_lsb(two_push);
                move = convertToMove((Square)(square), (Square)(square - 16), TO_NONE, EN_PASSANT);
                move_list.push_back(move);
            }

            while (p_state) // generate captures; it's harder to keep track of the original position otherwise
            {
                square = pop_lsb(p_state);
                captures = PAWN_ATTACKS[WHITE][square];
                while (captures) {
                    // implement pop bits and move conversion because we know the square (index)
                }
            }
        }
        else
        {
            one_push = south(p_state) & empty;
            two_push = south(one_push & RANK_6) & empty;

            Square square;
            Move move;

            while (one_push)
            {
                square = pop_lsb(one_push);
                move = convertToMove((Square)(square), (Square)(square + 8));
                move_list.push_back(move);
            }

            while (two_push)
            {
                square = pop_lsb(two_push);
                move = convertToMove((Square)(square), (Square)(square + 16), TO_NONE, EN_PASSANT);
                move_list.push_back(move);
            }
        }

        return move_list;
    }
    std::vector<Move> generateBishopMoves(Bitboard &empty, Bitboard &b_state, Side us);
    std::vector<Move> generateKnightMoves(Bitboard &empty, Bitboard &n_state, Side us);
    std::vector<Move> generateRookMoves(Bitboard &empty, Bitboard &r_state, Side us);
    std::vector<Move> generateQueenMoves(Bitboard &empty, Bitboard &q_state, Side us);
    std::vector<Move> generateKingMoves(Bitboard &empty, Bitboard &k_state, Side us, u8 castling_rights);

};
