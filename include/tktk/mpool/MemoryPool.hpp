//  tktk
//
//  tktk - Copyright (C) 2015 Stanislav Demyanovich <stan@angrybubo.com>
//
//  This software is provided 'as-is', without any express or
//  implied warranty. In no event will the authors be held
//  liable for any damages arising from the use of this software.
//
//  Permission is granted to anyone to use this software for any purpose,
//  including commercial applications, and to alter it and redistribute
//  it freely, subject to the following restrictions:
//
//  1.  The origin of this software must not be misrepresented;
//      you must not claim that you wrote the original software.
//      If you use this software in a product, an acknowledgment
//      in the product documentation would be appreciated but
//      is not required.
//
//  2.  Altered source versions must be plainly marked as such,
//      and must not be misrepresented as being the original software.
//
//  3.  This notice may not be removed or altered from any
//      source distribution.


/// \file
/// \brief This file provides MemoryPool type
/// \author Stanislav Demyanovich <stan@angrybubo.com>
/// \date 2015-2016
/// \copyright tktk is released under the terms of zlib/png license; see full license text at https://opensource.org/licenses/Zlib

#ifndef TKTK_MPOOL_MEMORY_POOL_HPP
#define TKTK_MPOOL_MEMORY_POOL_HPP

#include <tktk/mpool/Config.hpp>
#include <tktk/mpool/AMemoryPool.hpp>
#include <tktk/mpool/Id64.hpp>
#include <vector>
#include <cassert>
#include <type_traits>
#include <functional>
#include <algorithm>

namespace tktk
{
namespace mpool
{

/// \brief Pool for contiguous managed memory allocation
/// \tparam ValueT The data type to manage memory for
/// \tparam chunkSizeV Number of \em ValueT per memory chunk
///
/// Usage example:
/// \code{.cpp}
/// #include <tktk/mpool/MemoryPool.hpp>
/// #include <cassert>
/// using namespace tktk::mpool;
/// struct Some
/// {
///     Some( int i, const std::string& s )
///     : idata{ i }
///     , sdata{ s }
///     {
///     }
///     int idata{0};
///     std::string sdata{""};
/// };
/// MemoryPool< Some, 32 > pool;
/// Id64 id{ pool.alloc( 8, "aubergine" ) };
/// assert( pool.isIdValid( id ) && "Bug in mpool!" );
/// \endcode
template < typename ValueT, std::uint32_t chunkSizeV = 256 >
class MemoryPool
: public AMemoryPool
{
public:

    /// \brief Wrap-type for managing value type
    using ValueTypeT = ValueT;
    /// \brief Wrap-type for underlaying aligned_storage template
    using AlignedTypeT = typename std::aligned_storage< sizeof( ValueTypeT ), alignof( ValueTypeT ) >::type;

    /// \brief Represents one chunk of allocated memory
    struct Chunk
    {
        AlignedTypeT data[ chunkSizeV ]; ///< Array of aligned memory
    };

    /// \brief Default constructor
    MemoryPool() noexcept
    {
    }

    /// \brief Virtual destructor for further inheritance
    virtual ~MemoryPool()
    {
        clear();
    }

    /// \brief Allocate one element with constructor \em args
    template< typename... Args >
    Id64 alloc( Args&&... args )
    {
        std::uint32_t index;

        if ( mFreeIndices.empty() )
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
            index = mFreeIndices.back();
            mFreeIndices.pop_back();
        }

        std::size_t chunkIndex{ index / chunkSizeV };
        std::size_t elementIndex{ index % chunkSizeV };
        new( mChunks[ chunkIndex ].data + elementIndex ) ValueTypeT( std::forward< Args >( args )... );

        Id64 id{ index, mVersions[ index ] };
        return ( id );
    }

    /// \brief Free memory block by given id
    void free( Id64 id )
    {
        if ( !isIdValid( id ) )
        {
            return;
        }
        free( id.index() );
    }

    /// \brief Gets pointer to memory block
    ValueTypeT* getPtr( Id64 id ) const noexcept
    {
        if ( !isIdValid( id ) )
        {
            return ( nullptr );
        }
        std::uint32_t index{ id.index() };
        assert( index < mVersions.size() && "Bad index!" );
        std::size_t chunkIndex{ index / chunkSizeV };
        std::size_t elementIndex{ index % chunkSizeV };
        return ( const_cast< ValueTypeT* >( reinterpret_cast< const ValueTypeT* >( mChunks[ chunkIndex ].data + elementIndex ) ) ); //fuck!
    }

    /// \brief Gets const reference to memory block
    const ValueTypeT& operator[]( Id64 id ) const
    {
        if ( !isIdValid( id ) )
        {
            throw std::range_error( "MemoryPool element access violation" );
        }
        std::uint32_t index{ id.index() };
        assert( index < mVersions.size() && "Bad index!" );
        std::uint32_t chunkIndex{ index / chunkSizeV };
        std::uint32_t elementIndex{ index % chunkSizeV };
        return ( *reinterpret_cast< const ValueTypeT* >( mChunks[ chunkIndex ].data + elementIndex ) );
    }

