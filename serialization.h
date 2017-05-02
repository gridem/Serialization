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

#include "common.h"

namespace synca {

Buffer& tlsBuffer();

constexpr size_t c_ptrSize = sizeof(Ptr);
constexpr size_t c_intSize = sizeof(int);

struct MemoryAllocator
{
    MemoryAllocator(Buffer& buffer);
    ~MemoryAllocator();

    void* alloc(size_t sz);
    void dealloc(void* p);
    size_t size() const;
    void setSize(size_t size);

private:
    size_t offset_ = 0;
    Buffer& buffer_;
};

template <typename T>
View serialize(const T& obj, Buffer& buffer, size_t checkedSize = 0)
{
    View v{buffer.data(), 0};
    size_t total;
    {
        MemoryAllocator a(buffer);
        new T{obj};
        v.size = a.size();
        new T{obj};
        total = a.size();
    }

    VERIFY(v.size % c_ptrSize == 0, "Unaligned data");
    VERIFY(v.size * 2 == total, "Unpredictable copy constructor");

    int pMax = v.size / c_ptrSize;
    int pCount = checkedSize != 0 ? (checkedSize / c_ptrSize) : pMax;
    auto data = PtrArray(v.data);
    auto diffData = IntArray(v.data + v.size);
    for (int i = 0; i < pCount; ++i) {
        ptrdiff_t diff = data[i + pMax] - data[i];
        if (diff) {
            VERIFY(size_t(diff) == v.size, "Invalid pointers shift");
            data[i] -= ptrdiff_t(v.data);
            *diffData++ = i;
        }
    }
    *diffData++ = v.size / c_intSize;
    v.size = Ptr(diffData) - v.data;
    return v;
}

template <typename T>
View serialize(const T& obj, size_t checkedSize = 0)
{
    return serialize<T>(obj, tlsBuffer(), checkedSize);
}

template <typename T>
View serializeVector(const std::vector<T>& obj)
{
    return serialize<std::vector<T>>(
        obj, obj.size() * sizeof(T) + sizeof(std::vector<T>));
}

template <typename T>
View serializeFlat(const T& obj)
{
    return serialize<T>(obj, sizeof(T));
}

// changes the buffer, performs in-place transformation
template <typename T>
T& deserialize(View view)
{
    VERIFY(view.size >= c_intSize, "Invalid buffer size: must be >= intSize");
    VERIFY(view.size % c_intSize == 0, "Invalid buffer size: must be aligned");
    int intCount = view.size / c_intSize - 1; // not to include the diff index
    auto intArray = IntArray(view.data);
    auto data = PtrArray(view.data);
    int diffIndex = intArray[intCount];
    VERIFY(diffIndex <= intCount, "Invalid diff index");
    int dataCount = diffIndex * c_intSize / c_ptrSize;
    for (int i = diffIndex; i < intCount; ++i) {
        int dataIndex = intArray[i];
        VERIFY(dataIndex < dataCount, "Invalid data index");
        data[dataIndex] += intptr_t(view.data);
    }
    return *(T*)view.data;
}
}
