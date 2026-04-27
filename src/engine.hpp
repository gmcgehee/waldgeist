#pragma once

#include <set>
#include <vector>
#include <bit>

#include "types.hpp"
#include "bitboard.hpp"
#include "gamestate.hpp"
#include "movegen.hpp"
#include "debug.hpp"
#include "../precalculation/piece_square.hpp"

class Engine
{
public:
    GameState gamestate;

    Engine()
    {
        gamestate = GameState();
    }

    inline Square get_destination_square(Move move)
    {
        return move & 0x3F;
    }

    inline Square get_origin_square(Move move)
    {
        return (move >> 6) & 0x3F;
    }

    inline u8 get_flag(Move move)
    {
        return move >> 14;
    }

    inline float get_move_score(Move move)
    {

        Square origin = get_origin_square(move);
        Square destination = get_destination_square(move);
        PieceType origin_piece = gamestate.getPieceAt(origin).piece_type;
        PieceType destination_piece = gamestate.getPieceAt(destination).piece_type;

        if (origin_piece == PAWN and destination == gamestate.state.enPassantSquare)
        {
            destination_piece = PAWN;
        }

        if (destination_piece == EMPTY)
            return 0;

        // MVV LVA
        return MVV_LVA[destination_piece][origin_piece];
    }

    std::pair<float, Move> alpha_beta(int depth, float alpha, float beta)
    {

        float max_score = -__FLT_MAX__;
        float curr_score{};
        Move best_move{};

        Bitboard occ = gamestate.getFullState();
        Bitboard empty = ~occ;

        Side us = gamestate.state.sideToPlay;
        Side them = (us == WHITE) ? BLACK : WHITE;

        Bitboard their_state = gamestate.getSideState(them);

        Bitboard our_p_state = gamestate.state.pieces[us][PAWN];
        Bitboard our_n_state = gamestate.state.pieces[us][KNIGHT];
        Bitboard our_b_state = gamestate.state.pieces[us][BISHOP];
        Bitboard our_r_state = gamestate.state.pieces[us][ROOK];
        Bitboard our_q_state = gamestate.state.pieces[us][QUEEN];
        Bitboard our_k_state = gamestate.state.pieces[us][KING];

        u8 castling_rights = gamestate.state.castlingRights;
        Square en_passant_square = gamestate.state.enPassantSquare;

        MoveList move_list{};

        MoveGeneration::generateAllMoves(
            occ,
            empty,
            their_state,
            us,
            our_p_state,
            our_n_state,
            our_b_state,
            our_r_state,
            our_q_state,
            our_k_state,
            castling_rights,
            en_passant_square,
            move_list);

        int legal_moves = 0;

        std::array<float, 256> scores{};

        for (int i = 0; i < move_list.count; i++)
            scores[i] = get_move_score(move_list.moves[i]);

        for (int i = 0; i < move_list.count; i++)
        {

            int best_score_index{i};
            float best_move_score{scores[i]};

            for (int j = i + 1; j < move_list.count; j++)
            {
                if (scores[j] > best_move_score)
                {
                    best_move_score = scores[j];
                    best_score_index = j;
                }
            }

            std::swap(move_list.moves[i], move_list.moves[best_score_index]);
            std::swap(scores[i], scores[best_score_index]);

            Move curr_move = move_list.moves[i];

            Undo undo;

            if (gamestate.make(curr_move, undo))
            {
                legal_moves++;
                curr_score = -alpha_beta_recursion(depth - 1, -beta, -alpha);
                // std::cout << MoveGeneration::moveToString(curr_move) << ": " << curr_score << '\n';

                gamestate.unmake(curr_move, undo);

                if (curr_score > max_score)
                {
                    max_score = curr_score;
                    best_move = curr_move;
                }

                alpha = curr_score > alpha ? curr_score : alpha;

                if (alpha >= beta)
                {
                    break;
                }
            }
        }

        if (legal_moves == 0)
        {
            if (gamestate.isSquareThreatened(__builtin_ctzll(gamestate.state.pieces[us][KING]), them))
                return std::pair<float, Move>{-MATE_SCORE + depth, best_move};
            else
            {
                return std::pair<float, Move>{0, best_move};
            }
        }

        return std::pair<float, Move>{max_score, best_move};
    }