    /// \brief Gets reference to memory block
    ValueTypeT& operator[]( Id64 id )
    {
        if ( !isIdValid( id ) )
        {
            throw std::range_error( "MemoryPool element access violation" );
        }
        std::uint32_t index{ id.index() };
        assert( index < mVersions.size() && "Bad index!" );
        std::uint32_t chunkIndex{ index / chunkSizeV };
        std::uint32_t elementIndex{ index % chunkSizeV };
        return ( *reinterpret_cast< ValueTypeT* >( mChunks[ chunkIndex ].data + elementIndex ) );
    }

    /// \brief Pool size
    virtual std::uint32_t getSize() const noexcept final
    {
        return ( mVersions.size() );
    }

    /// \brief Pool capacity (memory footprint)
    virtual std::uint32_t getCapacity() const noexcept final
    {
        return ( mCapacity );
    }

    /// \brief Number of chunks used
    virtual std::uint32_t getChunksNumber() const noexcept final
    {
        return ( mChunks.size() );
    }

    /// \brief Getter for chunk capacity
    virtual std::uint32_t getChunkCapacity() const noexcept final
    {
        return ( chunkSizeV );
    }

    /// \brief Number of free elements contained
    virtual std::uint32_t getFreeSize() const noexcept final
    {
        return ( mFreeIndices.size() );
    }

    /// \brief Memory block size in bytes
    virtual std::size_t getElementSize() const noexcept final
    {
        return ( sizeof( ValueTypeT ) );
    }

    /// \brief Calls destructor for each alive element, deallocates the chunks
    virtual void clear() noexcept final
    {
        for ( std::uint32_t i = 0; i < mVersions.size(); ++i )
        {
            if ( !isAlive( i ) )
            {
                continue;
            }
            free( i );
        }

        while ( !mChunks.empty() )
        {
            mChunks.pop_back();
        }
    }

    /// \brief Reserves capacity >= \em cap
    virtual void reserve( std::uint32_t cap ) noexcept final
    {
        while ( mCapacity < cap )
        {
            addChunk();
        }
    }

    /// \brief Test if id valid
    virtual bool isIdValid( Id64 id  ) const noexcept final
    {
        std::uint32_t index{ id.index() };
        if ( index < mVersions.size() && mVersions[ index ] == id.version() )
        {
            return ( true );
        }
        return ( false );
    }

    /// \brief Calls given function for every element
    void forEach( std::function< void( ValueTypeT& value ) > func )
    {
        for ( std::uint32_t index{ 0 }; index < getSize(); ++index )
        {
            if ( !isAlive( index ) ) continue;
            std::uint32_t chunkIndex{ index / chunkSizeV };
            std::uint32_t elementIndex{ index % chunkSizeV };
            func ( *reinterpret_cast< ValueTypeT* >( mChunks[ chunkIndex ].data + elementIndex ) );
        }
    }

private:

    /// \brief Adds new chunk of memory onto the field
    void addChunk()
    {
        mChunks.emplace_back();
        mCapacity += chunkSizeV;
        mVersions.reserve( mCapacity );
    }

    /// \brief Mark the memory block as free
    void free( std::uint32_t index )
    {
        std::size_t chunkIndex{ index / chunkSizeV };
        std::size_t elementIndex{ index % chunkSizeV };
        ll_debug( "-> destroying element by valid index: " << index << "; chunkIndex: " << chunkIndex << "; elementIndex: " << elementIndex );
        ll_debug( "before destroying - pool size: " << mVersions.size() << "; pool effective size: " << ( mVersions.size() - mFreeIndices.size() ) );
        reinterpret_cast< const ValueTypeT* >( mChunks[ chunkIndex ].data + elementIndex )->~ValueTypeT();
        mVersions[ index ]++;

        auto it = std::lower_bound( mFreeIndices.begin(), mFreeIndices.end(), index, std::greater< std::size_t >() );
        mFreeIndices.insert( it, index );
        ll_debug( "index " << index << " is now free" );
        ll_debug( "after destroying - pool size: " << mVersions.size() << "; pool effective size: " << ( mVersions.size() - mFreeIndices.size() ) );
    }

    /// \brief Test if the index not in the free-list
    virtual bool isAlive( std::uint32_t index ) const noexcept final
    {
        assert( index < mVersions.size() && "Bad index!" );
        auto it( std::find_if( mFreeIndices.begin(), mFreeIndices.end(), [index]( std::uint32_t dead ) { return ( dead == index ); } ) );
        return ( it == mFreeIndices.end() );
    }

private:

//     std::uint32_t mChunkSize;
    std::uint32_t mCapacity{ 0 }; ///< Current pool capacity

    std::vector< Chunk > mChunks; ///< Actual chunks of our managed memory.
    std::vector< std::uint32_t > mVersions; ///< Vector of current versions for memory blocks.
    std::vector< std::uint32_t > mFreeIndices; ///< Vector of free-marked elements' indexes. Keeping sorted.
};


} //namespace mpool
} //namespace tktk

#endif /* end of include guard: TKTK_MPOOL_MEMORY_POOL_HPP */
