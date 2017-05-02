#include "common.h"

#include <random>

void raiseException(const char* msg)
{
    throw std::runtime_error(msg);
}

std::string generateRandomString(size_t length)
{
    auto randchar = [] {
        const char charset[] = "0123456789"
                               "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                               "abcdefghijklmnopqrstuvwxyz";
        const size_t max_index = (sizeof(charset) - 1);
        return charset[std::rand() % max_index];
    };
    std::string str(length, 0);
    std::generate_n(str.begin(), length, randchar);
    return str;
}

Buffer generateRandomBuffer(size_t length)
{
    auto s = generateRandomString(length);
    return Buffer(s.begin(), s.end());
}

std::vector<std::string> generateRandomStrings(size_t strings, size_t chars)
{
    std::vector<std::string> result;
    for (size_t i = 0; i < strings; ++i) {
        result.push_back(generateRandomString(chars));
    }
    return result;
}

std::vector<Buffer> generateRandomBuffers(size_t buffers, size_t bytes)
{
    std::vector<Buffer> result;
    for (size_t i = 0; i < buffers; ++i) {
        result.push_back(generateRandomBuffer(bytes));
    }
    return result;
}

std::unordered_map<std::string, std::string> generateRandomStringMap(
    size_t strings, size_t chars)
{
    std::unordered_map<std::string, std::string> result;
    for (size_t i = 0; i < strings; ++i) {
        result.emplace(generateRandomString(chars), generateRandomString(chars));
    }
    return result;
}

struct RandomGenerator
{
public:
    RandomGenerator()
    {
        random_.seed(std::mt19937_64::default_seed);
    }

    // This is equivalent to rand().
    uint64_t get()
    {
        return random_();
    }

private:
    std::mt19937_64 random_;
};

uint64_t generateRandomInt()
{
    static RandomGenerator gen;
    return gen.get();
}

std::unordered_map<uint64_t, uint64_t> generateRandomIntMap(size_t ints)
{
    std::unordered_map<uint64_t, uint64_t> result;
    for (size_t i = 0; i < ints; ++i) {
        result.emplace(generateRandomInt(), generateRandomInt());
    }
    return result;
}
