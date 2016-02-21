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

#include <memory>
#include <tktk/ecs/System.hpp>
#include <tktk/ecs/Component.hpp>
#include <cassert>
#include <utility>

using namespace tktk;

class ECS
: public ecs::System<>
{
public:

    ECS() noexcept
    : ecs::System<>()
    {
    }
    virtual ~ECS() = default;

    ecs::EntityHandle addEntity() noexcept;
    void removeEntity( ecs::EntityHandle& handle ) noexcept;

    template< typename T, typename... TArgs >
    ecs::ComponentHandle< T > addComponent( TArgs&&... args )
    {
        auto processor( getProcessorForCompType< T >() );
        assert( processor && "Processor for given component type is not registered." );

        ecs::ComponentHandle< T > handle{ processor->addComponent( std::forward< TArgs >( args )... ) };

        const uint32_t index{ handle->getEntity().index() };
        mCompByTypeMapList[ index ].insert< T >( handle.getIndex() );

        return ( handle );
    }

    template< typename T >
    void removeComponent( const ecs::EntityHandle& owner )
    {
        // find Component index for given Entity corresponding to given component type T
        const uint32_t index{ owner.getEntity().index() };
        const util::TypeMap< std::size_t >& typeMap{ mCompByTypeMapList[ index ] };
        auto it( typeMap.find< T >() );
        if ( it == typeMap.end() )
        {
            return;
        }
        std::size_t compIndex{ it->second };
        // remove entry of given component type T for given Entity
        mCompByTypeMapList[ index ].remove< T >();

        // destroy the Component instance
        auto processor( getProcessorForCompType< T >() );
        assert( processor && "Processor for given component type is not registered." );
        processor->removeComponent( compIndex );
    }

    template< typename T >
    ecs::ComponentHandle< T > getComponent( const ecs::EntityHandle& owner )
    {
        std::size_t compIndex;
        // find the mapped Component index for given Entity and T
        std::uint32_t index = owner.getEntity().index();
        auto it = mCompByTypeMapList[ index ].find< T >();
        if ( it != mCompByTypeMapList[ index ].end() )
        {
            compIndex = it->second;
        }

        std::shared_ptr< ecs::Processor< T > > processor = getProcessorForCompType< T >();
        ecs::ComponentHandle< T > handle{ compIndex, processor };
        return ( handle );
    }

private:

    std::vector< util::TypeMap< std::size_t > > mCompByTypeMapList;

};

#endif /* end of include guard: ARCANOID_ECS_HPP */
