/*
 * Copyright 2015 Grigory Demchenko (aka gridem)
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#pragma once

#include <cstdlib>
#include <functional>
#include <string>
#include <unordered_map>
#include <vector>

#define VERIFY(D_cond, D_str)                                                            \
    if (!(D_cond))                                                                       \
    raiseException("Verification failed: " #D_cond ": " D_str)

using Byte = unsigned char;
using Ptr = Byte*;
using PtrArray = Ptr*;
using IntArray = int*;
using Buffer = std::vector<Byte>;

struct View
{
    View() = default;
    View(Buffer& buf)
        : data(buf.data())
        , size(buf.size())
    {
    }
    View(Ptr data_, size_t size_)
        : data{data_}
        , size(size_)
    {
    }

    Ptr data{};
    size_t size{};

    Buffer toBuffer()
    {
        return Buffer(data, data + size);
    }
};

void raiseException(const char* msg);

std::string generateRandomString(size_t length);
Buffer generateRandomBuffer(size_t length);
uint64_t generateRandomInt();

std::vector<std::string> generateRandomStrings(size_t strings, size_t chars);
std::vector<Buffer> generateRandomBuffers(size_t buffers, size_t bytes);
std::unordered_map<std::string, std::string> generateRandomStringMap(
    size_t strings, size_t chars);
std::unordered_map<uint64_t, uint64_t> generateRandomIntMap(size_t ints);

template <int N>
const std::vector<std::string>& getGeneratedStrings()
{
    static const std::vector<std::string> strings = generateRandomStrings(N, N);
    return strings;
}

template <int N>
const std::vector<Buffer>& getGeneratedBuffers()
{
    static const std::vector<Buffer> buffers = generateRandomBuffers(N, N);
    return buffers;
}

template <int N_strings, int N_chars>
const std::unordered_map<std::string, std::string>& getGeneratedStringMap()
{
    static const std::unordered_map<std::string, std::string> map =
        generateRandomStringMap(N_strings, N_chars);
    return map;
}

template <int N>
const std::unordered_map<uint64_t, uint64_t>& getGeneratedIntMap()
{
    static const std::unordered_map<uint64_t, uint64_t> ints = generateRandomIntMap(N);
    return ints;
}
