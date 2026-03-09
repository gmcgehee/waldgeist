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
    game->state = loadFromFen("r3k2r/3NNN2/8/8/8/8/3nbn2/R1R1K2R w KQkq - 0 1");
    std::cout << getPrintableBoardState(game->state) << std::endl;

    Bitboard blackSide = game->getSideState(BLACK);
    Bitboard whiteSide = game->getSideState(WHITE);
    Bitboard empty = ~(whiteSide | blackSide);

    // Pawn movegen testing

    std::vector<Move> pawn_captures = MoveGeneration::generatePawnCaptures(blackSide, game->state.wPawn, WHITE, game->state.enPassantSquare);
    std::vector<Move> pawn_pushes = MoveGeneration::generatePawnPushes(empty, game->state.wPawn, WHITE);

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
    std::vector<Move> knight_captures = MoveGeneration::generateKnightCaptures(blackSide, game->state.wKnight);
    std::vector<Move> knight_quiets = MoveGeneration::generateKnightQuiets(empty, game->state.wKnight);

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

    // King
    std::vector<Move> king_captures = MoveGeneration::generateKingCaptures(blackSide, game->state.wKing);
    std::vector<Move> king_quiets = MoveGeneration::generateKingQuiets(empty, game->state.wKing, game->state.castlingRights, WHITE);
    
    for (unsigned int i = 0; i < king_captures.size(); i++)
    {
        std::cout << "King capture " << i + 1 << " : " << std::format("{:016b}", king_captures[i]) << '\n';
    }
    
    for (unsigned int i = 0; i < king_quiets.size(); i++)
    {
        std::cout << "King quiet " << i + 1 << " : " << std::format("{:016b}", king_quiets[i]) << '\n';
    }

    std::cout << "Total king moves: " << king_quiets.size() + king_captures.size() << std::endl;

    // std::cout << std::format("{:016b}\n", MoveGeneration::convertToMove(g8, e8, TO_NONE, CASTLING));

    return 0;
}