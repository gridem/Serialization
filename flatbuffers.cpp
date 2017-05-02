#include <benchmark/benchmark.h>
#include <flatbuffers/flatbuffers.h>

#include "flatbuffers_generated.h"

#include "common.h"

void FlatBuffersSerializationInt(benchmark::State& state)
{
    flatbuffers::FlatBufferBuilder fbb;
    while (state.KeepRunning()) {
        fbb.Clear();
        IntBuilder i(fbb);
        i.add_value(0);
        fbb.Finish(i.Finish());
    }
}

void FlatBuffersDeserializationInt(benchmark::State& state)
{
    flatbuffers::FlatBufferBuilder fbb;
    auto offset = CreateInt(fbb, 42);
    fbb.Finish(offset);
    auto* pi = flatbuffers::GetRoot<Int>(fbb.GetBufferPointer());
    VERIFY(pi->value() == 42, "invalid value");
    while (state.KeepRunning()) {
        const Int* pi = flatbuffers::GetRoot<Int>(fbb.GetBufferPointer());
        auto v = pi->value();
        (void)v;
    }
}

void FlatBuffersSerializationString(benchmark::State& state)
{
    flatbuffers::FlatBufferBuilder fbb;
    while (state.KeepRunning()) {
        fbb.Clear();
        auto hello = fbb.CreateString("hello world!");
        StringBuilder s(fbb);
        s.add_value(hello);
        fbb.Finish(s.Finish());
    }
}

void FlatBuffersDeserializationString(benchmark::State& state)
{
    flatbuffers::FlatBufferBuilder fbb;
    auto offset = CreateStringDirect(fbb, "hello world!");
    fbb.Finish(offset);
    auto* pi = flatbuffers::GetRoot<String>(fbb.GetBufferPointer());
    VERIFY(pi->value()->c_str() == std::string("hello world!"), "invalid value");

    size_t size = 0;
    while (state.KeepRunning()) {
        const String* pi = flatbuffers::GetRoot<String>(fbb.GetBufferPointer());
        size += pi->value()->size();
    }
    state.SetBytesProcessed(size);
}

void FlatBuffersSerializationString100(benchmark::State& state)
{
    flatbuffers::FlatBufferBuilder fbb;
    while (state.KeepRunning()) {
        fbb.Clear();
        std::vector<flatbuffers::Offset<flatbuffers::String>> strings;
        for (auto&& s : getGeneratedStrings<100>()) {
            auto so = fbb.CreateString(s);
            strings.push_back(so);
        }
        auto fstrings = fbb.CreateVector(strings);
        StringsBuilder s(fbb);
        s.add_value(fstrings);
        fbb.Finish(s.Finish());
    }
}

void FlatBuffersDeserializationString100(benchmark::State& state)
{
    flatbuffers::FlatBufferBuilder fbb;
    std::vector<flatbuffers::Offset<flatbuffers::String>> strings;
    for (auto&& s : getGeneratedStrings<100>()) {
        strings.push_back(fbb.CreateString(s));
    }
    auto fstrings = fbb.CreateVector(strings);
    StringsBuilder s(fbb);
    s.add_value(fstrings);
    fbb.Finish(s.Finish());

    while (state.KeepRunning()) {
        const Strings* pi = flatbuffers::GetRoot<Strings>(fbb.GetBufferPointer());
        std::vector<std::string> result;
        for (auto* s : *pi->value()) {
            result.push_back(s->str());
        }
    }
}

void FlatBuffersSerializationString1000(benchmark::State& state)
{
    flatbuffers::FlatBufferBuilder fbb;
    while (state.KeepRunning()) {
        fbb.Clear();
        std::vector<flatbuffers::Offset<flatbuffers::String>> strings;
        for (auto&& s : getGeneratedStrings<1000>()) {
            strings.push_back(fbb.CreateString(s));
        }
        auto fstrings = fbb.CreateVector(strings);
        StringsBuilder s(fbb);
        s.add_value(fstrings);
        fbb.Finish(s.Finish());
    }
}

void FlatBuffersDeserializationString1000(benchmark::State& state)
{
    flatbuffers::FlatBufferBuilder fbb;
    std::vector<flatbuffers::Offset<flatbuffers::String>> strings;
    for (auto&& s : getGeneratedStrings<1000>()) {
        strings.push_back(fbb.CreateString(s));
    }
    auto fstrings = fbb.CreateVector(strings);
    StringsBuilder s(fbb);
    s.add_value(fstrings);
    fbb.Finish(s.Finish());

    while (state.KeepRunning()) {
        const Strings* pi = flatbuffers::GetRoot<Strings>(fbb.GetBufferPointer());
        std::vector<std::string> result;
        for (auto* s : *pi->value()) {
            result.push_back(s->str());
        }
    }
}

