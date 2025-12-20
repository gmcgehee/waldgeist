#include <iostream>
#include "bitboard.h"

int main() {

    std::string fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
    
    GameState *game = new GameState(fen);

    return 0;
}