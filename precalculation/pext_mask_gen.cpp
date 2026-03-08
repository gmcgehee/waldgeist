#include <random>
#include <iostream>
#include <bit>
#include <vector>
#include <immintrin.h>
#include <chrono>
#include <functional>
#include <fstream>

#include "../src/bitboard.hpp"
#include "../src/types.hpp"
#include "pext_rook_masks.hpp"

Bitboard generate_rook_rays(Square rook_square)
{

    Bitboard rook_bb = 0ULL;
    Bitboard curr_ray_bb = 0ULL;
    Bitboard final_rays = 0ULL;

    set_bit(rook_bb, rook_square);

    curr_ray_bb = rook_bb;

    for (int i = 0; i < 8; i++)
    {
        curr_ray_bb = north(curr_ray_bb) & ~RANK_8;
        final_rays |= curr_ray_bb;
    }

    curr_ray_bb = rook_bb;

    for (int i = 0; i < 8; i++)
    {
        curr_ray_bb = south(curr_ray_bb) & ~RANK_1;
        final_rays |= curr_ray_bb;
    }

    curr_ray_bb = rook_bb;

    for (int i = 0; i < 8; i++)
    {
        curr_ray_bb = east(curr_ray_bb) & ~FILE_H;
        final_rays |= curr_ray_bb;
    }

    curr_ray_bb = rook_bb;

    for (int i = 0; i < 8; i++)
    {
        curr_ray_bb = west(curr_ray_bb) & ~FILE_A;
        final_rays |= curr_ray_bb;
    }

    return final_rays;
}

Bitboard generate_bishop_rays(Square bishop_square)
{

    Bitboard bishop_bb = 0ULL;
    Bitboard curr_ray_bb = 0ULL;
    Bitboard final_rays = 0ULL;

    set_bit(bishop_bb, bishop_square);

    curr_ray_bb = bishop_bb;

    for (int i = 0; i < 8; i++)
    {
        curr_ray_bb = (northe(curr_ray_bb) & ~RANK_8) & ~FILE_H;
        final_rays |= curr_ray_bb;
    }

    curr_ray_bb = bishop_bb;

    for (int i = 0; i < 8; i++)
    {
        curr_ray_bb = (southe(curr_ray_bb) & ~RANK_1) & ~FILE_H;
        final_rays |= curr_ray_bb;
    }

    curr_ray_bb = bishop_bb;

    for (int i = 0; i < 8; i++)
    {
        curr_ray_bb = (northw(curr_ray_bb) & ~FILE_A) & ~RANK_8;
        final_rays |= curr_ray_bb;
    }

    curr_ray_bb = bishop_bb;

    for (int i = 0; i < 8; i++)
    {
        curr_ray_bb = (southw(curr_ray_bb) & ~FILE_A) & ~RANK_1;
        final_rays |= curr_ray_bb;
    }

    return final_rays;
}

std::vector<Bitboard> generate_rook_blockers(Square rook_square)
{
    Bitboard mask = generate_rook_rays(rook_square);
    int N = std::popcount(mask);
    std::vector<Bitboard> results;

    /*

    leetcode problem:
    Given an unsigned integer from 0-63, generate all possible combinations of rays from that index.

    */

    for (int i = 0; i <= (1 << N) - 1; i++)
    {
        // as far as I can tell, this is the best way to do this:
        // NOTE: IF RAY MOVE GEN IS EVERY NOT WORKING, CHECK HERE. IT MAY MYSTERIOUSLY BE THIS
        Bitboard ans = _pdep_u64((Bitboard)i, mask);
        // printf("\nResult: %llu", ans);
        // print_bb(ans);
        results.push_back(ans);
    }

    return results;
}

void write_bitboards_to_file(std::vector<Bitboard> bitboards, std::string filename)
{
    std::ofstream outFile(filename);

    if (outFile.is_open())
    {
        for (int i = 0; i < bitboards.size(); i++)
            outFile << std::format("0x{:X}ULL,", bitboards[i]) << std::endl;
    }
    outFile.close();
}

void time_func(std::function<Bitboard(void)> func, int trial_count = 999999)
{

    std::chrono::time_point start = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < trial_count; i++)
    {
        Bitboard ans = func();
    }

    std::chrono::time_point end = std::chrono::high_resolution_clock::now();

    std::cout << "Total duration  : " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start) << std::endl;
    std::cout << "Average duration: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start) / trial_count << std::endl;
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

    // std::vector<Bitboard> a1_rook_blockers = generate_rook_blockers(a1);

    // std::cout << a1_rook_blockers.size() << std::endl;

    // write_bitboards_to_file(a1_rook_blockers, "pext_rook_masks.txt");

    // std::ofstream outFile("pext_rook_masks.txt");
    // if (outFile.is_open())
    // {
    //     for (int i = 0; i < 64; i++)
    //     {

    //         std::vector<Bitboard> blockers = generate_rook_blockers(i);
    //         outFile << "{" << std::endl;
    //         for (int j = 0; j < blockers.size(); j++)
    //         {
    //             outFile << std::format("0x{:X}ULL,", blockers[j]) << std::endl;
    //         }
    //         outFile << "}," << std::endl;
    //     }
    // }

    // print_bb(0x101010101017CULL);

    // int MOVES[2][2] = {
    //     {1, 2},
    //     {3, 4}
    // };

    print_bb(ROOK_BLOCKER_PERMUTATIONS[a2][2047]);

    return 0;
}