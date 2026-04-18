#include <set>
#include <vector>
#include <bit>

#include "types.hpp"
#include "bitboard.hpp"
#include "gamestate.hpp"
#include "movegen.hpp"

class Engine
{
public:
    GameState gamestate;

    Engine()
    {
        gamestate = GameState();
    }

    float search(int depth = 3)
    {

        if (depth == 0)
        {
            return quiesce();
        }

        Bitboard occ = gamestate.getFullState();
        Bitboard empty = ~occ;

        Side us = gamestate.state.sideToPlay;
        Side them = (us == WHITE) ? BLACK : WHITE;

        Bitboard our_state = gamestate.getSideState(us);
        Bitboard their_state = gamestate.getSideState(them);

        Bitboard our_p_state = gamestate.state.pieces[us][PAWN];
        Bitboard our_n_state = gamestate.state.pieces[us][KNIGHT];
        Bitboard our_b_state = gamestate.state.pieces[us][BISHOP];
        Bitboard our_r_state = gamestate.state.pieces[us][ROOK];
        Bitboard our_q_state = gamestate.state.pieces[us][QUEEN];
        Bitboard our_k_state = gamestate.state.pieces[us][KING];

        u8 castling_rights = gamestate.state.castlingRights;
        Square en_passant_square = gamestate.state.enPassantSquare;

        std::vector<Move> move_list = MoveGeneration::generateAllMoves(
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
            en_passant_square);

        if (move_list.empty())
        {
            return 0; // or however you represent "no move"
        }

        Move curr_move;

        Undo undo = gamestate.make(curr_move);
        curr_move = search(depth - 1); // may need to be 'max(curr_best, search())'
        gamestate.unmake(curr_move, undo);

        return curr_move;
    }

    Move minimax(int depth = 1)
    {
        Move move = 0;
        return move;
    }

    float quiesce()
    {

        Bitboard occ = gamestate.getFullState();
        Bitboard empty = ~occ;

        Side us = gamestate.state.sideToPlay;
        Side them = (us == WHITE) ? BLACK : WHITE;

        Bitboard our_state = gamestate.getSideState(us);
        Bitboard their_state = gamestate.getSideState(them);

        Bitboard our_p_state = gamestate.state.pieces[us][PAWN];
        Bitboard our_n_state = gamestate.state.pieces[us][KNIGHT];
        Bitboard our_b_state = gamestate.state.pieces[us][BISHOP];
        Bitboard our_r_state = gamestate.state.pieces[us][ROOK];
        Bitboard our_q_state = gamestate.state.pieces[us][QUEEN];
        Bitboard our_k_state = gamestate.state.pieces[us][KING];

        u8 castling_rights = gamestate.state.castlingRights;
        Square en_passant_square = gamestate.state.enPassantSquare;

        std::vector<Move> captures = MoveGeneration::generateAllCaptures(
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
            en_passant_square);

        if (captures.size() == 0)
        {
            return eval();
        }

        for (Move curr_move : captures)
        {
            Undo undo = gamestate.make(curr_move);
            float score = quiesce(); // may need to be 'max(curr_best, search())'
            gamestate.unmake(curr_move, undo);
        }
    }

    float eval()
    {
        BoardState state = gamestate.state;

        float p_score = 100 * (std::popcount(state.pieces[WHITE][PAWN]) - std::popcount(state.pieces[BLACK][PAWN]));
        float n_score = 300 * (std::popcount(state.pieces[WHITE][KNIGHT]) - std::popcount(state.pieces[BLACK][KNIGHT]));
        float b_score = 320 * (std::popcount(state.pieces[WHITE][BISHOP]) - std::popcount(state.pieces[BLACK][BISHOP]));
        float r_score = 500 * (std::popcount(state.pieces[WHITE][ROOK]) - std::popcount(state.pieces[BLACK][ROOK]));
        float q_score = 900 * (std::popcount(state.pieces[WHITE][QUEEN]) - std::popcount(state.pieces[BLACK][QUEEN]));

        return (p_score + n_score + b_score + r_score + q_score) / 1000;
    }

    unsigned long long perft(int depth)
    {
        unsigned long long node_count = 0;

        Bitboard occ = gamestate.getFullState();
        Bitboard empty = ~occ;

        Side us = gamestate.state.sideToPlay;
        Side them = (us == WHITE) ? BLACK : WHITE;

        Bitboard our_state = gamestate.getSideState(us);
        Bitboard their_state = gamestate.getSideState(them);

        Bitboard our_p_state = gamestate.state.pieces[us][PAWN];
        Bitboard our_n_state = gamestate.state.pieces[us][KNIGHT];
        Bitboard our_b_state = gamestate.state.pieces[us][BISHOP];
        Bitboard our_r_state = gamestate.state.pieces[us][ROOK];
        Bitboard our_q_state = gamestate.state.pieces[us][QUEEN];
        Bitboard our_k_state = gamestate.state.pieces[us][KING];

        u8 castling_rights = gamestate.state.castlingRights;
        Square en_passant_square = gamestate.state.enPassantSquare;

        std::vector<Move> move_list = MoveGeneration::generateAllMoves(
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
            en_passant_square);

        if (move_list.empty())
        {
            return 0; // or however you represent "no move"
        }

        for (Move curr_move : move_list)
        {
            Undo undo = gamestate.make(curr_move);
            node_count += perft(depth - 1); // may need to be 'max(curr_best, search())'
            gamestate.unmake(curr_move, undo);
        }
    }
};
