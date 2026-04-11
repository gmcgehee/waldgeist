#include <random>
#include <iostream>
#include <bit>
#include <vector>
#include <immintrin.h>
#include <chrono>
#include <functional>
#include <fstream>

#include "../../src/bitboard.hpp"
#include "../../src/types.hpp"
#include "rook_blocker_permutations.hpp"
#include "rook_ray_masks.hpp"
#include "old_rook_attack_tables.hpp"
#include "rook_pext_tables.hpp"
#include "../write_to_file.hpp"

Bitboard generate_bishop_rays(Square rook_square)
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

std::vector<Bitboard> generate_bishop_blockers(Square rook_square)
{
    Bitboard mask = generate_bishop_rays(rook_square);
    int N = std::popcount(mask);
    std::vector<Bitboard> results;

    /*

    leetcode problem:
    Given an unsigned integer from 0-63, generate all possible combinations of rays from that index.

    */

    for (Bitboard i = 0ULL; i <= (1ULL << N) - 1ULL; i++)
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

Bitboard generate_bishop_attacks(Square rook_square, Bitboard occupancy = 0ULL)
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

std::vector<std::vector<Bitboard>> generate_all_rook_attacks()
{

    std::vector<std::vector<Bitboard>> all_attack_list;

    for (Square sq = 0; sq < 64; sq++)
    {
        std::vector<Bitboard> curr_attack_list;
        for (int idx = 0; idx < 4096; idx++)
        {

            Bitboard curr_blocker_permutation = ROOK_BLOCKER_PERMUTATIONS[sq][idx];
            if (curr_blocker_permutation == 0ULL)
            {
                continue;
            }
            Bitboard curr_atack = generate_bishop_attacks(sq, curr_blocker_permutation);
            curr_attack_list.push_back(curr_atack);
        }
        all_attack_list.push_back(curr_attack_list);
    }

    return all_attack_list;
}

int main()
{
    // Bitboard[64][4096] ROOK_ATTACKS =
    /*
    #include "../src/bitboard.hpp"
    #include "../src/types.hpp"

    Bitboard ROOK_ATTACKS[64][4096] =

    */

    
    return 0;
}