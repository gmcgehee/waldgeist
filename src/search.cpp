#include <iostream>
#include <thread>
#include <atomic>

#include "engine.hpp"
#include "tables.hpp"
#include "movegen.hpp"

/************************\

Move Generation and Scoring

\************************/

void Engine::generateAllQuiets(const GameState &gamestate, MoveList &move_list)
{
    Bitboard occ = gamestate.getFullState();
    Bitboard empty = ~occ;

    Side us = gamestate.state.sideToPlay;
    Side them = us == WHITE ? BLACK : WHITE;

    generatePawnPushes(empty, gamestate.state.pieces[us][PAWN], us, move_list);
    generateKnightQuiets(empty, gamestate.state.pieces[us][KNIGHT], move_list);
    generateBishopQuiets(occ, empty, gamestate.state.pieces[us][BISHOP], move_list);
    generateRookQuiets(occ, empty, gamestate.state.pieces[us][ROOK], move_list);
    generateQueenQuiets(occ, empty, gamestate.state.pieces[us][QUEEN], move_list);
    generateKingQuiets(empty, gamestate.state.pieces[us][KING], gamestate.state.castlingRights, us, move_list);
}

void Engine::generateAllCaptures(const GameState &gamestate, MoveList &move_list)
{
    Bitboard occ = gamestate.getFullState();
    Bitboard empty = ~occ;

    Side us = gamestate.state.sideToPlay;
    Side them = us == WHITE ? BLACK : WHITE;
    Bitboard their_state = gamestate.getSideState(them);

    generatePawnCaptures(their_state, gamestate.state.pieces[us][PAWN], us, gamestate.state.enPassantSquare, move_list);
    generateKnightCaptures(their_state, gamestate.state.pieces[us][KNIGHT], move_list);
    generateBishopCaptures(occ, empty, their_state, gamestate.state.pieces[us][BISHOP], move_list);
    generateRookCaptures(occ, empty, their_state, gamestate.state.pieces[us][ROOK], move_list);
    generateQueenCaptures(occ, empty, their_state, gamestate.state.pieces[us][QUEEN], move_list);
    generateKingCaptures(their_state, gamestate.state.pieces[us][KING], move_list);
}

void Engine::generateAllMoves(const GameState &gamestate, MoveList &move_list)
{
    generateAllCaptures(gamestate, move_list);
    generateAllQuiets(gamestate, move_list);
}

float Engine::get_move_score(Move move)
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

std::array<float, 256> Engine::get_all_move_scores(MoveList &move_list)
{

    std::array<float, 256> scores;

    for (int i = 0; i < move_list.count; i++)
    {
        scores[i] = get_move_score(move_list.moves[i]);
        if (scores[i] == 0)
            break;
    }

    return scores;
}

void Engine::swap_best_move_to_index(MoveList &move_list, int idx, std::array<float, 256> scores)
{
    int best_score_index{idx};
    float best_move_score{-1000000};

    for (int j = idx + 1; j < move_list.count; j++)
    {
        if (scores[j] > best_move_score)
        {
            best_move_score = scores[j];
            best_score_index = j;
        }
    }

    std::swap(move_list.moves[idx], move_list.moves[best_score_index]);
    std::swap(scores[idx], scores[best_score_index]);
}

/************************\

          Search

\************************/

std::pair<float, Move> Engine::iterative_search(int max_depth, int movetime)
{
    start_time = std::chrono::steady_clock::now();
    this->movetime = movetime;

    std::pair<float, Move> best_move{};
    std::pair<float, Move> curr_best_move{};

    for (int depth = 1; depth <= max_depth; depth++)
    {
        curr_best_move = alpha_beta(depth);

        if (time_up())
        {
            // auto end = std::chrono::steady_clock::now();

            // auto duration = end - start_time;
            // double milliseconds = std::chrono::duration<double, std::milli>(duration).count();

            // std::cout << milliseconds << " ms\n";
            this->movetime = 0;
            return best_move;
        }

        std::cout << "info depth " << depth
                  << " score cp " << static_cast<int>(best_move.second)
                  << '\n';

        best_move = curr_best_move;
    }

    this->movetime = 0;
    return best_move;
}

