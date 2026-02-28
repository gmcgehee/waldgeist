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

    std::vector<Move> generatePawnCaptures(Bitboard their_state, Bitboard p_state, Side us, Square en_passant_square = 0)
    {
        std::vector<Move> move_list = {};
        move_list.reserve(14); // maximum of 14 possible pawn captures in any state

        Bitboard captures;
        Square origin;
        Square destination;

        if (en_passant_square > 0) // the en passant square will never be a0
        {
            set_bit(their_state, en_passant_square); // en passant is accounted for in make--no need to make a special move flag
        }

        while (p_state) // generate captures; it's harder to keep track of the original position otherwise
        {
            origin = pop_lsb(p_state);
            captures = PAWN_ATTACKS[us][origin] & their_state;

            while (captures)
            {
                destination = pop_lsb(captures);
                move_list.push_back(convertToMove(destination, origin));
            }
        }

        return move_list;
    }

    std::vector<Move> generatePawnCapturesLive(Bitboard their_state, Bitboard p_state, Side us, Square en_passant_square = 0)
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
            east_captures = northe(p_state) & their_state;

            while (east_captures)
            {
                destination = pop_lsb(east_captures);
                origin = destination - 9;
                Move move = convertToMove(destination, origin);
                move_list.push_back(move);
            }

            west_captures = northw(p_state) & their_state;

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

            east_captures = southe(p_state) & their_state;

            while (east_captures)
            {
                destination = pop_lsb(east_captures);
                origin = destination + 7;
                Move move = convertToMove(destination, origin);
                move_list.push_back(move);
            }

            west_captures = southw(p_state) & their_state;

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

    std::vector<Move> generatePawnMoves(Bitboard empty, Bitboard p_state, Side us)
    {
        // how is it labelled that there will be an en passant square?
        std::vector<Move> move_list = {};
        move_list.reserve(32);

        Bitboard one_push;
        Bitboard two_push;

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
    };

    std::vector<Move> generateBishopMoves(Bitboard &empty, Bitboard &b_state, Side us);
    std::vector<Move> generateKnightMoves(Bitboard &empty, Bitboard &n_state, Side us);
    std::vector<Move> generateRookMoves(Bitboard &empty, Bitboard &r_state, Side us);
    std::vector<Move> generateQueenMoves(Bitboard &empty, Bitboard &q_state, Side us);
    std::vector<Move> generateKingMoves(Bitboard &empty, Bitboard &k_state, Side us, u8 castling_rights);

}
