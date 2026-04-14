#pragma once

#include <vector>
#include <bit>
#include <immintrin.h>

#include "bitboard.hpp"
#include "types.hpp"
#include "tables.hpp"
#include "../precalculation/bishop/bishop_pext_tables.hpp"
#include "../precalculation/bishop/bishop_ray_masks.hpp"
#include "../precalculation/rook/rook_pext_tables.hpp"
#include "../precalculation/rook/rook_ray_masks.hpp"

namespace MoveGeneration
{

    inline Move convertToMove(Square destination, Square origin, PromotionPiece promotion_piece = TO_NONE, SpecialMoveFlag flag = NONSPECIAL)
    {

        // From official Stockfish code:
        /// A move needs 16 bits to be stored
        ///
        /// bit  0- 5: destination square (from 0 to 63)
        /// bit  6-11: origin square (from 0 to 63)
        /// bit 12-13: promotion piece type - 2 (from KNIGHT-2 to QUEEN-2)
        /// bit 14-15: special move flag: promotion (1), en passant (2), castling (3)
        /// NOTE: EN-PASSANT bit is set only when a pawn can be captured
        //  ^^^^^^^^^^ does this mean it is set when a pawn is double pushed, or when en passant is performed in a capture? this is pretty crucial.

        Move move = 0U;

        move |= destination;
        move |= origin << 6U;
        move |= promotion_piece << 12U;
        move |= flag << 14U;

        return move;
    }

    std::vector<Move> generatePawnCaptures(Bitboard their_state, Bitboard our_p_state, Side us, Square en_passant_square = 0)
    {
        std::vector<Move> move_list = {};
        move_list.reserve(14); // maximum of 14 possible pawn captures in any state

        Bitboard west_captures;
        Bitboard east_captures;
        Square origin;
        Square destination;

        if (en_passant_square > 0) // the en passant square will never be a0
        {
            set_bit(their_state, en_passant_square); // en passant is accounted for in make--no need to make a special move flag
        }

        if (us == WHITE)
        {
            east_captures = northe(our_p_state) & their_state;

            while (east_captures)
            {
                destination = pop_lsb(east_captures);
                origin = destination - 9;
                Move move = convertToMove(destination, origin);
                move_list.push_back(move);
            }

            west_captures = northw(our_p_state) & their_state;

            while (west_captures)
            {
                destination = pop_lsb(west_captures);
                origin = destination - 7;
                Move move = convertToMove(destination, origin);
                move_list.push_back(move);
            }
        }
        else
        {

            east_captures = southe(our_p_state) & their_state;

            while (east_captures)
            {
                destination = pop_lsb(east_captures);
                origin = destination + 7;
                Move move = convertToMove(destination, origin);
                move_list.push_back(move);
            }

            west_captures = southw(our_p_state) & their_state;

            while (west_captures)
            {
                destination = pop_lsb(west_captures);
                origin = destination + 9;
                Move move = convertToMove(destination, origin);
                move_list.push_back(move);
            }
        }

        return move_list;
    }

    std::vector<Move> generatePawnPushes(Bitboard empty, Bitboard our_p_state, Side us)
    {
        // TODO: include promotions
        // how is it labelled that there will be an en passant square?
        std::vector<Move> move_list = {};
        move_list.reserve(16);

        Bitboard one_push;
        Bitboard two_push;

        if (us == Side::WHITE)
        {
            one_push = north(our_p_state) & empty;
            two_push = north(one_push & RANK_3) & empty; // & with RANK_3 to verify that the pieces that have moved by 1 are actually able to double push

            Square square;
            Move move;

            while (one_push)
            {
                square = pop_lsb(one_push);
                move = convertToMove(square, square - 8);
                move_list.push_back(move);
            }

            while (two_push)
            {
                square = pop_lsb(two_push);
                move = convertToMove(square, square - 16, TO_NONE, EN_PASSANT);
                move_list.push_back(move);
            }
        }
        else
        {
            one_push = south(our_p_state) & empty;
            two_push = south(one_push & RANK_6) & empty;

            Square square;
            Move move;

            while (one_push)
            {
                square = pop_lsb(one_push);
                move = convertToMove(square, square + 8);
                move_list.push_back(move);
            }

            while (two_push)
            {
                square = pop_lsb(two_push);
                move = convertToMove(square, square + 16, TO_NONE, EN_PASSANT);
                move_list.push_back(move);
            }
        }

        return move_list;
    };

