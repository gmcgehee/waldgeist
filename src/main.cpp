#include <iostream>
#include <cassert>
#include <format>

#include "gamestate.hpp"
#include "bitboard.hpp"
#include "types.hpp"
#include "tables.hpp"
#include "movegen.hpp"

// temporary
#include <chrono>

int main()
{
    GameState *game = new GameState();
    game->state = loadFromFen("r2q1rk1/p1p2pbp/1pn1pnp1/2bP4/2B1P3/2N2N2/PPP1QPPP/R1B2RK1 w - - 0 1");
    std::cout << getPrintableBoardState(game->state) << std::endl;

    Bitboard blackSide = game->getSideState(BLACK);
    Bitboard whiteSide = game->getSideState(WHITE);
    Bitboard occ = whiteSide | blackSide;
    Bitboard empty = ~(occ);
    Bitboard their_state = whiteSide;
    Side us = BLACK;

    // Pawn movegen testing

    std::vector<Move> pawn_captures = MoveGeneration::generatePawnCaptures(their_state, game->state.bPawn, us, game->state.enPassantSquare);
    std::vector<Move> pawn_pushes = MoveGeneration::generatePawnPushes(empty, game->state.bPawn, us);

    for (unsigned int i = 0; i < pawn_captures.size(); i++)
    {
        std::cout << "Capture " << i + 1 << " : " << std::format("{:016b}", pawn_captures[i]) << '\n';
    }

    for (unsigned int i = 0; i < pawn_pushes.size(); i++)
    {
        std::cout << "Push " << i + 1 << " : " << std::format("{:016b}", pawn_pushes[i]) << '\n';
    }

    std::cout << "Total pawn moves: " << pawn_pushes.size() + pawn_captures.size() << std::endl;

    // Knight movegen testing
    std::vector<Move> knight_captures = MoveGeneration::generateKnightCaptures(their_state, game->state.bKnight);
    std::vector<Move> knight_quiets = MoveGeneration::generateKnightQuiets(empty, game->state.bKnight);

    for (unsigned int i = 0; i < knight_captures.size(); i++)
    {

        std::cout << "Knight capture " << i + 1 << " : " << std::format("{:016b}", knight_captures[i]) << '\n';
    }

    for (unsigned int i = 0; i < knight_quiets.size(); i++)
    {
        std::cout << "Knight quiet " << i + 1 << " : " << std::format("{:016b}", knight_quiets[i]) << '\n';
    }

    std::cout << "Total knight moves: " << knight_quiets.size() + knight_captures.size() << std::endl;

    // RAY movegen

    // Bishop movegen testing
    std::vector<Move> bishop_moves = MoveGeneration::generateBishopMoves(occ, empty, their_state, game->state.bBishop);

    for (unsigned int i = 0; i < bishop_moves.size(); i++)
    {

        std::cout << "Bishop move " << i + 1 << " : " << std::format("{:016b}", bishop_moves[i]) << '\n';
    }

    // Rook
    std::vector<Move> rook_moves = MoveGeneration::generateRookMoves(occ, empty, their_state, game->state.bRook);

    for (unsigned int i = 0; i < rook_moves.size(); i++)
    {

        std::cout << "Rook move " << i + 1 << " : " << std::format("{:016b}", rook_moves[i]) << '\n';
    }


    std::cout << "Total rook moves: " << rook_moves.size() << std::endl;

    // Queen
    std::vector<Move> queen_moves = MoveGeneration::generateQueenMoves(occ, empty, their_state, game->state.bQueen);

    for (unsigned int i = 0; i < queen_moves.size(); i++)
    {

        std::cout << "Queen move " << i + 1 << " : " << std::format("{:016b}", queen_moves[i]) << '\n';
    }


    std::cout << "Total queen moves: " << queen_moves.size() << std::endl;

    // King
    std::vector<Move> king_captures = MoveGeneration::generateKingCaptures(their_state, game->state.bKing);
    std::vector<Move> king_quiets = MoveGeneration::generateKingQuiets(empty, game->state.bKing, game->state.castlingRights, us);
    
    for (unsigned int i = 0; i < king_captures.size(); i++)
    {
        std::cout << "King capture " << i + 1 << " : " << std::format("{:016b}", king_captures[i]) << '\n';
    }
    
    for (unsigned int i = 0; i < king_quiets.size(); i++)
    {
        std::cout << "King quiet " << i + 1 << " : " << std::format("{:016b}", king_quiets[i]) << '\n';
    }

    std::cout << "Total king moves: " << king_quiets.size() + king_captures.size() << std::endl;

    return 0;
}