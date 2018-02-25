#ifndef DP_SOA_BUFFER_H
#define DP_SOA_BUFFER_H
#include <cstdint>
#include <cstring>
#include <type_traits>
#include <cassert>
namespace dp {

    template <class ObjectT,size_t MemberN,class AosT = typename ObjectT::AosType, class SoaT = typename ObjectT::SoaType>
    struct ObjectTraitsBase
    {
        enum 
        {
            MEMBER_COUNT =  MemberN,
        };
        using AosType = AosT;
        using SoaType = SoaT;
    };

    template <class ObjectT>
    struct ObjectTypeTraits;

    template <class T,size_t I>
    struct MemberTypeTraits;

    template <class T, size_t I>
    using MemberType = typename MemberTypeTraits<T, I>::Type;
    template <class T> 
    using Aos = typename ObjectTypeTraits<T>::AosType;
    template <class T> 
    using Soa = typename ObjectTypeTraits<T>::SoaType;

    template <class T, size_t I>
    struct PointerToMemberTraits
    {
        typedef MemberType<T,I> T::*Type;
    };

    template <class T, size_t I>
    using PointerToMember = typename PointerToMemberTraits<T, I>::Type;

    template <class T, size_t I>
    static PointerToMember<T,I> GetPointerToMember();



#define DP_DECL_OBJECT(object_type,member_count)\
    template<>\
    struct dp::ObjectTypeTraits<object_type>: public dp::ObjectTraitsBase<object_type, member_count>\
    {\
    }
#define DP_DECL_OBJECT_AOS(object_type,member_count,aos_type)\
    template<>\
    struct dp::ObjectTypeTraits<object_type>: public dp::ObjectTraitsBase<object_type, member_count,aos_type>\
    {\
    }
#define DP_DECL_OBJECT_SOA(object_type,member_count,soa_type)\
    template<>\
    struct dp::ObjectTypeTraits<object_type>: public dp::ObjectTraitsBase<object_type, member_count,object_type::AosType,soa_type>\
    {\
    }
#define DP_DECL_OBJECT_AOS_SOA(object_type,member_count,aos_type,soa_type)\
    template<>\
    struct dp::ObjectTypeTraits<object_type>: public dp::ObjectTraitsBase<object_type, member_count,aos_type,soa_type>\
    {\
    }
#define DP_DECL_OBJECT_SOA_AOS(object_type,member_count,soa_type,aos_type) \
    DP_DECL_OBJECT_AOS_SOA(object_type,member_count,aos_type,soa_type)



#define DP_DECL_MEMBER_TRAITS(object_type,index,member_type)\
    template <>\
    struct dp::MemberTypeTraits<object_type, index>\
    {\
        using Type = member_type;\
    }
#define DP_DECL_POINTER_TO_MEMBER(object_type,index,member_name)\
    template <> \
    dp::PointerToMember<object_type, index> dp::GetPointerToMember<object_type, index>() \
    {\
        return &object_type::member_name;\
    }
#define DP_DECL_MEMBER(object_type ,index, member_type, member_name) \
        DP_DECL_MEMBER_TRAITS    (dp::Aos<object_type>,index,member_type);\
        DP_DECL_POINTER_TO_MEMBER(dp::Aos<object_type>,index,member_name)
        //DP_DECL_POINTER_TO_MEMBER(Soa<object_type>,index,member_name)
    /////////////////////////////////////////////////////
    namespace detail {
        template
        <
            template <size_t CallI, typename...> class MemberMethodT,
            size_t CallN, // 调用MemberMethodT的次数
            typename    ThisT,
            typename... ArgT
        >
        struct MemberMethodCaller
        {
            static void call(ThisT* thisptr, ArgT&&... args)
            {
                MemberMethodT<CallN, ArgT...> memberMethod;
                memberMethod(thisptr, std::forward<ArgT>(args)...);

                MemberMethodCaller<MemberMethodT,CallN - 1,ThisT,ArgT...>::call(thisptr, args...);
            }
        };

