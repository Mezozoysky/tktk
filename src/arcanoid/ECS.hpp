/**
 *    tktk
 * 
 *    tktk - Copyright (C) 2015 Stanislav Demyanovich <stan@angrybubo.com>
 * 
 *    This software is provided 'as-is', without any express or
 *    implied warranty. In no event will the authors be held
 *    liable for any damages arising from the use of this software.
 * 
 *    Permission is granted to anyone to use this software for any purpose,
 *    including commercial applications, and to alter it and redistribute
 *    it freely, subject to the following restrictions:
 * 
 *    1.  The origin of this software must not be misrepresented;
 *        you must not claim that you wrote the original software.
 *        If you use this software in a product, an acknowledgment
 *        in the product documentation would be appreciated but
 *        is not required.
 * 
 *    2.  Altered source versions must be plainly marked as such,
 *        and must not be misrepresented as being the original software.
 * 
 *    3.  This notice may not be removed or altered from any
 *        source distribution.
 */

#ifndef ARCANOID_ECS_HPP
#define ARCANOID_ECS_HPP

#include <tktk/ecs/Ecs.hpp>
#include <tktk/util/Signal.hpp>

using namespace tktk;

class ECS;

template< typename CompT >
class CompMgr
: public ecs::CompMgr< CompT >
{
public:

    CompMgr( ECS* ecs ) noexcept
    : ecs::CompMgr< CompT >()
    , mECS{ ecs }
    {
    }

    virtual bool setup() noexcept
    {
        //stub
        return ( false );
    }

    virtual void shutdown() noexcept
    {
        //stub
    }

    inline ECS* getECS() const noexcept
    {
//         return ( static_cast< ECS* >( ecs::CompMgr< CompT >::getEcs() ) );
        return ( mECS );
    }

private:

    ECS* mECS;
};

class ECS
: public ecs::Ecs
{
public:

    util::Signal< float > updateSignal{};

    ECS() noexcept
    : ecs::Ecs()
    {
    }

    virtual ~ECS() noexcept
    {
    }

    virtual bool setup() noexcept override
    {
        bool res{ ecs::Ecs::setup() };
        return ( res );
    }

    virtual void shutdown() noexcept override
    {
        ecs::Ecs::shutdown();
    }

    template< typename CompT, typename MgrT, typename... MgrArgsT >
    bool regCompType( MgrArgsT&&... args ) noexcept
    {
        static_assert(
            std::is_base_of< CompMgr< CompT >, MgrT >::value
            , "MgrT should extend CompMgr<CompT> template"
        );

        return ( tktk::ecs::Ecs::regCompType< CompT, MgrT >( this, std::forward< MgrArgsT >( args )... ) );
    }

};

#endif /* end of include guard: ARCANOID_ECS_HPP */
