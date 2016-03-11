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
/// \brief This file provides Id64 type
/// \author Stanislav Demyanovich <stan@angrybubo.com>
/// \date 2015-2016
/// \copyright tktk is released under the terms of zlib/png license; see full license text at https://opensource.org/licenses/Zlib

#include <cstddef>

#ifndef TKTK_MPOOL_ID64_HPP
#define TKTK_MPOOL_ID64_HPP

namespace tktk
{
namespace mpool
{

/// \brief Struct-wrapper around 64bit number
///
/// 64bit combination of two 32bit numbers, first for the index and second for the 'mark' or 'version', provide the concept of 'index validity'.
struct Id64
{
    /// \brief Default constructor; constructs invalid id
    inline Id64() noexcept
    {
    }

    /// \brief Constructor from index and version
    inline Id64( std::uint32_t index, std::uint32_t version ) noexcept
    : mId{ std::uint64_t( index ) | std::uint64_t( version ) << 32UL }
    {
    }

    inline explicit Id64( std::uint64_t id ) noexcept
    : mId{ id }
    {
    }

    /// \brief Gets index
    inline std::uint32_t index() const noexcept
    {
        return ( mId & 0xffffffffUL );
    }

    /// \brief Gets version
    inline std::uint32_t version() const noexcept
    {
        return ( mId >> 32UL );
    }

    /// \brief Gets actual id
    inline std::uint64_t id() const noexcept
    {
        return ( mId );
    }

private:
    std::uint64_t mId{ 0 }; ///<  Actual id
};

const Id64 ID64_INVALID{};

} //namespace mpool
} //namespace tktk

#endif /* end of include guard: TKTK_MPOOL_ID64_HPP */
