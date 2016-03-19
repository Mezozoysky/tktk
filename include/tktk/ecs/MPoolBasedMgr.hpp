//  tktk
//
//  tktk - Copyright (C) 2016 Stanislav Demyanovich <stan@angrybubo.com>
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
/// \brief This file provides MPoolBasedMgr type
/// \author Stanislav Demyanovich <stan@angrybubo.com>
/// \date 2016
/// \copyright tktk is released under the terms of zlib/png license; see full license text at https://opensource.org/licenses/Zlib


#ifndef TKTK_ECS_MPOOL_BASED_MGR_HPP
#define TKTK_ECS_MPOOL_BASED_MGR_HPP

#include <tktk/ecs/Config.hpp>
#include <tktk/ecs/Mgr.hpp>
#include <tktk/mpool/MemoryPool.hpp>

namespace tktk
{
namespace ecs
{

template< typename DataT >
class MPoolBasedMgr
: public Mgr< DataT >
{
public:

    MPoolBasedMgr() noexcept
    : Mgr< DataT >()
    {
    }

    virtual ~MPoolBasedMgr() noexcept
    {
    }

    /// \brief Adds component with constructructor arguments \em args and returns a handle of type CompTypeT::Handle
    template< typename... ArgsT >
    inline mpool::Id64 add( ArgsT&&... args )
    {
        return ( mPool.alloc( std::forward< ArgsT >( args )... ) );
    }

    /// \brief Marks \em DataTypeT object specified by \em id as destroyed if \em id is valid
    inline void drop( mpool::Id64 id )
    {
        mPool.free( id );
    }

    /// \brief Returns true if given id is valid
    inline bool isIdValid( mpool::Id64 id ) const noexcept
    {
        return ( mPool.isIdValid( id ) );
    }

    /// \brief Returns raw pointer to the data by given \em id
    inline DataT* getDataPtr( mpool::Id64 id ) const noexcept
    {
        return ( mPool.getPtr( id ) );
    }


private:
    using PoolTypeT = mpool::MemoryPool<DataT>;
    PoolTypeT mPool;

};

} //namespace ecs
} //namespace tktk

#endif /* end of include guard: TKTK_ECS_MPOOL_BASED_MGR_HPP */
