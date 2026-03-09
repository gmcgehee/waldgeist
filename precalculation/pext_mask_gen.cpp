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

void write_table_to_file(std::vector<Bitboard> bitboards, std::string filename)
{
    std::ofstream outFile(filename);
    if (outFile.is_open())
    {
        for (int i = 0; i < 64; i++)
        {

            outFile << "{" << std::endl;
            for (int j = 0; j < bitboards.size(); j++)
            {
                outFile << std::format("0x{:X}ULL,", bitboards[j]) << std::endl;
            }
            outFile << "}," << std::endl;
        }
    }
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

Bitboard generate_rook_attacks(Square rook_square, Bitboard occupancy)
{
    Bitboard rook_bb = 0ULL;
    Bitboard curr_ray_bb = 0ULL;
    Bitboard final_rays = 0ULL;

    set_bit(rook_bb, rook_square);

    curr_ray_bb = rook_bb;

    for (int i = 0; i <= 8; i++)
    {
        curr_ray_bb = north(curr_ray_bb);
        final_rays |= curr_ray_bb;
        if (curr_ray_bb & occupancy)
        {
            break;
        }
    }

    curr_ray_bb = rook_bb;

    for (int i = 0; i <= 8; i++)
    {
        curr_ray_bb = south(curr_ray_bb);
        final_rays |= curr_ray_bb;
        if (curr_ray_bb & occupancy)
        {
            break;
        }
    }

    curr_ray_bb = rook_bb;

    for (int i = 0; i <= 8; i++)
    {
        curr_ray_bb = east(curr_ray_bb);
        final_rays |= curr_ray_bb;
        if (curr_ray_bb & occupancy)
        {
            break;
        }
    }

    curr_ray_bb = rook_bb;

    for (int i = 0; i <= 8; i++)
    {
        curr_ray_bb = west(curr_ray_bb);
        final_rays |= curr_ray_bb;
        if (curr_ray_bb & occupancy)
        {
            break;
        }
    }

    return final_rays;
}

int main()
{
    Bitboard d4_random_rook_perms = ROOK_BLOCKER_PERMUTATIONS[d4][200];
    Bitboard rook_attacks_at_a3 = generate_rook_attacks(d4, d4_random_rook_perms);

    print_bb(d4_random_rook_perms);
    print_bb(rook_attacks_at_a3);

    return 0;
}