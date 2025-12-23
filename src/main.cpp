#include <iostream>
#include "bitboard.h"

int main() {

    std::string fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 12 122";
    
    GameState *game = new GameState(fen);

    // game->printBoardState(BoardState game.state); // idk how to make this work

    // int test_square = game->squareToNumber("g2");
    // std::cout << "Test square: " << test_square << std::endl;

    std::cout << "Program ran successfully!" << '\n';

    return 0;
}