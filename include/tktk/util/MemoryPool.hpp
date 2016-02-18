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
#include <functional>
#include <algorithm>

namespace tktk
{
namespace util
{

class MemoryPoolBase
{
public:
    MemoryPoolBase() = default;
    virtual ~MemoryPoolBase() = default;

    virtual std::size_t getSize() const noexcept = 0;
    virtual std::size_t getCapacity() const noexcept = 0;
    virtual std::size_t getChunksNumber() const noexcept = 0;
    virtual std::size_t getChunkCapacity() const noexcept = 0;
    virtual std::size_t getDeadSize() const noexcept = 0;

    virtual void clear() noexcept = 0;
    virtual void reserve( std::size_t cap ) noexcept = 0;
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
    
    explicit MemoryPool() noexcept
    : mChunkSize{ chunkSizeV }
    {
    }
    
    virtual ~MemoryPool()
    {
        clear();
    }

    template< typename... Args >
    std::size_t createElement( Args&&... args )
    {
        std::size_t index;

        if ( mDeadIndices.empty() )
        {
            index = mElementsPresence.size();

            if ( index == mCapacity )
            {
                addChunk();
            }

            mElementsPresence.push_back( 1 );
        }
        else
        {
            index = mDeadIndices.back();
            mDeadIndices.pop_back();
            mElementsPresence[ index ] = 1;
        }

        std::size_t chunkIndex{ index / mChunkSize };
        std::size_t elementIndex{ index % mChunkSize };
        new( mChunks[ chunkIndex ].data + elementIndex ) ValueTypeT( std::forward< Args >( args )... );

        return ( index );
    }
    
    void destroyElement( std::size_t index )
    {
        assert( index < mElementsPresence.size() );

        std::size_t chunkIndex{ index / mChunkSize };
        std::size_t elementIndex{ index % mChunkSize };
        reinterpret_cast< const ValueTypeT* >( mChunks[ chunkIndex ].data + elementIndex )->~ValueTypeT();
        mElementsPresence[ index ] = 0;

        auto it = std::lower_bound( mDeadIndices.begin(), mDeadIndices.end(), index, std::greater< std::size_t >() );
        mDeadIndices.insert( it, index );
    }
    
    const ValueTypeT& operator[]( std::size_t index ) const
    {
        std::size_t chunkIndex{ index / mChunkSize };
        std::size_t elementIndex{ index % mChunkSize };
        return ( *reinterpret_cast< const ValueTypeT* >( mChunks[ chunkIndex ].data + elementIndex ) );
    }

    ValueTypeT& operator[]( std::size_t index )
    {
        std::size_t chunkIndex{ index / mChunkSize };
        std::size_t elementIndex{ index % mChunkSize };
        return ( *reinterpret_cast< ValueTypeT* >( mChunks[ chunkIndex ].data + elementIndex ) );
    }

    const ValueTypeT* getPtr( std::size_t index ) const
    {
        std::size_t chunkIndex{ index / mChunkSize };
        std::size_t elementIndex{ index % mChunkSize };
        return ( reinterpret_cast< ValueTypeT* >( mChunks[ chunkIndex ].data + elementIndex ) );
    }

    ValueTypeT* getPtr( std::size_t index )
    {
        std::size_t chunkIndex{ index / mChunkSize };
        std::size_t elementIndex{ index % mChunkSize };
        return ( reinterpret_cast< ValueTypeT* >( mChunks[ chunkIndex ].data + elementIndex ) );
    }
    
    virtual std::size_t getSize() const noexcept final
    {
        return ( mElementsPresence.size() );
    }
    
    virtual std::size_t getCapacity() const noexcept final
    {
        return ( mCapacity );
    }

    virtual std::size_t getChunksNumber() const noexcept final
    {
        return ( mChunks.size() );
    }

    virtual std::size_t getChunkCapacity() const noexcept final
    {
        return ( mChunkSize );
    }

    virtual std::size_t getDeadSize() const noexcept final
    {
        return ( mDeadIndices.size() );
    }

    virtual void clear() noexcept final
    {
        for ( std::size_t i = 0; i < mElementsPresence.size(); ++i )
        {
            if ( mElementsPresence[ i ] )
            {
                destroyElement( i );
            }
        }

        while ( !mChunks.empty() )
        {
            mChunks.pop_back();
        }
    }

    virtual void reserve( std::size_t cap ) noexcept final
    {
        while ( mCapacity < cap )
        {
            addChunk();
        }
    }

private:

    void addChunk()
    {
        mChunks.emplace_back();
        mCapacity += mChunkSize;
        mElementsPresence.reserve( mCapacity );
    }

private:
    std::size_t mElementSize{ sizeof( ValueTypeT ) };
    std::size_t mChunkSize;
    std::size_t mCapacity{ 0 };

    std::vector< Chunk > mChunks;
    std::vector< std::uint8_t > mElementsPresence;
    std::vector< std::size_t > mDeadIndices;
};


} //namespace util
} //namespace tktk

#endif /* end of include guard: TKTK_UTIL_MEMORY_POOL_HPP */
