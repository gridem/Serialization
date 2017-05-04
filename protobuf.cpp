#include <benchmark/benchmark.h>

#include "common.h"
#include "protobuf.pb.h"

void ProtobufSerializationInt(benchmark::State& state)
{
    std::string buf;
    buf.resize(10 * 1024 * 1024);
    while (state.KeepRunning()) {
        protobuf::Int i;
        i.set_value(0);
        bool serialized = i.SerializeToString(&buf);
        VERIFY(serialized, "Protobuf serialization failed");
    }
}

void ProtobufDeserializationInt(benchmark::State& state)
{
    protobuf::Int i;
    i.set_value(0);
    std::string buf = i.SerializeAsString();
    while (state.KeepRunning()) {
        protobuf::Int j;
        bool parsed = j.ParseFromString(buf);
        VERIFY(parsed, "Protobuf deserialization failed");
    }
}

void ProtobufSerializationString(benchmark::State& state)
{
    std::string buf;
    buf.resize(10 * 1024 * 1024);

    while (state.KeepRunning()) {
        protobuf::String str;
        str.set_value("hello world!");
        bool serialized = str.SerializeToString(&buf);
        VERIFY(serialized, "Protobuf serialization failed");
    }
}

void ProtobufDeserializationString(benchmark::State& state)
{
    protobuf::String s;
    s.set_value("hello world!");
    std::string buf = s.SerializeAsString();

    size_t size = 0;
    while (state.KeepRunning()) {
        protobuf::String str;
        bool parsed = str.ParseFromString(buf);
        VERIFY(parsed, "Protobuf deserialization failed");
        size += str.value().size();
    }
    state.SetBytesProcessed(size);
}

void ProtobufSerializationString100(benchmark::State& state)
{
    std::string buf;
    buf.resize(10 * 1024 * 1024);

    while (state.KeepRunning()) {
        protobuf::Strings strings;
        for (auto&& str : getGeneratedStrings<100>()) {
            strings.add_value(str);
        }
        bool serialized = strings.SerializeToString(&buf);
        VERIFY(serialized, "Protobuf serialization failed");
    }
}

void ProtobufDeserializationString100(benchmark::State& state)
{
    protobuf::Strings s;
    for (auto&& str : getGeneratedStrings<100>()) {
        s.add_value(str);
    }
    std::string buf = s.SerializeAsString();
    while (state.KeepRunning()) {
        protobuf::Strings strings;
        bool parsed = strings.ParseFromString(buf);
        VERIFY(parsed, "Protobuf deserialization failed");
        std::vector<std::string> result;
        for (int i = 0; i < strings.value_size(); ++i) {
            result.push_back(strings.value(i));
        }
    }
}

void ProtobufSerializationString1000(benchmark::State& state)
{
    protobuf::Strings s;
    for (auto&& str : getGeneratedStrings<1000>()) {
        s.add_value(str);
    }
    std::string buf;
    buf.resize(10 * 1024 * 1024);
    while (state.KeepRunning()) {
        bool serialized = s.SerializeToString(&buf);
        VERIFY(serialized, "Protobuf serialization failed");
    }
}

void ProtobufDeserializationString1000(benchmark::State& state)
{
    protobuf::Strings s;
    for (auto&& str : getGeneratedStrings<1000>()) {
        s.add_value(str);
    }
    std::string buf = s.SerializeAsString();
    while (state.KeepRunning()) {
        protobuf::Strings strings;
        bool parsed = strings.ParseFromString(buf);
        VERIFY(parsed, "Protobuf deserialization failed");
        std::vector<std::string> result;
        for (int i = 0; i < strings.value_size(); ++i) {
            result.push_back(strings.value(i));
        }
    }
}

void ProtobufSerializationStringMap(benchmark::State& state)
{
    std::string buf;
    buf.resize(10 * 1024 * 1024);
    while (state.KeepRunning()) {
        protobuf::StringMap s;
        for (auto&& str : getGeneratedStringMap<100, 100>()) {
            s.add_key(str.first);
            s.add_value(str.second);
        }
        bool serialized = s.SerializeToString(&buf);
        VERIFY(serialized, "Protobuf serialization failed");
    }
}