std::pair<float, Move> Engine::alpha_beta(int depth)
{

    float alpha = -1'000'000;
    float beta = 1'000'000;

    float curr_score{};
    Move best_move{};

    Side us = gamestate.state.sideToPlay;
    Side them = (us == WHITE) ? BLACK : WHITE;

    MoveList move_list{};

    generateAllMoves(gamestate, move_list);

    int legal_moves = 0;

    std::array<float, 256> scores = get_all_move_scores(move_list);

    for (int i = 0; i < move_list.count; i++)
    {

        if (time_up())
            return std::pair<float, Move>{alpha, best_move == 0 ? move_list.moves[0] : best_move};

        swap_best_move_to_index(move_list, i, scores);

        Move curr_move = move_list.moves[i];

        Undo undo;

        if (gamestate.make(curr_move, undo))
        {

            legal_moves++;
            curr_score = -alpha_beta_recursion(depth - 1, -beta, -alpha, 1);

            gamestate.unmake(curr_move, undo);

            if (curr_score > alpha)
            {
                alpha = curr_score;
                best_move = curr_move;

                if (alpha >= beta)
                {
                    break;
                }
            }
        }
    }

    if (legal_moves == 0)
    {
        if (gamestate.isSquareThreatened(__builtin_ctzll(gamestate.state.pieces[us][KING]), them))
            return std::pair<float, Move>{-MATE_SCORE, best_move};
        else
        {
            return std::pair<float, Move>{0, best_move};
        }
    }

    return std::pair<float, Move>{alpha, best_move};
}

float Engine::alpha_beta_recursion(int depth, float alpha, float beta, int ply, bool is_null_search)
{

    if (depth == 0)
    {
        return quiesce(alpha, beta); // TODO
        // return eval();
    }

    float curr_score{};

    // Bitboard occ = gamestate.getFullState();
    // Bitboard empty = ~occ;

    Side us = gamestate.state.sideToPlay;
    Side them = (us == WHITE) ? BLACK : WHITE;

    int R = 3 + depth / 6;

    if (!gamestate.isSquareThreatened(__builtin_ctzll(gamestate.state.pieces[us][KING]), them) && get_non_pawn_material(us) > 0 && depth >= R + 1 && !is_null_search)
    {
        if (eval() > beta)
        {
            float temp_score{};
            Square en_passant_square = gamestate.state.enPassantSquare;
            gamestate.make_null();
            temp_score = -alpha_beta_recursion(depth - 1 - R, -beta, -beta + 1, ply + 1, true);
            gamestate.unmake_null(en_passant_square);

            if (temp_score >= beta)
                return beta;
        }
    }

    MoveList move_list{};

    generateAllMoves(gamestate, move_list);

    int legal_moves = 0;

    std::array<float, 256> scores = get_all_move_scores(move_list);

    for (int i = 0; i < move_list.count; i++)
    {

        if (time_up())
            return alpha;

        swap_best_move_to_index(move_list, i, scores);

        Move curr_move = move_list.moves[i];

        Undo undo;

        if (gamestate.make(curr_move, undo))
        {
            legal_moves++;
            curr_score = -alpha_beta_recursion(depth - 1, -beta, -alpha, ply + 1, is_null_search);

            gamestate.unmake(curr_move, undo);

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
            return -MATE_SCORE + ply;
        else
        {
            return 0;
        }
    }

    return alpha;
}

float Engine::quiesce(float alpha, float beta)
{

    float stand_pat = eval();

    if (stand_pat >= beta)
        return stand_pat;

    if (stand_pat > alpha)
        alpha = stand_pat;

    float max_score = stand_pat;
    float curr_score{};

    MoveList capture_list{};

    generateAllCaptures(gamestate, capture_list);

    if (capture_list.count == 0)
    {
        return stand_pat;
    }

    int legal_captures{};

    std::array<float, 256> scores = get_all_move_scores(capture_list);

    for (int i = 0; i < capture_list.count; i++)
    {
        swap_best_move_to_index(capture_list, i, scores);

        Move curr_move = capture_list.moves[i];

        Undo undo;

        if (gamestate.make(curr_move, undo))
        {
            legal_captures++;
            curr_score = -quiesce(-beta, -alpha);
            gamestate.unmake(curr_move, undo);

            if (time_up()) return 0;

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

    return alpha;
}