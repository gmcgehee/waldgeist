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
#include "bishop_blocker_permutations.hpp"
#include "bishop_ray_masks.hpp"
#include "bishop_pext_tables.hpp"
#include "../write_to_file.hpp"

Bitboard generate_bishop_rays(Square bishop_square)
{

    Bitboard bishop_bb = 0ULL;
    Bitboard curr_ray_bb = 0ULL;
    Bitboard final_rays = 0ULL;

    set_bit(bishop_bb, bishop_square);

    curr_ray_bb = bishop_bb;

    for (int i = 0; i < 8; i++)
    {
        curr_ray_bb = northe(curr_ray_bb) & ~RANK_8 & ~FILE_H;
        final_rays |= curr_ray_bb;
    }

    curr_ray_bb = bishop_bb;

    for (int i = 0; i < 8; i++)
    {
        curr_ray_bb = southe(curr_ray_bb) & ~RANK_1 & ~FILE_H;
        final_rays |= curr_ray_bb;
    }

    curr_ray_bb = bishop_bb;

    for (int i = 0; i < 8; i++)
    {
        curr_ray_bb = southw(curr_ray_bb) & ~RANK_1 & ~FILE_A;
        final_rays |= curr_ray_bb;
    }

    curr_ray_bb = bishop_bb;

    for (int i = 0; i < 8; i++)
    {
        curr_ray_bb = northw(curr_ray_bb) & ~RANK_8 & ~FILE_A;
        final_rays |= curr_ray_bb;
    }

    return final_rays;
}

std::vector<Bitboard> generate_bishop_blockers(Square bishop_square)
{
    Bitboard mask = generate_bishop_rays(bishop_square);
    int N = std::popcount(mask);
    std::vector<Bitboard> results;

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
    Bitboard bishop_bb = 0ULL;
    Bitboard curr_ray_bb = 0ULL;
    Bitboard final_rays = 0ULL;

    set_bit(bishop_bb, rook_square);

    curr_ray_bb = bishop_bb;

    for (int i = 0; i <= 8; i++)
    {
        curr_ray_bb = northe(curr_ray_bb);
        final_rays |= curr_ray_bb;
        if (curr_ray_bb & occupancy)
        {
            break;
        }
    }

    curr_ray_bb = bishop_bb;

    for (int i = 0; i <= 8; i++)
    {
        curr_ray_bb = southe(curr_ray_bb);
        final_rays |= curr_ray_bb;
        if (curr_ray_bb & occupancy)
        {
            break;
        }
    }

    curr_ray_bb = bishop_bb;

    for (int i = 0; i <= 8; i++)
    {
        curr_ray_bb = southw(curr_ray_bb);
        final_rays |= curr_ray_bb;
        if (curr_ray_bb & occupancy)
        {
            break;
        }
    }

    curr_ray_bb = bishop_bb;

    for (int i = 0; i <= 8; i++)
    {
        curr_ray_bb = northw(curr_ray_bb);
        final_rays |= curr_ray_bb;
        if (curr_ray_bb & occupancy)
        {
            break;
        }
    }

    return final_rays;
}

std::vector<std::vector<Bitboard>> generate_all_bishop_attacks()
{

    std::vector<std::vector<Bitboard>> all_attack_list;

    for (Square sq = 0; sq < 64; sq++)
    {
        std::vector<Bitboard> curr_attack_list;
        for (int idx = 0; idx < 4096; idx++)
        {

            Bitboard curr_blocker_permutation = BISHOP_BLOCKER_PERMUTATIONS[sq][idx];
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
    /*
    #include "../../src/bitboard.hpp"
    #include "../../src/types.hpp"

    Bitboard BISHOP_ATTACKS[64][4096] =

    */

    // Generate PEXT

    std::vector TEMP_BISHOP_ATTACKS(64, std::vector<Bitboard>(4096, 0));

    /*
    for (int i = 0; i < 64; i++)
    {
        std::vector<Bitboard> blockers = generate_bishop_blockers(i);
        Bitboard curr_rook_ray_mask = BISHOP_RAY_MASKS[i];
        for (Bitboard curr_blockers : blockers)
        {
            // Logical flow:
            // Get the proper attackers for the blocker configuration
            // Use PEXT to assign that an index
            // push it forward
            Bitboard curr_attacks = generate_bishop_attacks(i, curr_blockers);

            int index = (int)_pext_u64(curr_blockers, curr_rook_ray_mask);
            TEMP_BISHOP_ATTACKS[i][index] = curr_attacks;
        }
    }
    */

    Bitboard board_state = 0x44F269181625E04CULL;

    Square bishop_square = e4;
    
    print_bb(board_state);
    return 0;
}