    float alpha_beta_recursion(int depth, float alpha, float beta)
    {

        if (depth == 0)
        {
            return quiesce(alpha, beta);
        }

        float max_score = -__FLT_MAX__;
        float curr_score{};

        Bitboard occ = gamestate.getFullState();
        Bitboard empty = ~occ;

        Side us = gamestate.state.sideToPlay;
        Side them = (us == WHITE) ? BLACK : WHITE;

        Bitboard their_state = gamestate.getSideState(them);

        Bitboard our_p_state = gamestate.state.pieces[us][PAWN];
        Bitboard our_n_state = gamestate.state.pieces[us][KNIGHT];
        Bitboard our_b_state = gamestate.state.pieces[us][BISHOP];
        Bitboard our_r_state = gamestate.state.pieces[us][ROOK];
        Bitboard our_q_state = gamestate.state.pieces[us][QUEEN];
        Bitboard our_k_state = gamestate.state.pieces[us][KING];

        u8 castling_rights = gamestate.state.castlingRights;
        Square en_passant_square = gamestate.state.enPassantSquare;

        MoveList move_list{};

        MoveGeneration::generateAllMoves(
            occ,
            empty,
            their_state,
            us,
            our_p_state,
            our_n_state,
            our_b_state,
            our_r_state,
            our_q_state,
            our_k_state,
            castling_rights,
            en_passant_square,
            move_list);

        int legal_moves = 0;

        std::array<float, 256> scores{};

        for (int i = 0; i < move_list.count; i++)
            scores[i] = get_move_score(move_list.moves[i]);

        for (int i = 0; i < move_list.count; i++)
        {

            int best_score_index{i};
            float best_move_score{-1000000};

            for (int j = i + 1; j < move_list.count; j++)
            {
                if (scores[j] > best_move_score)
                {
                    best_move_score = scores[j];
                    best_score_index = j;
                }
            }

            std::swap(move_list.moves[i], move_list.moves[best_score_index]);
            std::swap(scores[i], scores[best_score_index]);

            Move curr_move = move_list.moves[i];

            Undo undo;

            if (gamestate.make(curr_move, undo))
            {
                legal_moves++;
                curr_score = -alpha_beta_recursion(depth - 1, -beta, -alpha); // may need to be 'max(curr_best, search())'

                gamestate.unmake(curr_move, undo);

                if (curr_score > max_score)
                {
                    max_score = curr_score;
                }

                alpha = curr_score > alpha ? curr_score : alpha;

                if (alpha >= beta)
                {
                    break;
                }
            }
        }

        if (legal_moves == 0)
        {
            if (gamestate.isSquareThreatened(__builtin_ctzll(gamestate.state.pieces[us][KING]), them))
                return -MATE_SCORE + depth;
            else
            {
                return 0;
            }
        }

        return max_score;
    }

    float quiesce(float alpha, float beta)
    {

        float stand_pat = eval();

        if (stand_pat >= beta)
            return beta;

        if (stand_pat > alpha)
            alpha = stand_pat;

        float max_score = stand_pat;
        float curr_score{};

        Bitboard occ = gamestate.getFullState();
        Bitboard empty = ~occ;

        Side us = gamestate.state.sideToPlay;
        Side them = (us == WHITE) ? BLACK : WHITE;

        Bitboard their_state = gamestate.getSideState(them);

        Bitboard our_p_state = gamestate.state.pieces[us][PAWN];
        Bitboard our_n_state = gamestate.state.pieces[us][KNIGHT];
        Bitboard our_b_state = gamestate.state.pieces[us][BISHOP];
        Bitboard our_r_state = gamestate.state.pieces[us][ROOK];
        Bitboard our_q_state = gamestate.state.pieces[us][QUEEN];
        Bitboard our_k_state = gamestate.state.pieces[us][KING];

        u8 castling_rights = gamestate.state.castlingRights;
        Square en_passant_square = gamestate.state.enPassantSquare;

        MoveList capture_list{};

        MoveGeneration::generateAllCaptures(
            occ,
            empty,
            their_state,
            us,
            our_p_state,
            our_n_state,
            our_b_state,
            our_r_state,
            our_q_state,
            our_k_state,
            castling_rights,
            en_passant_square,
            capture_list);

        if (capture_list.count == 0)
        {
            return stand_pat;
        }

        int legal_captures{};

        std::array<float, 256> scores{};

        for (int i = 0; i < capture_list.count; i++)
            scores[i] = get_move_score(capture_list.moves[i]);

        for (int i = 0; i < capture_list.count; i++)
        {

            int best_score_index{i};
            float best_move_score{-1000000};

            for (int j = i + 1; j < capture_list.count; j++)
            {
                if (scores[j] > best_move_score)
                {
                    best_move_score = scores[j];
                    best_score_index = j;
                }
            }

            std::swap(capture_list.moves[i], capture_list.moves[best_score_index]);
            std::swap(scores[i], scores[best_score_index]);

            Move curr_move = capture_list.moves[i];

            Undo undo;

            if (gamestate.make(curr_move, undo))
            {
                legal_captures++;
                curr_score = -quiesce(-beta, -alpha); // may need to be 'max(curr_best, search())'
                gamestate.unmake(curr_move, undo);

                if (curr_score > max_score)
                {
                    max_score = curr_score;
                }

                if (curr_score >= beta)
                {
                    break;
                }
                alpha = curr_score > alpha ? curr_score : alpha;
            }
        }

        return max_score;
    }

