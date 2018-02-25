#include <Dope/Common/Dod/DataBuffer.h>
namespace dp {

    //struct Transform {
    //    int x, y, z;
    //};
    //// Number of members
    //template<typename ComponentType>
    //struct GetMemberCount;
    //
    //// Type of member N
    //template<typename ComponentType, unsigned int N>
    //struct GetType;
    //template<> struct GetMemberCount<Transform> { static const unsigned int count = 3; };
    //template<> struct GetType<Transform, 0> { typedef int Type; };
    //template<> struct GetType<Transform, 1> { typedef int Type; };
    //template<> struct GetType<Transform, 2> { typedef int Type; };
    //
    ////Helper Type : pointer to member of struct
    //template<typename T, unsigned int N>
    //struct GetPointerToMemberType 
    //{
    //    typedef typename GetType<T, N>::Type T::*Type;
    //};
    ////template<typename T, unsigned int N>
    ////using PointerToMemberType  = GetPointerToMemberType<T, N>::Type;
    ////Pointers to members of struct (override to give them)
    //template<typename T, unsigned int N>
    //static typename GetPointerToMemberType<T, N>::Type GetPointerToMember();
    //
    ////Specialized GetPointerToMember()
    //template<> 
    //typename GetPointerToMemberType<Transform, 0>::Type GetPointerToMember<Transform, 0>() { return &Transform::x; };
    //template<>  
    //typename GetPointerToMemberType<Transform, 1>::Type GetPointerToMember<Transform, 1>() { return &Transform::y; };
    //template<>  
    //typename GetPointerToMemberType<Transform, 2>::Type GetPointerToMember<Transform, 2>() { return &Transform::z; };

    struct TestStruct
    {
        struct AosType
        {
            float m_positionX;
            float m_positionY;
            float m_positionZ;
        };
        struct SoaType
        {
            float* m_positionX;
            float* m_positionY;
            float* m_positionZ;
        };
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
    struct TestStruct2_soa
    {
        float* m_positionX;
        float* m_positionY;
        float* m_positionZ;
    };
    DP_DECL_OBJECT_AOS_SOA(TestStruct2,3,TestStruct2,TestStruct2_soa);
    DP_DECL_MEMBER(TestStruct2, 0, float, m_positionX);
    DP_DECL_MEMBER(TestStruct2, 1, float, m_positionY);
    DP_DECL_MEMBER(TestStruct2, 2, float, m_positionZ);

    void foo()
    {
        DataBuffer<TestStruct> buffer(100);
        buffer.append(Aos<TestStruct>());
        buffer.remove(0);

        DataBuffer<TestStruct2> buffer2(100);
        buffer2.append(TestStruct2());
        buffer2.remove(0);
    }
}