    std::vector<Move> generateKnightCaptures(Bitboard their_state, Bitboard our_n_state)
    {
        std::vector<Move> move_list;
        move_list.reserve(8 * std::popcount(our_n_state)); // NOTE: is this right to do? some considerations:

        // - The number 16 (originally) is because two knights in the middle of the board (safe assumption) have 8 moves
        // - Might I want to take the populattion count?

        while (our_n_state)
        {
            Square origin = pop_lsb(our_n_state);
            Bitboard knight_captures_squares = KNIGHT_MOVES[origin] & their_state;

            while (knight_captures_squares)
            {
                Square destination = pop_lsb(knight_captures_squares);
                Move move = convertToMove(destination, origin);
                move_list.push_back(move);
            }
        }
        return move_list;
    }

    std::vector<Move> generateKnightQuiets(Bitboard empty, Bitboard our_n_state)
    {

        std::vector<Move> move_list;
        move_list.reserve(8 * std::popcount(our_n_state)); // NOTE: is this right to do? some considerations:

        // - The number 16 (originally) is because two knights in the middle of the board (safe assumption) have 8 moves
        // - Might I want to take the populattion count?

        while (our_n_state)
        {
            Square origin = pop_lsb(our_n_state);
            Bitboard knight_squares = KNIGHT_MOVES[origin] & empty;

            while (knight_squares)
            {
                Square destination = pop_lsb(knight_squares);
                Move move = convertToMove(destination, origin);
                move_list.push_back(move);
            }
        }
        return move_list;
    }

    // todo: currently returns captures and quiets as one thing
    std::vector<Move> generateBishopMoves(Bitboard occ, Bitboard empty, Bitboard their_state, Bitboard our_b_state)
    {

        std::vector<Move> move_list;

        /*
        Logical ordering:
        Generate all bishop moves given bishop current position and blockers (~empty)
        & it with their state
        return move list

        considerations: it's going to have to pop off one bit at a time anyways. what if I generate all possible moves right here, but with captures at the beginning?
        how would I mak
        */

        // O(bishop_count) time complexity
        while (our_b_state)
        {
            Square origin = pop_lsb(our_b_state);
            int pext_index = (int)_pext_u64(occ, BISHOP_RAY_MASKS[origin]);
            Bitboard moves = BISHOP_PEXT_TABLES[origin][pext_index];
            Bitboard captures = moves & their_state;
            Bitboard quiets = moves & empty;

            while (captures)
            {
                Square destination = pop_lsb(captures);
                Move move = convertToMove(destination, origin);
                move_list.push_back(move);
            }

            while (quiets)
            {
                Square destination = pop_lsb(quiets);
                Move move = convertToMove(destination, origin);
                move_list.push_back(move);
            }
        }

        return move_list;
    }

    // NOTE: currently unused
    std::vector<Move> generateBishopCaptures(Bitboard occ, Bitboard empty, Bitboard their_state, Bitboard our_b_state);

    // NOTE: currently unused
    std::vector<Move> generateBishopQuiets(Bitboard empty, Bitboard our_b_state);

    std::vector<Move> generateRookMoves(Bitboard occ, Bitboard empty, Bitboard their_state, Bitboard our_r_state)
    {

        std::vector<Move> move_list;

        while (our_r_state)
        {
            Square origin = pop_lsb(our_r_state);
            int pext_index = (int)_pext_u64(occ, ROOK_RAY_MASKS[origin]);
            Bitboard moves = ROOK_PEXT_TABLES[origin][pext_index];
            Bitboard captures = moves & their_state;
            Bitboard quiets = moves & empty;

            while (captures)
            {
                Square destination = pop_lsb(captures);
                Move move = convertToMove(destination, origin);
                move_list.push_back(move);
            }

            while (quiets)
            {
                Square destination = pop_lsb(quiets);
                Move move = convertToMove(destination, origin);
                move_list.push_back(move);
            }
        }

        return move_list;
    }

    // NOTE: currently unused
    std::vector<Move> generateRookCaptures(Bitboard empty, Bitboard our_r_state);

    // NOTE: currently unused
    std::vector<Move> generateRookQuiets(Bitboard empty, Bitboard our_r_state);

