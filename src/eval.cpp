#include "engine.hpp"
#include "tables.hpp"

bool Engine::time_up()
{
    return (std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - start_time).count() > movetime) and (this->movetime != 0);
}

float Engine::get_non_pawn_material(Side us)
{
    // float p_score = 100 * std::popcount(gamestate.state.pieces[us][PAWN]);
    float n_score = 300 * std::popcount(gamestate.state.pieces[us][KNIGHT]);
    float b_score = 320 * std::popcount(gamestate.state.pieces[us][BISHOP]);
    float r_score = 500 * std::popcount(gamestate.state.pieces[us][ROOK]);
    float q_score = 900 * std::popcount(gamestate.state.pieces[us][QUEEN]);
    return n_score + b_score + r_score + q_score;
}

float Engine::eval()
{
    BoardState state = gamestate.state;

    Bitboard occ = gamestate.getFullState();
    Bitboard empty = ~occ;

    Side us = gamestate.state.sideToPlay;
    Side them = (us == WHITE) ? BLACK : WHITE;

    Bitboard their_state = gamestate.getSideState(them);

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
            square_eval += PST[i][inverse_square(sq)];
        }
    }

    for (int i = 0; i < KING - 1; i++)
    {
        Bitboard curr_piece = state.pieces[BLACK][i];
        while (curr_piece)
        {
            Square sq = pop_lsb(curr_piece);
            square_eval -= PST[i][sq];
        }
    }

    // Pawn structure

    // Bitboard white_pawn_occ = state.pieces[WHITE][PAWN];
    // Bitboard black_pawn_occ = state.pieces[BLACK][PAWN];

    // float pawn_structure_score{};

    // Count pawn islands

    // for (int i = 0; i < 8; i++) {

    // }

    // while (white_pawn_occ) {
    //     Square curr_pawn_square = pop_lsb(white_pawn_occ);
    //     Bitboard curr_pawn_bb = 1ULL << curr_pawn_square;

    //     if () {
    //         ;
    //     }
    // }

    // King safety eval

    return (material_eval + square_eval) * (us == WHITE ? 1 : -1);
}