#include <random>
#include <iostream>
#include <bit>
#include <vector>
#include <immintrin.h>
#include <chrono>
#include <functional>
#include <fstream>
#include <stdio.h>

#include "../src/bitboard.hpp"
#include "../src/types.hpp"
#include "rook/rook_blocker_permutations.hpp"
#include "rook/rook_ray_masks.hpp"
#include "rook/old_rook_attack_tables.hpp"
#include "rook/rook_pext_tables.hpp"

void write_table_to_file(std::vector<std::vector<Bitboard>> bitboards, std::string filename)
{
    std::ofstream outFile;

    outFile.open(filename);

    if (outFile.is_open())
    {
        for (int i = 0; i < 64; i++)
        {
            std::vector<Bitboard> curr_bitboard = bitboards[i];
            outFile << "\n{" << std::endl;
            for (int j = 0; j < curr_bitboard.size(); j++)
            {
                outFile << std::format("0x{:X}ULL,", curr_bitboard[j]) << std::endl;
            }
            outFile << "}," << std::endl;
        }
    }
    outFile.close();
}

void write_list_to_file(std::vector<Bitboard> bitboards, std::string filename)
{
    std::ofstream outFile;

    outFile.open(filename);

    if (outFile.is_open())
    {
        outFile << "\n{" << std::endl;
        for (int i = 0; i < 64; i++)
        {
            Bitboard curr_bitboard = bitboards[i];
            outFile << std::format("0x{:X}ULL,", curr_bitboard) << std::endl;
        }
        outFile << "}," << std::endl;
    }
    outFile.close();
}

void pretty_print_vector(std::vector<Bitboard> bitboards)
{
    printf("\nStart");
    for (int i = 0; i < bitboards.size(); i++)
    {
        printf("\nValue: %llu", bitboards[i]);
    }
    printf("\nEnd");
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
