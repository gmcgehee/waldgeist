#include <iostream>
#include "bitboard.h"

int main() {

    std::string fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
    std::string invalid_fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR f KQkq - 0 1";
    
    GameState *game = new GameState(invalid_fen);

    // game->printBoardState(BoardState game.state); // idk how to make this work

    return 0;
}