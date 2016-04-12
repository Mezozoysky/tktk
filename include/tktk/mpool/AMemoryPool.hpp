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
/// \brief This file provides AMemoryPool abstract type
/// \author Stanislav Demyanovich <stan@angrybubo.com>
/// \date 2015-2016
/// \copyright tktk is released under the terms of zlib/png license; see full license text at https://opensource.org/licenses/Zlib

#ifndef TKTK_MPOOL_A_MEMORY_POOL_HPP
#define TKTK_MPOOL_A_MEMORY_POOL_HPP

#include <tktk/mpool/Config.hpp>
#include <cstdint>

namespace tktk
{
namespace mpool
{

/// \brief Abstract base for memory pools
class AMemoryPool
{
    AMemoryPool( const AMemoryPool& ) = delete;
    AMemoryPool& operator= ( const AMemoryPool& ) = delete;

public:
    AMemoryPool() = default;
    virtual ~AMemoryPool() = default;

    virtual std::uint32_t getSize() const noexcept = 0;
    virtual std::uint32_t getCapacity() const noexcept = 0;
    virtual std::uint32_t getChunksNumber() const noexcept = 0;
    virtual std::uint32_t getChunkCapacity() const noexcept = 0;
    virtual std::uint32_t getFreeSize() const noexcept = 0;
    virtual std::size_t getElementSize() const noexcept = 0;

    virtual void clear() noexcept = 0;
    virtual void reserve( std::uint32_t cap ) noexcept = 0;

    virtual bool isAlive( std::uint32_t index ) const noexcept = 0;
};

} //namespace mpool
} //namespace tktk

#endif /* end of include guard: TKTK_MPOOL_A_MEMORY_POOL_HPP */
