#include "Random.h"

namespace ks {

std::random_device* Random::ms_rd = nullptr;
std::mt19937* Random::ms_mt = nullptr;

void Random::Init()
{
    ms_rd = new std::random_device();
    ms_mt = new std::mt19937(ms_rd->operator()());
}

void Random::Init(uint32_t seed)
{
    ms_rd = new std::random_device();
    ms_mt = new std::mt19937(seed);
}

void Random::Shutdown()
{
    delete ms_mt;
    delete ms_rd;
}

uint8_t Random::U8()
{
    static std::uniform_int_distribution<uint8_t> dist;
    return dist(*ms_mt);
}

}