void FlatBuffersSerializationStringMap(benchmark::State& state)
{
    flatbuffers::FlatBufferBuilder fbb;
    while (state.KeepRunning()) {
        fbb.Clear();
        std::vector<flatbuffers::Offset<flatbuffers::String>> keys;
        std::vector<flatbuffers::Offset<flatbuffers::String>> values;
        for (auto&& s : getGeneratedStringMap<100, 100>()) {
            auto key = fbb.CreateString(s.first);
            auto value = fbb.CreateString(s.second);
            keys.push_back(key);
            values.push_back(value);
        }
        auto fkeys = fbb.CreateVector(keys);
        auto fvalues = fbb.CreateVector(values);
        StringMapBuilder s(fbb);
        s.add_key(fkeys);
        s.add_value(fvalues);
        fbb.Finish(s.Finish());
    }
}

void FlatBuffersSerializationStringMap10(benchmark::State& state)
{
    flatbuffers::FlatBufferBuilder fbb;
    while (state.KeepRunning()) {
        fbb.Clear();
        std::vector<flatbuffers::Offset<flatbuffers::String>> keys;
        std::vector<flatbuffers::Offset<flatbuffers::String>> values;
        for (auto&& s : getGeneratedStringMap<1000, 10>()) {
            auto key = fbb.CreateString(s.first);
            auto value = fbb.CreateString(s.second);
            keys.push_back(key);
            values.push_back(value);
        }
        auto fkeys = fbb.CreateVector(keys);
        auto fvalues = fbb.CreateVector(values);
        StringMapBuilder s(fbb);
        s.add_key(fkeys);
        s.add_value(fvalues);
        fbb.Finish(s.Finish());
    }
}

void FlatBuffersDeserializationStringMap(benchmark::State& state)
{
    flatbuffers::FlatBufferBuilder fbb;
    std::vector<flatbuffers::Offset<flatbuffers::String>> keys;
    std::vector<flatbuffers::Offset<flatbuffers::String>> values;
    for (auto&& s : getGeneratedStringMap<100, 100>()) {
        auto key = fbb.CreateString(s.first);
        auto value = fbb.CreateString(s.second);
        keys.push_back(key);
        values.push_back(value);
    }
    auto fkeys = fbb.CreateVector(keys);
    auto fvalues = fbb.CreateVector(values);
    StringMapBuilder s(fbb);
    s.add_key(fkeys);
    s.add_value(fvalues);
    fbb.Finish(s.Finish());

    while (state.KeepRunning()) {
        const StringMap* map = flatbuffers::GetRoot<StringMap>(fbb.GetBufferPointer());
        std::unordered_map<std::string, std::string> result;
        for (size_t i = 0; i < map->key()->size(); ++i) {
            result.emplace(map->key()->Get(i)->str(), map->value()->Get(i)->str());
        }
    }
}

void FlatBuffersSerializationIntMap(benchmark::State& state)
{
    flatbuffers::FlatBufferBuilder fbb;
    IntMapBuilder s(fbb);
    while (state.KeepRunning()) {
        fbb.Clear();
        std::vector<uint64_t> keys;
        std::vector<uint64_t> values;
        for (auto&& s : getGeneratedIntMap<10000>()) {
            keys.push_back(s.first);
            values.push_back(s.second);
        }
        auto fkeys = fbb.CreateVector(keys);
        auto fvalues = fbb.CreateVector(values);
        IntMapBuilder s(fbb);
        s.add_key(fkeys);
        s.add_value(fvalues);
        fbb.Finish(s.Finish());
    }
}

void FlatBuffersDeserializationIntMap(benchmark::State& state)
{
    flatbuffers::FlatBufferBuilder fbb;
    std::vector<uint64_t> keys;
    std::vector<uint64_t> values;
    for (auto&& s : getGeneratedIntMap<10000>()) {
        keys.push_back(s.first);
        values.push_back(s.second);
    }
    auto fkeys = fbb.CreateVector(keys);
    auto fvalues = fbb.CreateVector(values);
    IntMapBuilder s(fbb);
    s.add_key(fkeys);
    s.add_value(fvalues);
    fbb.Finish(s.Finish());

    size_t sum = 0;
    while (state.KeepRunning()) {
        const IntMap* map = flatbuffers::GetRoot<IntMap>(fbb.GetBufferPointer());
        std::unordered_map<uint64_t, uint64_t> result;
        for (size_t i = 0; i < map->key()->size(); ++i) {
            result.emplace(map->key()->Get(i), map->value()->Get(i));
        }
    }
    state.SetBytesProcessed(sum);
}

BENCHMARK(FlatBuffersSerializationInt);
BENCHMARK(FlatBuffersDeserializationInt);
BENCHMARK(FlatBuffersSerializationString);
BENCHMARK(FlatBuffersDeserializationString);
BENCHMARK(FlatBuffersSerializationString100);
BENCHMARK(FlatBuffersDeserializationString100);
BENCHMARK(FlatBuffersSerializationString1000);
BENCHMARK(FlatBuffersDeserializationString1000);
BENCHMARK(FlatBuffersSerializationStringMap);
BENCHMARK(FlatBuffersSerializationStringMap10);
BENCHMARK(FlatBuffersDeserializationStringMap);
BENCHMARK(FlatBuffersSerializationIntMap);
BENCHMARK(FlatBuffersDeserializationIntMap);
