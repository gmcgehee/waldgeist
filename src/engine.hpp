#pragma once

#include <chrono>
#include <cstring>

#include "gamestate.hpp"
#include "types.hpp"

class Engine
{

private:
    // the idea is that when the best move is found for a certain ply/depth (?), pv_table at that is set for the best move
    Move pv_table[256]{};
    int movetime{};
    std::chrono::steady_clock::time_point start_time{};

public:
    GameState gamestate;

    Engine()
    {
        gamestate = GameState();
    }

    void reset()
    {
        std::memset(pv_table, 0, 256 * sizeof(pv_table[0]));
    }

    float get_move_score(Move move);

    void generateAllQuiets(const GameState &gamestate, MoveList &move_list);

    void generateAllCaptures(const GameState &gamestate, MoveList &move_list);

    void generateAllMoves(const GameState &gamestate, MoveList &move_list);
    
    bool time_up();

    std::pair<float, Move> iterative_search(int max_depth, int movetime = 0);

    std::pair<float, Move> alpha_beta(int depth);

    float alpha_beta_recursion(int depth, float alpha, float beta, int ply, bool is_null_search = false);

    float quiesce(float alpha, float beta);

    float get_non_pawn_material(Side us);

    float eval();

    unsigned long long perft(int depth);

    unsigned long long perft_recursion(int depth);
    
};
