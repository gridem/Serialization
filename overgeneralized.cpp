#include <benchmark/benchmark.h>

#include "serialization.h"

struct Int
{
    int value{};
};

struct String
{
    std::string str = "hello world!";
};

void OvergeneralizedSerializationInt(benchmark::State& state)
{
    Int i;
    while (state.KeepRunning()) {
        synca::serialize(i);
    }
}

void OvergeneralizedDeserializationInt(benchmark::State& state)
{
    View result = synca::serialize(Int{});
    while (state.KeepRunning()) {
        auto value = synca::deserialize<Int>(result).value;
        (void)value;
    }
}

void OvergeneralizedSerializationString(benchmark::State& state)
{
    String s;
    while (state.KeepRunning()) {
        synca::serialize(s);
    }
}

void OvergeneralizedDeserializationString(benchmark::State& state)
{
    View result = synca::serialize(String{});
    size_t size = 0;
    while (state.KeepRunning()) {
        auto& str = synca::deserialize<String>(result);
        size += str.str.size();
    }
    state.SetBytesProcessed(size);
}

void OvergeneralizedSerializationString100(benchmark::State& state)
{
    while (state.KeepRunning()) {
        synca::serialize(getGeneratedStrings<100>());
    }
}

void OvergeneralizedSerializationVectorString100(benchmark::State& state)
{
    while (state.KeepRunning()) {
        synca::serializeVector(getGeneratedStrings<100>());
    }
}

void OvergeneralizedSerializationVectorBuffer100(benchmark::State& state)
{
    while (state.KeepRunning()) {
        synca::serializeVector(getGeneratedBuffers<100>());
    }
}

void OvergeneralizedDeserializationString100(benchmark::State& state)
{
    View serialized = synca::serialize(getGeneratedStrings<100>());
    while (state.KeepRunning()) {
        synca::deserialize<std::vector<std::string>>(serialized);
    }
}

void OvergeneralizedSerializationString1000(benchmark::State& state)
{
    while (state.KeepRunning()) {
        synca::serialize(getGeneratedStrings<1000>());
    }
}

void OvergeneralizedSerializationVectorString1000(benchmark::State& state)
{
    while (state.KeepRunning()) {
        synca::serializeVector(getGeneratedStrings<1000>());
    }
}

void OvergeneralizedDeserializationString1000(benchmark::State& state)
{
    View serialized = synca::serialize(getGeneratedStrings<1000>());
    while (state.KeepRunning()) {
        synca::deserialize<std::vector<std::string>>(serialized);
    }
}

void OvergeneralizedSerializationStringMap(benchmark::State& state)
{
    while (state.KeepRunning()) {
        synca::serialize(getGeneratedStringMap<100, 100>());
    }
}

void OvergeneralizedDeserializationStringMap(benchmark::State& state)
{
    View serialized = synca::serialize(getGeneratedStringMap<100, 100>());
    while (state.KeepRunning()) {
        synca::deserialize<std::unordered_map<std::string, std::string>>(serialized);
    }
}

void OvergeneralizedSerializationStringMap10(benchmark::State& state)
{
    while (state.KeepRunning()) {
        synca::serialize(getGeneratedStringMap<1000, 100>());
    }
}

void OvergeneralizedSerializationIntMap(benchmark::State& state)
{
    while (state.KeepRunning()) {
        synca::serialize(getGeneratedIntMap<10000>());
    }
}

void OvergeneralizedDeserializationIntMap(benchmark::State& state)
{
    View serialized = synca::serialize(getGeneratedIntMap<10000>());
    while (state.KeepRunning()) {
        synca::deserialize<std::unordered_map<uint64_t, uint64_t>>(serialized);
    }
}

BENCHMARK(OvergeneralizedSerializationInt);
BENCHMARK(OvergeneralizedDeserializationInt);
BENCHMARK(OvergeneralizedSerializationString);
BENCHMARK(OvergeneralizedDeserializationString);
BENCHMARK(OvergeneralizedSerializationString100);
BENCHMARK(OvergeneralizedSerializationVectorString100);
BENCHMARK(OvergeneralizedSerializationVectorBuffer100);
BENCHMARK(OvergeneralizedDeserializationString100);
BENCHMARK(OvergeneralizedSerializationVectorString1000);
BENCHMARK(OvergeneralizedSerializationString1000);
BENCHMARK(OvergeneralizedDeserializationString1000);
BENCHMARK(OvergeneralizedSerializationStringMap);
BENCHMARK(OvergeneralizedDeserializationStringMap);
BENCHMARK(OvergeneralizedSerializationStringMap10);
BENCHMARK(OvergeneralizedSerializationIntMap);
BENCHMARK(OvergeneralizedDeserializationIntMap);
