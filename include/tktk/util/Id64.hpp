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
/// \brief This file provides util::Id64 type
/// \author Stanislav Demyanovich <stan@angrybubo.com>
/// \date 2016
/// \copyright tktk is released under the terms of zlib/png license; see full license text at https://opensource.org/licenses/Zlib

#include <cstddef>

#ifndef TKTK_UTIL_ID64_HPP
#define TKTK_UTIL_ID64_HPP

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

} //namespace util
} //namespace tktk

#endif /* end of include guard: TKTK_UTIL_ID64_HPP */
