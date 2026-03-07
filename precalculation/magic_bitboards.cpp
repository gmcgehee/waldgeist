#include <random>
#include <iostream>
#include <bit>
#include <vector>

#include "../src/bitboard.hpp"
#include "../src/types.hpp"


Bitboard generate_rook_rays(Square rook_square) {

    Bitboard rook_bb = 0ULL;
    Bitboard curr_ray_bb = 0ULL;
    Bitboard final_rays = 0ULL;

    set_bit(rook_bb, rook_square);

    curr_ray_bb = rook_bb;


    for (int i = 0; i < 8; i++) {
        curr_ray_bb = north(curr_ray_bb) & ~RANK_8;
        final_rays |= curr_ray_bb;
    }

    curr_ray_bb = rook_bb;

    for (int i = 0; i < 8; i++) {
        curr_ray_bb = south(curr_ray_bb) & ~RANK_1;
        final_rays |= curr_ray_bb;
    }

    curr_ray_bb = rook_bb;

    for (int i = 0; i < 8; i++) {
        curr_ray_bb = east(curr_ray_bb) & ~FILE_H;
        final_rays |= curr_ray_bb;
    }

    curr_ray_bb = rook_bb;

    for (int i = 0; i < 8; i++) {
        curr_ray_bb = west(curr_ray_bb) & ~FILE_A;
        final_rays |= curr_ray_bb;
    }

    return final_rays;

}



Bitboard generate_bishop_rays(Square bishop_square) {

    Bitboard bishop_bb = 0ULL;
    Bitboard curr_ray_bb = 0ULL;
    Bitboard final_rays = 0ULL;

    set_bit(bishop_bb, bishop_square);

    curr_ray_bb = bishop_bb;


    for (int i = 0; i < 8; i++) {
        curr_ray_bb = (northe(curr_ray_bb) & ~RANK_8) & ~FILE_H;
        final_rays |= curr_ray_bb;
    }

    curr_ray_bb = bishop_bb;

    for (int i = 0; i < 8; i++) {
        curr_ray_bb = (southe(curr_ray_bb) & ~RANK_1) & ~FILE_H;
        final_rays |= curr_ray_bb;
    }

    curr_ray_bb = bishop_bb;

    for (int i = 0; i < 8; i++) {
        curr_ray_bb = (northw(curr_ray_bb) & ~FILE_A) & ~ RANK_8;
        final_rays |= curr_ray_bb;
    }

    curr_ray_bb = bishop_bb;

    for (int i = 0; i < 8; i++) {
        curr_ray_bb = (southw(curr_ray_bb) & ~FILE_A) & ~RANK_1;
        final_rays |= curr_ray_bb;
    }

    return final_rays;

}


std::vector<Bitboard> generate_rook_blockers(Square rook_square) {
    Bitboard mask = generate_rook_rays(rook_square);
    int N = std::popcount(mask);

    /*
    
    leetcode problem:
    Given an unsigned 64-bit integer with 
    
    */

    for (int i = 0; i < (1 << N) - 1; i++) {

    }
}



int main()
{

    // Bitboard bishop_a1 = 0x0040201008040200ULL;
    // Bitboard magic_num = 0xffedf9fd7cfcffffULL;
    // Bitboard pre_postmask = 0x8040201008040200ULL;

    // print_bb(bishop_a1);
    // print_bb(magic_num);
    // print_bb(pre_postmask);

    // print_bb((bishop_a1 & pre_postmask)* magic_num);

    // Square sq = e4;

    // Bitboard rook_rays = generate_rook_rays(sq);

    // print_bb(rook_rays);

    // Bitboard bishop_rays = generate_bishop_rays(sq);

    // print_bb(bishop_rays);

    // std::cout << "---------- Test section ----------" << std::endl;

    // for (int i = 0; i < 64; i++) {
    //     print_bb(generate_bishop_rays(i));
    // }

    return 0;
}