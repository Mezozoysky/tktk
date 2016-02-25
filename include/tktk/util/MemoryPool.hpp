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

#include <tktk/Config.hpp>
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

struct Id64
{
    Id64()
    : mId{ 0 }
    {
    }
    Id64( uint32_t index, uint32_t version )
    : mId{ uint64_t( index ) | uint64_t( version ) << 32UL }
    {
    }

    uint32_t index() const noexcept
    {
        return ( mId & 0xffffffffUL );
    }

    uint32_t version() const noexcept
    {
        return ( mId >> 32UL );
    }

private:
    uint64_t mId{ 0 };
};

const Id64 ID64_INVALID{};


class MemoryPoolBase
{
public:
    MemoryPoolBase() = default;
    virtual ~MemoryPoolBase() = default;

    virtual std::uint32_t getSize() const noexcept = 0;
    virtual std::uint32_t getCapacity() const noexcept = 0;
    virtual std::uint32_t getChunksNumber() const noexcept = 0;
    virtual std::uint32_t getChunkCapacity() const noexcept = 0;
    virtual std::uint32_t getDeadSize() const noexcept = 0;
    virtual std::size_t getElementSize() const noexcept = 0;

    virtual void clear() noexcept = 0;
    virtual void reserve( std::uint32_t cap ) noexcept = 0;

    virtual bool isAlive( std::uint32_t index ) const noexcept = 0;
};


template < typename ValueTypeT, std::uint32_t chunkSizeV = 256 >
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
        ll_trace( "Memory pool ctor in; chunk size: " << chunkSizeV );
        ll_trace( "Memory pool ctor out;" );
    }

    virtual ~MemoryPool()
    {
        ll_trace( "Memory pool dtor in;" );
        clear();
        ll_trace( "Memory pool dtor out;" );
    }

    template< typename... Args >
    Id64 createElement( Args&&... args )
    {
        std::uint32_t index;

        if ( mDeadIndices.empty() )
        {
            index = mVersions.size();

            if ( index == mCapacity )
            {
                addChunk();
            }

            mVersions.push_back( 1 );
        }
        else
        {
            index = mDeadIndices.back();
            mDeadIndices.pop_back();
        }

        std::size_t chunkIndex{ index / mChunkSize };
        std::size_t elementIndex{ index % mChunkSize };
        new( mChunks[ chunkIndex ].data + elementIndex ) ValueTypeT( std::forward< Args >( args )... );

        Id64 id{ index, mVersions[ index ] };
        return ( id );
    }

    void destroyElement( Id64 id )
    {
        if ( !isIdValid( id ) )
        {
            return;
        }

        destroyElement( id.index() );
    }

    void destroyElement( std::uint32_t index )
    {
        assert( index < mVersions.size() && "Bad index!" );
        if ( !isAlive( index ) )
        {
            ll_warning( "Trying to destroy dead element. index: " << index );
            return;
        }
        std::size_t chunkIndex{ index / mChunkSize };
        std::size_t elementIndex{ index % mChunkSize };
        ll_debug( "-> destroying element by valid index: " << index << "; chunkIndex: " << chunkIndex << "; elementIndex: " << elementIndex );
        ll_debug( "before destroying - pool size: " << mVersions.size() << "; pool effective size: " << ( mVersions.size() - mDeadIndices.size() ) );
//         const auto value = reinterpret_cast< const ValueTypeT* >( mChunks[ chunkIndex ].data + elementIndex );
//         value->~ValueTypeT();
        reinterpret_cast< const ValueTypeT* >( mChunks[ chunkIndex ].data + elementIndex )->~ValueTypeT();
        mVersions[ index ]++;

        auto it = std::lower_bound( mDeadIndices.begin(), mDeadIndices.end(), index, std::greater< std::size_t >() );
        mDeadIndices.insert( it, index );
        ll_debug( "index " << index << " is now dead" );
        ll_debug( "after destroying - pool size: " << mVersions.size() << "; pool effective size: " << ( mVersions.size() - mDeadIndices.size() ) );

    }

    const ValueTypeT& operator[]( std::uint32_t index ) const noexcept
    {
        assert( index < mVersions.size() && "Bad index!" );
        std::uint32_t chunkIndex{ index / mChunkSize };
        std::uint32_t elementIndex{ index % mChunkSize };
        return ( *reinterpret_cast< const ValueTypeT* >( mChunks[ chunkIndex ].data + elementIndex ) );
    }

    ValueTypeT& operator[]( std::uint32_t index ) noexcept
    {
        assert( index < mVersions.size() && "Bad index!" );
        std::uint32_t chunkIndex{ index / mChunkSize };
        std::uint32_t elementIndex{ index % mChunkSize };
        return ( *reinterpret_cast< ValueTypeT* >( mChunks[ chunkIndex ].data + elementIndex ) );
    }

    ValueTypeT* getPtr( std::size_t index ) const noexcept
    {
        assert( index < mVersions.size() && "Bad index!" );
        std::size_t chunkIndex{ index / mChunkSize };
        std::size_t elementIndex{ index % mChunkSize };
        return ( const_cast< ValueTypeT* >( reinterpret_cast< const ValueTypeT* >( mChunks[ chunkIndex ].data + elementIndex ) ) ); // fuck
    }

    virtual std::uint32_t getSize() const noexcept final
    {
        return ( mVersions.size() );
    }

    virtual std::uint32_t getCapacity() const noexcept final
    {
        return ( mCapacity );
    }

    virtual std::uint32_t getChunksNumber() const noexcept final
    {
        return ( mChunks.size() );
    }

    virtual std::uint32_t getChunkCapacity() const noexcept final
    {
        return ( mChunkSize );
    }

    virtual std::uint32_t getDeadSize() const noexcept final
    {
        return ( mDeadIndices.size() );
    }

    virtual std::size_t getElementSize() const noexcept final
    {
        return ( sizeof( ValueTypeT ) );
    }

    virtual void clear() noexcept final
    {
        for ( std::uint32_t i = 0; i < mVersions.size(); ++i )
        {
            if ( mVersions[ i ] )
            {
                destroyElement( i );
            }
        }

        while ( !mChunks.empty() )
        {
            mChunks.pop_back();
        }
    }

    virtual void reserve( std::uint32_t cap ) noexcept final
    {
        while ( mCapacity < cap )
        {
            addChunk();
        }
    }

    virtual bool isIdValid( const Id64& id  ) const noexcept final
    {
        std::uint32_t index{ id.index() };
        if ( index < mVersions.size() && mVersions[ index ] == id.version() )
        {
            return ( true );
        }
        return ( false );
    }

    virtual bool isAlive( std::uint32_t index ) const noexcept final
    {
        assert( index < mVersions.size() && "Bad index!" );
        auto it( std::find_if( mDeadIndices.begin(), mDeadIndices.end(), [index]( std::uint32_t dead ) { return ( dead == index ); } ) );
        return ( it == mDeadIndices.end() );
    }

private:

    void addChunk()
    {
        mChunks.emplace_back();
        mCapacity += mChunkSize;
        mVersions.reserve( mCapacity );
    }

private:
    std::uint32_t mChunkSize;
    std::uint32_t mCapacity{ 0 };

    std::vector< Chunk > mChunks;
    std::vector< std::uint32_t > mVersions;
    std::vector< std::uint32_t > mDeadIndices;
};


} //namespace util
} //namespace tktk

#endif /* end of include guard: TKTK_UTIL_MEMORY_POOL_HPP */
