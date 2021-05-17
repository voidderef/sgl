#pragma once

#include <random>

namespace ks {

class Random
{
public:
    static void Init();

    static void Init(uint32_t seed);

    static void Shutdown();

    static uint8_t U8();

private:
    static std::random_device* ms_rd;
    static std::mt19937* ms_mt;

private:
    Random() = default;

    ~Random() = default;
};

}
