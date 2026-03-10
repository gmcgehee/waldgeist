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
#include "rook_blocker_permutations.hpp"
#include "rook_ray_masks.hpp"
#include "rook_attack_tables.hpp"

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

void write_table_to_file(std::vector<std::vector<Bitboard>> bitboards, std::string filename)
{
    std::ofstream outFile(filename);
    if (outFile.is_open())
    {
        for (int i = 0; i < 64; i++)
        {
            std::vector<Bitboard> curr_bitboard = bitboards[i];
            outFile << "{" << std::endl;
            for (int j = 0; j < curr_bitboard.size(); j++)
            {
                outFile << std::format("0x{:X}ULL,", curr_bitboard[j]) << std::endl;
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

Bitboard generate_rook_attacks(Square rook_square, Bitboard occupancy = 0ULL)
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
            Bitboard curr_atack = generate_rook_attacks(sq, curr_blocker_permutation);
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

    //  std::vector<Bitboard> rook_ray_bitboards;
    //  for (int i = 0; i < 64; i++)
    //  {
    //      Bitboard rook_rays = generate_rook_rays(i);
    //      rook_ray_bitboards.push_back(rook_rays);
    //  }

    // write_bitboards_to_file(rook_ray_bitboards, "rook_ray_masks.hpp");

    Bitboard board_state = 0x44F269181625E04CULL;

    Square rook_square = e7;
    Bitboard rook_ray_mask = ROOK_RAY_MASKS[rook_square];
    Bitboard occupancy = board_state;

    int rook_index = (int)_pext_u64(occupancy, rook_ray_mask);
    Bitboard rook_attack = ROOK_ATTACKS[rook_square][rook_index];

    print_bb(rook_ray_mask);
    print_bb(occupancy);
    print_bb(rook_index);
    print_bb(rook_attack);

    print_bb(ROOK_RAY_MASKS[e8]);

    // print_bb(ROOK_BLOCKER_PERMUTATIONS[h8][3040]);

    // print_bb(generate_rook_attacks(h8, ROOK_BLOCKER_PERMUTATIONS[h8][3040]));

    return 0;
}