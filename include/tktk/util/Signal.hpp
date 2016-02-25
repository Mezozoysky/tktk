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

#ifndef TKTK_UTIL_SIGNAL_HPP
#define TKTK_UTIL_SIGNAL_HPP

#include <tktk/Config.hpp>
#include <functional>
#include <list>

namespace tktk
{
namespace util
{

template< typename... SignatureT >
class Signal
{
public:
    using Delegate = std::function< void( SignatureT... ) >;
    using DelegateList = std::list< Delegate >;

    using Connection = typename DelegateList::iterator;

    Connection connect( const Delegate& delegate ) noexcept
    {
        mDelegates.push_front( delegate );
        return ( mDelegates.begin() );
    }

    void disconnect( Connection& connection ) noexcept
    {
        if ( connection != mDelegates.end() )
        {
            mDelegates.erase( connection );
            connection = mDelegates.end();
        }
    }

    void disconnectAll() noexcept
    {
        while ( !mDelegates.empty() )
        {
            disconnect( mDelegates.back() );
        }
    }

    template< typename... Args >
    void operator()( Args&&... args ) const noexcept
    {
        for ( const auto& delegate : mDelegates )
        {
            delegate( std::forward< Args >( args )... );
        }
    }

private:
    DelegateList mDelegates;
};

} //namespace util
} //namespace tktk

#endif /* end of include guard: TKTK_UTIL_SIGNAL_HPP */
