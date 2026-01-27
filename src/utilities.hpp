#include "types.hpp"
#include <string>
#include <iostream>

#ifndef UTILITIES_HPP
#define UTILITIES_HPP

namespace util {

    inline Bitboard north(Bitboard bb) {
        return (bb << 8);
    }

    inline Bitboard south(Bitboard bb) {
        return (bb >> 8);
    }

    inline Bitboard west(Bitboard bb) {
        return (bb << 1);
    }

    inline Bitboard east(Bitboard bb) {
        return (bb >> 1);
    }
    
    int get_bit(Bitboard &bb, Square sq) {
        Bitboard bit = bb & (1ULL << sq);
        return bit ? 1 : 0;
    }
    
    void set_bit(Bitboard &bb, Square sq) {
        get_bit(bb, sq) ? bb ^= (1ULL << sq) : 0;
    }
    // Modifies the bb you're using
    Bitboard pop_lsb(Bitboard &bb) {
        bb &= bb - 1;
        return bb; // i believe this is supposed to remove the most significant bit but put it in a new thing
    }

    void print_bb(Bitboard &bb) {

        // std::string bb_str = "";

        for (int rank = 0; rank < 8; rank++) {
            std::cout << "\n " << std::to_string(8 - rank);
            for (int file = 0; file < 8; file++) {
                
                std::cout << ' ' << std::to_string(get_bit(bb, 8 * rank + file));
            }
        }

        std::cout << "\n   a b c d e f g h\n";

    }

}

#endif