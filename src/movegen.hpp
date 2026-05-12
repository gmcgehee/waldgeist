#pragma once

std::string moveToString(Move move);

inline Move convertToMove(Square destination, Square origin, PieceType promotion_piece = PAWN, SpecialMoveFlag flag = NONSPECIAL);

void generatePawnCaptures(Bitboard their_state, Bitboard our_p_state, Side us, Square en_passant_square, MoveList &move_list);

void generatePawnPushes(Bitboard empty, Bitboard our_p_state, Side us, MoveList &move_list);

void generateKnightCaptures(Bitboard their_state, Bitboard our_n_state, MoveList &move_list);

void generateKnightQuiets(Bitboard empty, Bitboard our_n_state, MoveList &move_list);

void generateBishopMoves(Bitboard occ, Bitboard empty, Bitboard their_state, Bitboard our_b_state, MoveList &move_list);

void generateBishopCaptures(Bitboard occ, Bitboard empty, Bitboard their_state, Bitboard our_b_state, MoveList &move_list);

void generateBishopQuiets(Bitboard occ, Bitboard empty, Bitboard our_b_state, MoveList &move_list);

void generateRookMoves(Bitboard occ, Bitboard empty, Bitboard their_state, Bitboard our_r_state, MoveList &move_list);

void generateRookCaptures(Bitboard occ, Bitboard empty, Bitboard their_state, Bitboard our_r_state, MoveList &move_list);

void generateRookQuiets(Bitboard occ, Bitboard empty, Bitboard our_r_state, MoveList &move_list);

void generateQueenMoves(Bitboard occ, Bitboard empty, Bitboard their_state, Bitboard our_q_state, MoveList &move_list);

void generateQueenCaptures(Bitboard occ, Bitboard empty, Bitboard their_state, Bitboard our_q_state, MoveList &move_list);

void generateQueenQuiets(Bitboard occ, Bitboard empty, Bitboard our_q_state, MoveList &move_list);

void generateKingCaptures(Bitboard their_state, Bitboard our_k_state, MoveList &move_list);

void generateKingQuiets(Bitboard empty, Bitboard our_k_state, u8 castling_rights, Side us, MoveList &move_list);

// void generateAllMoves(
//     Bitboard occ, Bitboard empty, Bitboard their_state, Side us,
//     Bitboard our_p_state, Bitboard our_n_state, Bitboard our_b_state,
//     Bitboard our_r_state, Bitboard our_q_state, Bitboard our_k_state,
//     u8 castling_rights, Square en_passant_square, MoveList &move_list)
// {

//     generatePawnCaptures(their_state, our_p_state, us, en_passant_square, move_list);
//     generateKnightCaptures(their_state, our_n_state, move_list);
//     generatePawnPushes(empty, our_p_state, us, move_list);
//     generateKnightQuiets(empty, our_n_state, move_list);
//     generateBishopMoves(occ, empty, their_state, our_b_state, move_list);
//     generateRookMoves(occ, empty, their_state, our_r_state, move_list);
//     generateQueenMoves(occ, empty, their_state, our_q_state, move_list);
//     generateKingCaptures(their_state, our_k_state, move_list);
//     generateKingQuiets(empty, our_k_state, castling_rights, us, move_list);
// }

// void generateAllCaptures(
//     Bitboard occ, Bitboard empty, Bitboard their_state, Side us,
//     Bitboard our_p_state, Bitboard our_n_state, Bitboard our_b_state,
//     Bitboard our_r_state, Bitboard our_q_state, Bitboard our_k_state,
//     u8 castling_rights, Square en_passant_square, MoveList &move_list)
// {

//     generatePawnCaptures(their_state, our_p_state, us, en_passant_square, move_list);
//     generateKnightCaptures(their_state, our_n_state, move_list);
//     generateBishopCaptures(occ, empty, their_state, our_b_state, move_list);
//     generateRookCaptures(occ, empty, their_state, our_r_state, move_list);
//     generateQueenCaptures(occ, empty, their_state, our_q_state, move_list);
//     generateKingCaptures(their_state, our_k_state, move_list);
// }

// void generateAllQuiets(const GameState &gamestate, MoveList &move_list)
// {
//     Bitboard occ = gamestate.getFullState();
//     Bitboard empty = ~occ;

//     Side us = gamestate.state.sideToPlay;
//     Side them = us == WHITE ? BLACK : WHITE;

//     generatePawnPushes(empty, gamestate.state.pieces[us][PAWN], us, move_list);
//     generateKnightQuiets(empty, gamestate.state.pieces[us][KNIGHT], move_list);
//     generateBishopQuiets(occ, empty, gamestate.state.pieces[us][BISHOP], move_list);
//     generateRookQuiets(occ, empty, gamestate.state.pieces[us][ROOK], move_list);
//     generateQueenQuiets(occ, empty, gamestate.state.pieces[us][QUEEN], move_list);
//     generateKingQuiets(empty, gamestate.state.pieces[us][KING], gamestate.state.castlingRights, us, move_list);
// }

// void generateAllCaptures(const GameState &gamestate, MoveList &move_list)
// {
//     Bitboard occ = gamestate.getFullState();
//     Bitboard empty = ~occ;

//     Side us = gamestate.state.sideToPlay;
//     Side them = us == WHITE ? BLACK : WHITE;
//     Bitboard their_state = gamestate.getSideState(them);

//     generatePawnCaptures(their_state, gamestate.state.pieces[us][PAWN], us, gamestate.state.enPassantSquare, move_list);
//     generateKnightCaptures(empty, gamestate.state.pieces[us][KNIGHT], move_list);
//     generateBishopCaptures(occ, empty, their_state, gamestate.state.pieces[us][BISHOP], move_list);
//     generateRookCaptures(occ, empty, their_state, gamestate.state.pieces[us][ROOK], move_list);
//     generateQueenCaptures(occ, empty, their_state, gamestate.state.pieces[us][QUEEN], move_list);
//     generateKingCaptures(their_state, gamestate.state.pieces[us][KING], move_list);
// }

// void generateAllMoves(const GameState &gamestate, MoveList &move_list)
// {
//     generateAllCaptures(gamestate, move_list);
//     generateAllQuiets(gamestate, move_list);
// }
