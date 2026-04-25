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

    inline Move convertToMove(Square destination, Square origin, PieceType promotion_piece = PAWN, SpecialMoveFlag flag = NONSPECIAL)
    {

        // From official Stockfish code:
        /// A move needs 16 bits to be stored
        ///
        /// bit  0- 5: destination square (from 0 to 63)
        /// bit  6-11: origin square (from 0 to 63)
        /// bit 12-13: promotion piece type - 2 (from KNIGHT-2 to QUEEN-2)
        /// bit 14-15: special move flag: promotion (1), double push (2), castling (3)
        /// NOTE: EN-PASSANT bit is set only when a pawn can be captured // wait, is this supposed to mean there is a pawn that can capture
        //  ^^^^^^^^^^ does this mean it is set when a pawn is double pushed, or when en passant is performed in a capture? this is pretty crucial.

        Move move = 0U;

        move |= destination;
        move |= origin << 6U;
        move |= promotion_piece << 12U;
        move |= flag << 14U;

        return move;
    }

    void generatePawnCaptures(Bitboard their_state, Bitboard our_p_state, Side us, Square en_passant_square, MoveList &move_list)
    {

        Bitboard west_captures;
        Bitboard east_captures;
        Square origin;
        Square destination;

        if (en_passant_square > 0) // the en passant square will never be a1
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
                if (destination >= a8)
                {
                    for (int piece = 0; piece < KING - 1; piece++)
                    {
                        Move move = convertToMove(destination, origin, (PieceType)piece, PROMOTION);
                        move_list.moves[move_list.count++] = move;
                    }
                    continue;
                }
                Move move = convertToMove(destination, origin);
                move_list.moves[move_list.count++] = move;
            }

            west_captures = northw(our_p_state) & their_state;

            while (west_captures)
            {
                destination = pop_lsb(west_captures);
                origin = destination - 7;
                if (destination >= a8)
                {
                    for (int piece = 0; piece < KING - 1; piece++)
                    {
                        Move move = convertToMove(destination, origin, (PieceType)piece, PROMOTION);
                        move_list.moves[move_list.count++] = move;
                    }
                    continue;
                }
                Move move = convertToMove(destination, origin);
                move_list.moves[move_list.count++] = move;
            }
        }
        else
        {

            east_captures = southe(our_p_state) & their_state;

            while (east_captures)
            {
                destination = pop_lsb(east_captures);
                origin = destination + 7;
                if (destination <= h1)
                {
                    for (int piece = 0; piece < KING - 1; piece++)
                    {
                        Move move = convertToMove(destination, origin, (PieceType)piece, PROMOTION);
                        move_list.moves[move_list.count++] = move;
                    }
                    continue;
                }
                Move move = convertToMove(destination, origin);
                move_list.moves[move_list.count++] = move;
            }

            west_captures = southw(our_p_state) & their_state;

            while (west_captures)
            {
                destination = pop_lsb(west_captures);
                origin = destination + 9;
                if (destination <= h1)
                {
                    for (int piece = 0; piece < KING - 1; piece++)
                    {
                        Move move = convertToMove(destination, origin, (PieceType)piece, PROMOTION);
                        move_list.moves[move_list.count++] = move;
                    }
                    continue;
                }
                Move move = convertToMove(destination, origin);
                move_list.moves[move_list.count++] = move;
            }
        }
    }

    void generatePawnPushes(Bitboard empty, Bitboard our_p_state, Side us, MoveList &move_list)
    {

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

                if (square >= a8)
                {
                    for (int piece = 0; piece < KING - 1; piece++)
                    {
                        Move move = convertToMove(square, square - 8, (PieceType)piece, PROMOTION);
                        move_list.moves[move_list.count++] = move;
                    }
                    continue;
                }
                move = convertToMove(square, square - 8);
                move_list.moves[move_list.count++] = move;
            }

            while (two_push)
            {
                square = pop_lsb(two_push);
                move = convertToMove(square, square - 16, PAWN, PAWN_DOUBLE_PUSH);
                move_list.moves[move_list.count++] = move;
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

                if (square <= h1)
                {
                    for (int piece = 0; piece < KING - 1; piece++)
                    {
                        Move move = convertToMove(square, square + 8, (PieceType)piece, PROMOTION);
                        move_list.moves[move_list.count++] = move;
                    }
                    continue;
                }

                move = convertToMove(square, square + 8);
                move_list.moves[move_list.count++] = move;
            }

            while (two_push)
            {
                square = pop_lsb(two_push);
                move = convertToMove(square, square + 16, PAWN, PAWN_DOUBLE_PUSH);
                move_list.moves[move_list.count++] = move;
            }
        }
    };

    void generateKnightCaptures(Bitboard their_state, Bitboard our_n_state, MoveList &move_list)
    {

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
                move_list.moves[move_list.count++] = move;
            }
        }
    }

    void generateKnightQuiets(Bitboard empty, Bitboard our_n_state, MoveList &move_list)
    {

        while (our_n_state)
        {
            Square origin = pop_lsb(our_n_state);
            Bitboard knight_squares = KNIGHT_MOVES[origin] & empty;

            while (knight_squares)
            {
                Square destination = pop_lsb(knight_squares);
                Move move = convertToMove(destination, origin);
                move_list.moves[move_list.count++] = move;
            }
        }
    }

    // todo: currently returns captures and quiets as one thing
    void generateBishopMoves(Bitboard occ, Bitboard empty, Bitboard their_state, Bitboard our_b_state, MoveList &move_list)
    {

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
                move_list.moves[move_list.count++] = move;
            }

            while (quiets)
            {
                Square destination = pop_lsb(quiets);
                Move move = convertToMove(destination, origin);
                move_list.moves[move_list.count++] = move;
            }
        }
    }

    // NOTE: currently unused
    void generateBishopCaptures(Bitboard occ, Bitboard empty, Bitboard their_state, Bitboard our_b_state, MoveList &move_list);

    // NOTE: currently unused
    void generateBishopQuiets(Bitboard empty, Bitboard our_b_state, MoveList &move_list);

    void generateRookMoves(Bitboard occ, Bitboard empty, Bitboard their_state, Bitboard our_r_state, MoveList &move_list)
    {

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
                move_list.moves[move_list.count++] = move;
            }

            while (quiets)
            {
                Square destination = pop_lsb(quiets);
                Move move = convertToMove(destination, origin);
                move_list.moves[move_list.count++] = move;
            }
        }
    }

    // NOTE: currently unused
    void generateRookCaptures(Bitboard empty, Bitboard our_r_state, MoveList &move_list);

    // NOTE: currently unused
    void generateRookQuiets(Bitboard empty, Bitboard our_r_state, MoveList &move_list);

    void generateQueenMoves(Bitboard occ, Bitboard empty, Bitboard their_state, Bitboard our_q_state, MoveList &move_list)
    {

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
                move_list.moves[move_list.count++] = move;
            }

            while (quiets)
            {
                Square destination = pop_lsb(quiets);
                Move move = convertToMove(destination, origin);
                move_list.moves[move_list.count++] = move;
            }
        }
    }

    // NOTE: currently unused
    void generateQueenCaptures(Bitboard their_state, Bitboard our_q_state, MoveList &move_list);

    // NOTE: currently unused
    void generateQueenQuiets(Bitboard empty, Bitboard our_q_state, MoveList &move_list);

    void generateKingCaptures(Bitboard their_state, Bitboard our_k_state, MoveList &move_list)
    {

        Square origin = __builtin_ctzll(our_k_state); // no need to pop lsb and modify k state
        Bitboard king_squares = KING_MOVES[origin] & their_state;

        while (king_squares)
        {
            Square destination = pop_lsb(king_squares);
            Move move = convertToMove(destination, origin);
            move_list.moves[move_list.count++] = move;
        }
    }

    void generateKingQuiets(Bitboard empty, Bitboard our_k_state, u8 castling_rights, Side us, MoveList &move_list)
    {

        Square origin = __builtin_ctzll(our_k_state); // no need to pop lsb and modify k state
        Bitboard king_squares = KING_MOVES[origin] & empty;

        while (king_squares)
        {
            Square destination = pop_lsb(king_squares);
            Move move = convertToMove(destination, origin);
            move_list.moves[move_list.count++] = move;
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
                move_list.moves[move_list.count++] = (0b1100000100000110); // hard-coded binary castling
            }

            if (!(west_mask & occupied) && (4 & castling_rights))
            {
                move_list.moves[move_list.count++] = (0b1100000100000010); // hard-coded binary castling
            }
        }

        else
        {
            if (!(east_mask & occupied) && (2 & castling_rights))
            {
                move_list.moves[move_list.count++] = (0b1100111100111110);
            }

            if (!(west_mask & occupied) && (1 & castling_rights))
            {
                move_list.moves[move_list.count++] = (0b1100111100111010);
            }
        }
    }

    void generateAllMoves(
        Bitboard occ, Bitboard empty, Bitboard their_state, Side us,
        Bitboard our_p_state, Bitboard our_n_state, Bitboard our_b_state,
        Bitboard our_r_state, Bitboard our_q_state, Bitboard our_k_state,
        u8 castling_rights, Square en_passant_square, MoveList &move_list)
    {

        generatePawnCaptures(their_state, our_p_state, us, en_passant_square, move_list);
        generateKnightCaptures(their_state, our_n_state, move_list);
        generatePawnPushes(empty, our_p_state, us, move_list);
        generateKnightQuiets(empty, our_n_state, move_list);
        generateBishopMoves(occ, empty, their_state, our_b_state, move_list);
        generateRookMoves(occ, empty, their_state, our_r_state, move_list);
        generateQueenMoves(occ, empty, their_state, our_q_state, move_list);
        generateKingCaptures(their_state, our_k_state, move_list);
        generateKingQuiets(empty, our_k_state, castling_rights, us, move_list);
    }

    void generateAllCaptures(
        Bitboard occ, Bitboard empty, Bitboard their_state, Side us,
        Bitboard our_p_state, Bitboard our_n_state, Bitboard our_b_state,
        Bitboard our_r_state, Bitboard our_q_state, Bitboard our_k_state,
        u8 castling_rights, Square en_passant_square, MoveList &move_list)
    {

        generatePawnCaptures(their_state, our_p_state, us, en_passant_square, move_list);
        generateKnightCaptures(their_state, our_n_state, move_list);
        // append_moves(generateBishopCaptures(occ, empty, their_state, our_b_state));
        // append_moves(generateRookCaptures(occ, empty, their_state, our_r_state));
        // append_moves(generateQueenCaptures(occ, empty, their_state, our_q_state));
        generateKingCaptures(their_state, our_k_state, move_list);
    }
}
