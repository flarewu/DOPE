#include <Dope/Common/Dod/DataBuffer.h>
#include <Dope/Common/Math/Real.h>
#include "gtest/gtest.h"
#include <memory>
struct TestStruct
{
    struct SoaType
    {
        float* m_positionX;
        float* m_positionY;
        float* m_positionZ;
    };
    using AosType = TestStruct;

    float m_positionX;
    float m_positionY;
    float m_positionZ;
};

DP_DECL_OBJECT(TestStruct, 3);
DP_DECL_MEMBER(TestStruct, 0, float, m_positionX);
DP_DECL_MEMBER(TestStruct, 1, float, m_positionY);
DP_DECL_MEMBER(TestStruct, 2, float, m_positionZ);

struct TestStruct2
{
    float m_positionX;
    float m_positionY;
    float m_positionZ;
};
using TestStruct2_aos = TestStruct2;
struct TestStruct2_soa
{
    float* m_positionX;
    float* m_positionY;
    float* m_positionZ;
};
DP_DECL_OBJECT_AOS_SOA(TestStruct2, 3, TestStruct2_aos, TestStruct2_soa);
DP_DECL_MEMBER(TestStruct2, 0, float, m_positionX);
DP_DECL_MEMBER(TestStruct2, 1, float, m_positionY);
DP_DECL_MEMBER(TestStruct2, 2, float, m_positionZ);

static TestStruct TestStructCreate()
{
    TestStruct ts;
    {
        ts.m_positionX =  dp::random();
        ts.m_positionY =  dp::random();
        ts.m_positionZ =  dp::random();
    }
    return ts;
}

TEST(DataBufferTest, Ctor)
{
    ::srand(::time(0));

    auto cap = size_t(dp::random() * 1000);
    dp::DataBuffer<TestStruct> buffer(cap);
    ::printf("成功创造容量为: %d 的DataBuffer<TestStruct>\n",cap);
    EXPECT_TRUE(buffer.empty());
    EXPECT_EQ(buffer.capacity() , cap);

    auto cap2 = size_t(dp::random() * 1000);
    dp::DataBuffer<TestStruct2> buffer2(cap2);
    ::printf("成功创造容量为: %d 的DataBuffer<TestStruct2>\n",cap2);
    EXPECT_TRUE(buffer2.empty(), true);
    EXPECT_EQ(buffer2.capacity() , cap2);
}
TEST(DataBufferTest, Append)
{
    ::srand(::time(0));
    const size_t cap = size_t(dp::random() * 1000);
    dp::DataBuffer<TestStruct> buffer(cap);
    ::printf("成功创造容量为: %d 的DataBuffer<TestStruct>\n",cap);
    for(int i = 0; i < cap; ++i)
    {
        auto ts = TestStructCreate();
        auto id = buffer.append(ts);
        EXPECT_EQ(ts.m_positionX, buffer.getElement(id).m_positionX);
        EXPECT_EQ(ts.m_positionY, buffer.getElement(id).m_positionY);
        EXPECT_EQ(ts.m_positionZ, buffer.getElement(id).m_positionZ);
    }
    EXPECT_TRUE(buffer.full());
}
TEST(DataBufferTest, Remove)
{
    ::srand(::time(0));
    const size_t cap = size_t(dp::random() * 1000);
    dp::DataBuffer<TestStruct> buffer(cap);
    ::printf("成功创造容量为: %d 的DataBuffer<TestStruct>\n", cap);
    for(int i = 0; i < cap; ++i)
        buffer.append(TestStructCreate());
    EXPECT_TRUE(buffer.full());

    const size_t nRemoval = size_t(dp::random() * buffer.size());
    const size_t nRemaining = buffer.size() - nRemoval;
    for(int i = 0; i < nRemoval; ++i)
        buffer.remove(dp::random() * buffer.size());
    ::printf("成功移除个数为: %d\n", nRemoval);
    EXPECT_EQ(buffer.size(),nRemaining);

}
TEST(DataBufferTest, GetMember)
{
    ::srand(::time(0));
    const size_t cap = size_t(dp::random() * 1000);
    dp::DataBuffer<TestStruct> buffer(cap);
    ::printf("成功创造容量为: %d 的DataBuffer<TestStruct>\n", cap);
    auto elements = std::make_unique<TestStruct[]>(cap);
    for(int i = 0; i < cap; ++i)
    {
        elements[i] = TestStructCreate();
        buffer.append(elements[i]);
    }
    const size_t start = dp::random() * buffer.size();
    ::printf("从: %d 开始到%d\n", start,cap);
    for(size_t i = start; i < cap; ++i)
    {
        EXPECT_EQ(buffer.getMember<0>(i), elements[i].m_positionX);
        EXPECT_EQ(buffer.getMember<1>(i), elements[i].m_positionY);
        EXPECT_EQ(buffer.getMember<2>(i), elements[i].m_positionZ);
    }

}
TEST(DataBufferTest, GetMembers)
{
    ::srand(::time(0));
    const size_t cap = size_t(dp::random() * 1000);
    dp::DataBuffer<TestStruct> buffer(cap);
    ::printf("成功创造容量为: %d 的DataBuffer<TestStruct>\n", cap);
    auto elements = std::make_unique<TestStruct[]>(cap);
    for(int i = 0; i < cap; ++i)
    {
        elements[i] = TestStructCreate();
        buffer.append(elements[i]);
    }
    const size_t start = dp::random() * buffer.size();
    ::printf("从: %d 开始到%d\n", start, cap);

    auto members0 = buffer.getMembers<0>();
    auto members1 = buffer.getMembers<1>();
    auto members2 = buffer.getMembers<2>();

    for(size_t i = start; i < cap; ++i)
    {
        EXPECT_EQ(members0[i], elements[i].m_positionX);
        EXPECT_EQ(members1[i], elements[i].m_positionY);
        EXPECT_EQ(members2[i], elements[i].m_positionZ);
    }

}