/*
 * Copyright (C) 2012 BMW Car IT GmbH
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <gtest/gtest.h>
#include "capu/container/RingBuffer.h"

TEST(RingBuffer, TestBufferSize1)
{
    capu::RingBuffer<uint32_t> buffer(1);
    uint32_t val1 = 42;
    uint32_t val2 = 666;

    buffer.add(val1);

    capu::RingBuffer<uint32_t>::Iterator it = buffer.begin();
    uint32_t curr;
    EXPECT_TRUE(it.hasNext());
    it.next(&curr);
    EXPECT_EQ(val1, curr);
    EXPECT_FALSE(it.hasNext());

    buffer.add(val2);

    it = buffer.begin();
    curr = 0;
    EXPECT_TRUE(it.hasNext());
    it.next(&curr);
    EXPECT_EQ(val2, curr);
    EXPECT_FALSE(it.hasNext());
}

TEST(RingBuffer, TestIterationFullBuffer)
{
    capu::RingBuffer<uint32_t> buffer(5);
    uint32_t count = 12;
    for (uint32_t i = 0; i < count; i++) // flood buffer
    {
        buffer.add(i);
    }

    uint32_t start = count - 5;
    capu::RingBuffer<uint32_t>::Iterator it = buffer.begin();
    uint32_t curr;

    EXPECT_TRUE(it.hasNext());
    while (it.hasNext())
    {
        it.next(&curr);
        EXPECT_EQ(start, curr);
        start++;
    }
    EXPECT_EQ(count, start);
}

TEST(RingBuffer, TestIterateWithInvalidAddress)
{
    capu::RingBuffer<uint32_t> buffer(5);

    uint32_t i1 = 1;
    uint32_t i2 = 2;
    uint32_t i3 = 3;

    buffer.add(i1);
    buffer.add(i2);
    buffer.add(i3);

    capu::RingBuffer<uint32_t>::Iterator it = buffer.begin();
    uint32_t* curr = 0; // invalid address with no memory
    capu::status_t st = it.next(curr);
    EXPECT_EQ(static_cast<uint32_t>(capu::CAPU_ENO_MEMORY), static_cast<uint32_t>(st));

}

TEST(RingBuffer, TestIterationTooManyNextCalls)
{
    capu::RingBuffer<uint32_t> buffer(5);

    uint32_t i1 = 1;
    uint32_t i2 = 2;
    uint32_t i3 = 3;

    buffer.add(i1);
    buffer.add(i2);
    buffer.add(i3);

    capu::RingBuffer<uint32_t>::Iterator it = buffer.begin();
    uint32_t curr;

    it.next(&curr);
    it.next(&curr);
    it.next(&curr);
    capu::status_t st = it.next(&curr); // too much!
    EXPECT_EQ(static_cast<uint32_t>(capu::CAPU_ENOT_EXIST), static_cast<uint32_t>(st));
    EXPECT_EQ(3u, curr);
}

TEST(RingBuffer, TestIterationEmptyBuffer)
{
    capu::RingBuffer<uint32_t> buffer(5);
    capu::RingBuffer<uint32_t>::Iterator it = buffer.begin();
    EXPECT_FALSE(it.hasNext());
}

TEST(RingBuffer, TestZeroSizeBuffer)
{
    capu::RingBuffer<uint32_t> buffer(0);
    uint32_t val = 42;
    // will not fail
    capu::status_t st = buffer.add(val);
    EXPECT_EQ(static_cast<int32_t>(capu::CAPU_ERANGE), static_cast<int32_t>(st));

    // iterator has no next
    capu::RingBuffer<uint32_t>::Iterator it = buffer.begin();
    EXPECT_FALSE(it.hasNext());

    val = 0;
    st = it.next(&val);
    EXPECT_EQ(static_cast<int32_t>(capu::CAPU_ENOT_EXIST), static_cast<int32_t>(st));
    EXPECT_EQ(0u, val); // val is unchanged
}


TEST(RingBuffer, TestBufferWithPointers)
{
    uint32_t i1 = 1;
    uint32_t i2 = 2;
    uint32_t i3 = 3;
    uint32_t i4 = 4;
    uint32_t i5 = 5;

    uint32_t* i1ptr = &i1;
    uint32_t* i2ptr = &i2;
    uint32_t* i3ptr = &i3;
    uint32_t* i4ptr = &i4;
    uint32_t* i5ptr = &i5;

    capu::RingBuffer<uint32_t*> buffer(3);
    buffer.add(i1ptr);
    buffer.add(i2ptr);
    buffer.add(i3ptr);
    buffer.add(i4ptr);
    buffer.add(i5ptr);

    capu::RingBuffer<uint32_t*>::Iterator it = buffer.begin();
    uint32_t* currPtr = 0;
    EXPECT_TRUE(it.hasNext());
    it.next(&currPtr);
    EXPECT_EQ(i3ptr, currPtr);
    it.next(&currPtr);
    EXPECT_EQ(i4ptr, currPtr);
    it.next(&currPtr);
    EXPECT_EQ(i5ptr, currPtr);
}

TEST(RingBuffer, TestSize)
{
    capu::RingBuffer<uint32_t> buffer1(1000);
    EXPECT_EQ(1000u, buffer1.size());

    capu::RingBuffer<uint32_t> buffer2(0);
    EXPECT_EQ(0u, buffer2.size());
}

TEST(RingBuffer, ClearBuffer)
{
    capu::RingBuffer<uint32_t> buffer(20u);

    buffer.add(10u);
    buffer.add(60666u);
    EXPECT_TRUE(buffer.begin().hasNext());
    EXPECT_EQ(capu::CAPU_OK, buffer.clear());
    EXPECT_FALSE(buffer.begin().hasNext());
}

//uncomment to run performance tests
/*
TEST(RingBuffer, TestBufferPerformance)
{
    capu::RingBuffer<uint32_t> buffer(1000);

    uint32_t count = 1000000; // one million add-operations
    uint32_t start = capu::PlatformTime::GetMilliseconds();

    for(uint32_t i = 0; i < count; i++)
    {
        buffer.add(i);
    }

    uint32_t dur = capu::PlatformTime::GetMilliseconds() - start;
   // printf("[HEAP]  - %i add-operations take %i ms\n", count, dur);
    EXPECT_TRUE(dur < 100); // make sure we do not loose too much time!

    capu::StackBasedRingBuffer<uint32_t, 1000> stackBuffer;
    start = capu::PlatformTime::GetMilliseconds();

    for(uint32_t i = 0; i < count; i++)
    {
        stackBuffer.add(i);
    }

    dur = capu::PlatformTime::GetMilliseconds() - start;
    printf("[STACK] - %i add-operations take %i ms\n", count, dur);

}

 */