        template
        <
            template <size_t MemberI, typename...> class MemberMethodT,
            typename    ThisT,
            typename... ArgT
        >
        struct MemberMethodCaller<MemberMethodT,0,ThisT,ArgT...>
        {
            static void call(ThisT* thisptr, ArgT&&... args)
            {
            }
        };
    }
    template 
    <
        template <size_t MemberI,typename...> class MemberMethodT,
        size_t CallN, // 调用MemberMethodT的次数
        typename    ThisT,
        typename... ArgT
    >
    void CallMemberMethod(ThisT* thisptr, ArgT&&... args)
    {
        detail::MemberMethodCaller<MemberMethodT,CallN,ThisT,ArgT...>::call(thisptr, std::forward<ArgT>(args)...);
    }
    /////////////////////////////////////////////////
#define _DP_DECL_MEMBER_METHOD_WRAPPER(member_method) \
        template <size_t CallI,typename... ArgT>\
        struct member_method##Wrapper\
        {\
            void operator()(DataBuffer<T>* thisptr, ArgT&&... args)\
            {\
                thisptr->member_method<CallI - 1>(std::forward<ArgT>(args)...);\
            }\
            void operator()(const DataBuffer<T>* thisptr, ArgT&&... args)\
            {\
                thisptr->member_method<CallI - 1>(std::forward<ArgT>(args)...);\
            }\
        }

#define _DP_WRAPPER(member_method) member_method##Wrapper
    /////////////////////////////////////////////////
    template <class T>
    class DataBuffer
    {
    public:
        enum 
        {
            MEMBER_COUNT = ObjectTypeTraits<T>::MEMBER_COUNT,
        };
        using Element  = Aos<T>;
        using Elements = Soa<T>;

        template <size_t MemberI> using Member  = MemberType<Element, MemberI>;
        template <size_t MemberI> using Members = Member<MemberI>*;

    public:
        explicit DataBuffer(size_t capacity)
            : m_capacity(capacity)
        {
            assert(capacity > 0);
            CallMemberMethod<_DP_WRAPPER(_allocateMember),MEMBER_COUNT>(this);
        }
        ~DataBuffer()
        {
            for(auto item: m_array)
                std::free(item);
        }
        size_t append(const Element& element)
        {
            assert(m_size < m_capacity);
            CallMemberMethod<_DP_WRAPPER(_appendMember),MEMBER_COUNT>(this, element);
            return m_size++;
        }
        void remove(size_t index)
        {
            assert(index < m_size);
            CallMemberMethod<_DP_WRAPPER(_removeMember), MEMBER_COUNT>(this, index);
            --m_size;
        }
        size_t size() const
        {
            return m_size;
        }
        size_t capacity() const
        {
            return m_capacity;
        }
        bool empty() const
        {
            return m_size == 0;
        }
        bool full() const
        {
            return m_size == m_capacity;
        }

        //Elements getElements(size_t elementOffset = 0) const
        //{
        //
        //}
        Element getElement(size_t index) const
        {
            assert(index < m_size);
            Element elementOut;
            CallMemberMethod<_DP_WRAPPER(_retrieveMember), MEMBER_COUNT>(this, index,elementOut);
            return elementOut;
        }
        template <size_t MemberI>
        const Members<MemberI> getMembers() const
        {
            static_assert(MemberI < MEMBER_COUNT);
            return reinterpret_cast<Members<MemberI>>(m_array[MemberI]);
        }
        template <size_t MemberI>
        const Member<MemberI>& getMember(size_t index) const
        {
            static_assert(MemberI < MEMBER_COUNT);
            return this->getMembers<MemberI>()[index];
        }

    public:
        template <size_t MemberI>
        void _appendMember(const Element& element)
        {
            auto data = reinterpret_cast<Members<MemberI>>(m_array[MemberI]);
            data[m_size] = element.*(GetPointerToMember<Element, MemberI>());
        }
        _DP_DECL_MEMBER_METHOD_WRAPPER(_appendMember);

        template <size_t MemberI>
        void _removeMember(size_t index)
        {
            auto data = reinterpret_cast<Members<MemberI>>(m_array[MemberI]);
            data[index] = data[m_size - 1];
        }
        _DP_DECL_MEMBER_METHOD_WRAPPER(_removeMember);

        template <size_t MemberI>
        void _retrieveMember(size_t index,Element& elementOut) const
        {
            auto data = reinterpret_cast<Members<MemberI>>(m_array[MemberI]);
            elementOut.*(GetPointerToMember<Element, MemberI>()) = data[index];
        }
        _DP_DECL_MEMBER_METHOD_WRAPPER(_retrieveMember);

        template <size_t MemberI>
        void _allocateMember()
        {
            m_array[MemberI] = (uint8_t*)std::malloc(sizeof(Member<MemberI>)*m_capacity);
#ifdef _DEBUG
            std::memset(m_array[MemberI], sizeof(Member<MemberI>)*m_capacity, 0);
#endif
        }
        _DP_DECL_MEMBER_METHOD_WRAPPER(_allocateMember);
    private:
        uint8_t* m_array[MEMBER_COUNT] = {nullptr};
        size_t   m_size       = 0;
        size_t   m_capacity   = 100;
    };

}//ns dp


#endif //DP_SOA_BUFFER_H