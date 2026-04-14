#include <set>
#include <vector>

#include "types.hpp"
#include "bitboard.hpp"
#include "gamestate.hpp"
#include "movegen.hpp"

class Engine {
    public:
        GameState gamestate;
    
    Move search(int depth = 1) {
        Bitboard occ = gamestate.getFullState();
        Bitboard empty = ~occ;
        Side us = gamestate.state.sideToPlay;
        Side them  = (us == WHITE) ? BLACK : WHITE;
        Bitboard our_state = gamestate.getSideState(us);
        Bitboard their_state = gamestate.getSideState(them);


        // std::vector<Move> move_list = MoveGeneration::generateAllMoves(occ, empty, their_state, us, );
    }

    Move minimax(int depth = 1) {

    }

    float eval() {

    }

};