    float eval()
    {
        BoardState state = gamestate.state;

        // Material eval
        float p_score = 100 * (std::popcount(state.pieces[WHITE][PAWN]) - std::popcount(state.pieces[BLACK][PAWN]));
        float n_score = 300 * (std::popcount(state.pieces[WHITE][KNIGHT]) - std::popcount(state.pieces[BLACK][KNIGHT]));
        float b_score = 320 * (std::popcount(state.pieces[WHITE][BISHOP]) - std::popcount(state.pieces[BLACK][BISHOP]));
        float r_score = 500 * (std::popcount(state.pieces[WHITE][ROOK]) - std::popcount(state.pieces[BLACK][ROOK]));
        float q_score = 900 * (std::popcount(state.pieces[WHITE][QUEEN]) - std::popcount(state.pieces[BLACK][QUEEN]));
        float material_eval = p_score + n_score + b_score + r_score + q_score;

        // Piece-square eval
        float square_eval{};
        for (int i = 0; i < KING - 1; i++)
        {
            Bitboard curr_piece = state.pieces[WHITE][i];
            while (curr_piece)
            {
                Square sq = pop_lsb(curr_piece);
                square_eval += PST[i][sq];
            }
        }

        for (int i = 0; i < KING - 1; i++)
        {
            Bitboard curr_piece = state.pieces[BLACK][i];
            while (curr_piece)
            {
                Square sq = pop_lsb(curr_piece);
                square_eval += PST[i][inverse_square(sq)];
            }
        }

        // King safety eval


        return (material_eval + square_eval) * (state.sideToPlay == WHITE ? 1 : -1);
    }

    unsigned long long perft(int depth)
    {
        unsigned long long node_count = 0;

        Bitboard occ = gamestate.getFullState();
        Bitboard empty = ~occ;

        Side us = gamestate.state.sideToPlay;
        Side them = (us == WHITE) ? BLACK : WHITE;

        Bitboard their_state = gamestate.getSideState(them);

        Bitboard our_p_state = gamestate.state.pieces[us][PAWN];
        Bitboard our_n_state = gamestate.state.pieces[us][KNIGHT];
        Bitboard our_b_state = gamestate.state.pieces[us][BISHOP];
        Bitboard our_r_state = gamestate.state.pieces[us][ROOK];
        Bitboard our_q_state = gamestate.state.pieces[us][QUEEN];
        Bitboard our_k_state = gamestate.state.pieces[us][KING];

        u8 castling_rights = gamestate.state.castlingRights;
        Square en_passant_square = gamestate.state.enPassantSquare;

        MoveList move_list{};

        MoveGeneration::generateAllMoves(
            occ,
            empty,
            their_state,
            us,
            our_p_state,
            our_n_state,
            our_b_state,
            our_r_state,
            our_q_state,
            our_k_state,
            castling_rights,
            en_passant_square,
            move_list);

        for (int i = 0; i < move_list.count; i++)
        {
            BoardState old_state = gamestate.state;
            Mailbox old_mailbox = gamestate.mailbox;
            const Move current_move = move_list.moves[i];

            Undo undo;

            if (gamestate.make(current_move, undo))
            {
                unsigned long long curr_move_node_count = perft_recursion(depth - 1);
                std::cout << MoveGeneration::moveToString(current_move) << ": " << curr_move_node_count << '\n';
                node_count += curr_move_node_count;
                gamestate.unmake(current_move, undo);
            }
        }
        return node_count;
    }

    unsigned long long perft_recursion(int depth)
    {

        unsigned long long node_count = 0;

        Bitboard occ = gamestate.getFullState();
        Bitboard empty = ~occ;

        Side us = gamestate.state.sideToPlay;
        Side them = (us == WHITE) ? BLACK : WHITE;

        Bitboard their_state = gamestate.getSideState(them);

        Bitboard our_p_state = gamestate.state.pieces[us][PAWN];
        Bitboard our_n_state = gamestate.state.pieces[us][KNIGHT];
        Bitboard our_b_state = gamestate.state.pieces[us][BISHOP];
        Bitboard our_r_state = gamestate.state.pieces[us][ROOK];
        Bitboard our_q_state = gamestate.state.pieces[us][QUEEN];
        Bitboard our_k_state = gamestate.state.pieces[us][KING];

        u8 castling_rights = gamestate.state.castlingRights;
        Square en_passant_square = gamestate.state.enPassantSquare;

        MoveList move_list{};

        MoveGeneration::generateAllMoves(
            occ,
            empty,
            their_state,
            us,
            our_p_state,
            our_n_state,
            our_b_state,
            our_r_state,
            our_q_state,
            our_k_state,
            castling_rights,
            en_passant_square,
            move_list);

        if (depth == 0)
            return 1;

        if (depth == 1)
        {
            for (int i = 0; i < move_list.count; i++)
            {

                const Move current_move = move_list.moves[i];

                Undo undo;

                if (gamestate.make(current_move, undo))
                {

                    node_count++;
                    gamestate.unmake(current_move, undo);
                }
            }
            return node_count;
        }

        for (int i = 0; i < move_list.count; i++)
        {
            BoardState old_state = gamestate.state;
            Mailbox old_mailbox = gamestate.mailbox;
            const Move current_move = move_list.moves[i];

            Undo undo;

            if (gamestate.make(current_move, undo))
            {
                node_count += perft_recursion(depth - 1);
                gamestate.unmake(current_move, undo);
            }
        }

        return node_count;
    }
};
