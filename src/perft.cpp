#include <iostream>

#include "bitboard.hpp"
#include "engine.hpp"
#include "debug.hpp"
#include "types.hpp"
#include "movegen.hpp"

unsigned long long Engine::perft(int depth)
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

    generateAllMoves(gamestate, move_list);

    for (int i = 0; i < move_list.count; i++)
    {
        BoardState old_state = gamestate.state;
        Mailbox old_mailbox = gamestate.mailbox;
        const Move current_move = move_list.moves[i];

        Undo undo;

        std::vector<GameState> gamestate_list(THREAD_COUNT);

        for (size_t thread_i = 0; thread_i < THREAD_COUNT; thread_i++)
        {
            gamestate_list.push_back(GameState(gamestate));
        }

        if (gamestate.make(current_move, undo))
        {
            unsigned long long curr_move_node_count = perft_recursion(gamestate, depth - 1);
            std::cout << moveToString(current_move) << ": " << curr_move_node_count << '\n';
            node_count += curr_move_node_count;
            gamestate.unmake(current_move, undo);
        }
    }
    return node_count;
}

unsigned long long Engine::perft_recursion(GameState& node_gamestate, int depth)
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

    generateAllMoves(gamestate, move_list);

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
            node_count += perft_recursion(gamestate, depth - 1);
            gamestate.unmake(current_move, undo);
        }
    }

    return node_count;
}