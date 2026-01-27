#include <iostream>
#include "bitboard.h"
#include "utilities.hpp"
#include "types.hpp"

int main()
{

    // //std::string fen_in = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 0";
    //                    r1bqkbn/pppp21p/1n21p1/54321p2/1/1PPPP1PP/RNBQ1RK/1 b kq - 1 4
    std::string fen_in = "r1bqkbnr/pppp2pp/2n2p2/4p3/2B1P3/5N2/PPPP1PPP/RNBQ1RK1 b kq - 1 4";
    // //std::string fen_in = "r3k2r/ppp1n1pp/2n2p1q/PPN1B1qb/2p2rpP/8/8/R3K3 b Qkq h3 0 1";

    GameState *game = new GameState(fen_in);

    std::string fen_out = game->exportFen(game->state);

    std::cout << "FEN Out: " << fen_out << '\n';

    std::string board_to_print = game->getPrintableBoardState(game->state);

    std::cout << board_to_print << std::endl;

    print_bb(game->state.bBishop);
 

    return 0;

    // currently trying to figure out: a1 is an lshamt of 0, a8 is an lshamt of 56; this is readable to machines, not people.
}