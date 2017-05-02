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

#include <cstdlib>

#include "serialization.h"

namespace synca {

namespace {
thread_local Buffer* t_buffer = nullptr;
thread_local MemoryAllocator* t_allocator = nullptr;
}

struct DefaultAllocator
{
    static void* alloc(size_t sz)
    {
        void* p = std::malloc(sz);
        VERIFY(p != nullptr, "Allocation failed");
        return p;
    }

    static void dealloc(void* p) noexcept
    {
        std::free(p);
    }
};

Buffer& tlsBuffer()
{
    auto buf = t_buffer;
    if (buf == nullptr) {
        buf = new Buffer(1024 * 1024 * 10); // 10MB
        t_buffer = buf;
    }
    return *buf;
}

MemoryAllocator::MemoryAllocator(Buffer& buffer)
    : buffer_{buffer}
{
    t_allocator = this;
}

MemoryAllocator::~MemoryAllocator()
{
    t_allocator = nullptr;
}

void* MemoryAllocator::alloc(size_t sz) // override
{
    Ptr p = buffer_.data() + offset_;
    size_t diff = sz % c_ptrSize;
    if (diff)
        sz += c_ptrSize - diff;
    VERIFY(
        offset_ + sz <= buffer_.size(), "MemoryAllocator: allocation failed: oversize");
    offset_ += sz;
    std::memset(p, 0, sz);
    return p;
}

void MemoryAllocator::dealloc(void* p) // override
{
    if (p < buffer_.data() || p >= buffer_.data() + buffer_.size())
        DefaultAllocator::dealloc(p);
}

size_t MemoryAllocator::size() const
{
    return offset_;
}

void MemoryAllocator::setSize(size_t size)
{
    offset_ = size;
}

void bufInsertView(Buffer& b, View v)
{
    b.insert(b.end(), v.data, v.data + v.size);
}

View bufToView(Buffer& b)
{
    return {b.data(), b.size()};
}
}

void* operator new(size_t sz)
{
    return synca::t_allocator ? synca::t_allocator->alloc(sz)
                              : synca::DefaultAllocator::alloc(sz);
}

void operator delete(void* p) noexcept
{
    synca::t_allocator ? synca::t_allocator->dealloc(p)
                       : synca::DefaultAllocator::dealloc(p);
}