    std::vector<Move> generateQueenMoves(Bitboard occ, Bitboard empty, Bitboard their_state, Bitboard our_q_state)
    {
        std::vector<Move> move_list;

        while (our_q_state)
        {
            Square origin = pop_lsb(our_q_state);
            int line_pext_index = (int)_pext_u64(occ, ROOK_RAY_MASKS[origin]);
            int diag_pext_index = (int)_pext_u64(occ, BISHOP_RAY_MASKS[origin]);

            Bitboard moves = ROOK_PEXT_TABLES[origin][line_pext_index] | BISHOP_PEXT_TABLES[origin][diag_pext_index];
            Bitboard captures = moves & their_state;
            Bitboard quiets = moves & empty;

            while (captures)
            {
                Square destination = pop_lsb(captures);
                Move move = convertToMove(destination, origin);
                move_list.push_back(move);
            }

            while (quiets)
            {
                Square destination = pop_lsb(quiets);
                Move move = convertToMove(destination, origin);
                move_list.push_back(move);
            }
        }

        return move_list;
    }

    // NOTE: currently unused
    std::vector<Move> generateQueenCaptures(Bitboard their_state, Bitboard our_q_state);

    // NOTE: currently unused
    std::vector<Move> generateQueenQuiets(Bitboard empty, Bitboard our_q_state);

    std::vector<Move> generateKingCaptures(Bitboard their_state, Bitboard our_k_state)
    {
        std::vector<Move> move_list;

        move_list.reserve(8);

        Square origin = __builtin_ctzll(our_k_state); // no need to pop lsb and modify k state
        Bitboard king_squares = KING_MOVES[origin] & their_state;

        while (king_squares)
        {
            Square destination = pop_lsb(king_squares);
            Move move = convertToMove(destination, origin);
            move_list.push_back(move);
        }

        return move_list;
    }

    std::vector<Move> generateKingQuiets(Bitboard empty, Bitboard our_k_state, u8 castling_rights, Side us)
    {
        std::vector<Move> move_list;

        move_list.reserve(8); // even with castling taken into account, there will only ever be 8 max

        Square origin = __builtin_ctzll(our_k_state); // no need to pop lsb and modify k state
        Bitboard king_squares = KING_MOVES[origin] & empty;

        while (king_squares)
        {
            Square destination = pop_lsb(king_squares);
            Move move = convertToMove(destination, origin);
            move_list.push_back(move);
        }

        // Castling rights
        // if we are white, we need to check if the two squares to our right are clear,
        // and if the three squares to our left are clear

        // Or, avoid branching and do it this way:
        Bitboard east_mask = 0b1100000ULL << (56 * us); // the 56 * us will shift it to the 8th rank if (us == BLACK)
        Bitboard west_mask = 0b1110ULL << (56 * us);

        Bitboard occupied = ~empty;

        /* steps (perform bitwise?)
        1. See if castling a particular way is possible (bits)
        2. If so, generate a move that from kingpos to there
        */

        if (us == WHITE) // can I avoid branching ?
        {
            if (!(east_mask & occupied) && (8 & castling_rights))
            {
                move_list.push_back(0b1100000100000110); // hard-coded binary castling
            }

            if (!(west_mask & occupied) && (4 & castling_rights))
            {
                move_list.push_back(0b1100000100000010); // hard-coded binary castling
            }
        }

        else
        {
            if (!(east_mask & occupied) && (2 & castling_rights))
            {
                move_list.push_back(0b1100111100111110);
            }

            if (!(west_mask & occupied) && (1 & castling_rights))
            {
                move_list.push_back(0b1100111100111010);
            }
        }

        return move_list;
    }

    std::vector<Move> generateAllMoves(
        Bitboard occ, Bitboard empty, Bitboard their_state, Side us,
        Bitboard our_p_state, Bitboard our_n_state, Bitboard our_b_state,
        Bitboard our_r_state, Bitboard our_q_state, Bitboard our_k_state,
        u8 castling_rights, Square en_passant_square = 0)
    {
        std::vector<Move> move_list;
        move_list.reserve(256);

        auto append_moves = [&move_list](std::vector<Move> &&moves)
        {
            move_list.insert(
                move_list.end(),
                std::make_move_iterator(moves.begin()),
                std::make_move_iterator(moves.end()));
        };

        append_moves(generatePawnCaptures(their_state, our_p_state, us, en_passant_square));
        append_moves(generatePawnPushes(empty, our_p_state, us));
        append_moves(generateKnightCaptures(their_state, our_n_state));
        append_moves(generateKnightQuiets(empty, our_n_state));
        append_moves(generateBishopMoves(occ, empty, their_state, our_b_state));
        append_moves(generateRookMoves(occ, empty, their_state, our_r_state));
        append_moves(generateQueenMoves(occ, empty, their_state, our_q_state));
        append_moves(generateKingCaptures(their_state, our_k_state));
        append_moves(generateKingQuiets(empty, our_k_state, castling_rights, us));

        return move_list;
    }

}
