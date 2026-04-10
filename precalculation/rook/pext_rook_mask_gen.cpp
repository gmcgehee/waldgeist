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
    // std::vector<std::vector<Bitboard>> all_rook_attacks = generate_all_rook_attacks();

    // write_table_to_file(all_rook_attacks, "rook_attack_tables.hpp");

    // Generate PEXT

    std::vector TEMP_ROOK_ATTACKS(64, std::vector<Bitboard>(4096, 0));

    for (int i = 0; i < 64; i++)
    {
        std::vector<Bitboard> blockers = generate_bishop_blockers(i);
        Bitboard curr_rook_ray_mask = ROOK_RAY_MASKS[i];
        for (Bitboard curr_blockers : blockers)
        {
            // Logical flow:
            // Get the proper attackers for the blocker configuration
            // Use PEXT to assign that an index
            // push it forward
            Bitboard curr_attacks = generate_bishop_attacks(i, curr_blockers);

            int index = (int)_pext_u64(curr_blockers, curr_rook_ray_mask);
            // std::cout << index << std::endl;
            TEMP_ROOK_ATTACKS[i][index] = curr_attacks;
        }
    }

    // why can i just not write this !!?!?!?!?!??!?!?!??

    // std::vector<std::vector<Bitboard>> temp_rook_attacks_vec = to_vector2d(TEMP_ROOK_ATTACKS);

    // write_table_to_file(TEMP_ROOK_ATTACKS, "C:\\Users\\spiri\\comp_sci\\projects\\waldgeist\\precalculation\\temp_rook_attacks_attempt.hpp");

    Bitboard board_state = 0x44F269181625E04CULL;

    Square rook_square = a6;

    // print_bb(rook_attack);
    //  print_bb();
    
    for (Square square = 0; square < 64; square++)
    {
        Bitboard rook_ray_mask = ROOK_RAY_MASKS[square];
        Bitboard occupancy = board_state;
        
        // how to use PEXT:
        int rook_index = (int)_pext_u64(occupancy, rook_ray_mask);
        Bitboard rook_attack = ROOK_PEXT_TABLES[square][rook_index];
        Bitboard other_rook_attack = OLD_ROOK_ATTACKS[square][rook_index];
        
        std::cout << "\nNew rook attacks:" << std::endl;
        print_bb(rook_attack);
        
        std::cout << "\nOld rook attacks:" << std::endl;
        
        print_bb(other_rook_attack);
    }
    
    print_bb(board_state);
    return 0;
}