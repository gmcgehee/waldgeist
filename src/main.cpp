#include <iostream>
#include "bitboard.h"

int main()
{

    //std::string fenIn = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq e3 12 122";
    std::string fenIn = "r3k2r/ppp1n1pp/2n2p1q/PPN1B1qb/2p2rpP/8/8/R3K3 b Qkq h3 0 1";
                      // r3k2r/ppp1n1pp/2n2p1q/PPN1B1qb/2p2rpP/8/8/R3K3 b Qkq h3 0 1

    GameState *game = new GameState(fenIn);

    std::string fenOut = game->exportFen(game->state);

    std::cout << "FEN Out: " << fenOut << '\n';

    std::cout << "Program ran successfully!" << '\n';
    return 0;
}