void ProtobufDeserializationStringMap(benchmark::State& state)
{
    protobuf::StringMap s;
    for (auto&& str : getGeneratedStringMap<100, 100>()) {
        s.add_key(str.first);
        s.add_value(str.second);
    }
    std::string buf = s.SerializeAsString();
    while (state.KeepRunning()) {
        protobuf::StringMap map;
        bool parsed = map.ParseFromString(buf);
        VERIFY(parsed, "Protobuf deserialization failed");
        std::unordered_map<std::string, std::string> result;
        for (int i = 0; i < map.key_size(); ++i) {
            result.emplace(map.key(i), map.value(i));
        }
    }
}

void ProtobufSerializationIntMap(benchmark::State& state)
{
    std::string buf;
    buf.resize(10 * 1024 * 1024);
    while (state.KeepRunning()) {
        protobuf::IntMap s;
        for (auto&& str : getGeneratedIntMap<10000>()) {
            s.add_key(str.first);
            s.add_value(str.second);
        }
        bool serialized = s.SerializeToString(&buf);
        VERIFY(serialized, "Protobuf serialization failed");
    }
}

void ProtobufDeserializationIntMap(benchmark::State& state)
{
    protobuf::IntMap s;
    for (auto&& str : getGeneratedIntMap<10000>()) {
        s.add_key(str.first);
        s.add_value(str.second);
    }
    std::string buf = s.SerializeAsString();
    while (state.KeepRunning()) {
        protobuf::IntMap map;
        bool parsed = map.ParseFromString(buf);
        VERIFY(parsed, "Protobuf deserialization failed");
        std::unordered_map<int, int> result;
        for (int i = 0; i < map.key_size(); ++i) {
            result.emplace(map.key(i), map.value(i));
        }
    }
}

void ProtobufSerializationFixedIntMap(benchmark::State& state)
{
    std::string buf;
    buf.resize(10 * 1024 * 1024);
    while (state.KeepRunning()) {
        protobuf::FixedIntMap s;
        for (auto&& str : getGeneratedIntMap<10000>()) {
            s.add_key(str.first);
            s.add_value(str.second);
        }
        bool serialized = s.SerializeToString(&buf);
        VERIFY(serialized, "Protobuf serialization failed");
    }
}

void ProtobufDeserializationFixedIntMap(benchmark::State& state)
{
    protobuf::FixedIntMap s;
    for (auto&& str : getGeneratedIntMap<10000>()) {
        s.add_key(str.first);
        s.add_value(str.second);
    }
    std::string buf = s.SerializeAsString();
    while (state.KeepRunning()) {
        protobuf::FixedIntMap map;
        bool parsed = map.ParseFromString(buf);
        VERIFY(parsed, "Protobuf deserialization failed");
        std::unordered_map<int, int> result;
        for (int i = 0; i < map.key_size(); ++i) {
            result.emplace(map.key(i), map.value(i));
        }
    }
}

BENCHMARK(ProtobufSerializationInt);
BENCHMARK(ProtobufDeserializationInt);
BENCHMARK(ProtobufSerializationString);
BENCHMARK(ProtobufDeserializationString);
BENCHMARK(ProtobufSerializationString100);
BENCHMARK(ProtobufDeserializationString100);
BENCHMARK(ProtobufSerializationString1000);
BENCHMARK(ProtobufDeserializationString1000);
BENCHMARK(ProtobufSerializationStringMap);
BENCHMARK(ProtobufDeserializationStringMap);
BENCHMARK(ProtobufSerializationIntMap);
BENCHMARK(ProtobufDeserializationIntMap);
BENCHMARK(ProtobufSerializationFixedIntMap);
BENCHMARK(ProtobufDeserializationFixedIntMap);
