/**
 tktk
 
 tktk - Copyright (C) 2015 Stanislav Demyanovich <stan@angrybubo.com>
 
 This software is provided 'as-is', without any express or
 implied warranty. In no event will the authors be held
 liable for any damages arising from the use of this software.
 
 Permission is granted to anyone to use this software for any purpose,
 including commercial applications, and to alter it and redistribute
 it freely, subject to the following restrictions:
 
 1.  The origin of this software must not be misrepresented;
 you must not claim that you wrote the original software.
 If you use this software in a product, an acknowledgment
 in the product documentation would be appreciated but
 is not required.
 
 2.  Altered source versions must be plainly marked as such,
 and must not be misrepresented as being the original software.
 
 3.  This notice may not be removed or altered from any
 source distribution.
 */

#ifndef TKTK_UTIL_MEMORY_POOL_HPP
#define TKTK_UTIL_MEMORY_POOL_HPP

#include <cstddef>
#include <vector>
#include <cassert>
#include <type_traits>

namespace tktk
{
namespace util
{

class MemoryPoolBase
{
public:
    MemoryPoolBase() = default;
    virtual ~MemoryPoolBase() = default;
};


template < typename ValueTypeT, std::size_t chunkSizeV = 256 >
class MemoryPool
: public MemoryPoolBase
{
public:

    using AlignedType = typename std::aligned_storage< sizeof( ValueTypeT ), alignof( ValueTypeT ) >::type;

    struct Chunk
    {
        AlignedType data[ chunkSizeV ];
    };
    
    explicit MemoryPool()
    : mChunkSize{ chunkSizeV }
    {
        mChunks.emplace_back();
        mCapacity += mChunkSize;
    }
    
    virtual ~MemoryPool()
    {
        if ( !mChunks.empty() )
        {
            for ( std::size_t i = 0; i < mSize; ++i )
            {
                reinterpret_cast< const ValueTypeT* >( mChunks[ 0 ].data + i )->~ValueTypeT();
            }
        }
    }

    template< typename... Args >
    std::size_t createElement( Args&&... args )
    {
        if ( mSize >= chunkSizeV )
        {
            throw std::bad_alloc{};
        }

        new( mChunks[ 0 ].data + mSize ) ValueTypeT( std::forward< Args >( args )... );

        return ( mSize++ );
    }
    
    void destroyElement( std::size_t index )
    {
    }
    
    const ValueTypeT& operator[]( std::size_t index ) const
    {
        return ( *reinterpret_cast< const ValueTypeT* >( mChunks[ 0 ].data + index ) );
    }

    ValueTypeT& operator[]( std::size_t index )
    {
        return ( *reinterpret_cast< ValueTypeT* >( mChunks[ 0 ].data + index ) );
    }
    
    std::size_t getSize() const noexcept
    {
        return ( mSize );
    }
    
    std::size_t getCapacity() const noexcept
    {
        return ( mCapacity );
    }

private:
    std::size_t mElementSize{ sizeof( ValueTypeT ) };
    std::size_t mChunkSize;
    std::vector< Chunk > mChunks;
    std::size_t mSize{ 0 };
    std::size_t mCapacity{ 0 };
};


} //namespace util
} //namespace tktk

#endif /* end of include guard: TKTK_UTIL_MEMORY_POOL_HPP */
