#include "types.hpp"
#include "bitboard.hpp"
#include <string>


class GameState
{

public:
    BoardState state;

    GameState(std::string fen = "")
    {
        if (fen != "")
        {
            state = loadFromFen(fen);
        }

        else
        {
            // do I need to do anything here?
        }
    }

    void setupDefaultBoard()
    {
        // default FEN : rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1
        // Convenience method
        state = loadFromFen("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    }

    
};
