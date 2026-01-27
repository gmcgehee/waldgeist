#include <iostream>
#include <cassert>
#include "bitboard.hpp"
#include "utilities.hpp"
#include "types.hpp"

int main()
{

    GameState *game = new GameState();

    //std::string fen_in = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 0";
    std::string fen_in = "r1bqkbnr/pppp2pp/2n2p2/4p3/2B1P3/5N2/PPPP1PPP/RNBQ1RK1 b kq - 1 4";

    game->state = game->loadFromFen(fen_in);
    std::string fen_out = game->exportFen(game->state);

    assert (fen_in == fen_out);

    std::string board_to_print = game->getPrintableBoardState(game->state);
    std::cout << board_to_print << std::endl;

    print_bb(game->state.wPawn);

    //std::cout << pop_lsb(game->state.wPawn) << '\n';

    print_bb(northw(game->state.bPawn));
    
    return 0;
    
}