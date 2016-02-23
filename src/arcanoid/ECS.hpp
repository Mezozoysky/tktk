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
#include <iostream>

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

//     ecs::EntityManager::Handle addEntity() noexcept;
//     void removeEntity( ecs::EntityManager::Handle& handle ) noexcept;

    template< typename T, typename... TArgs >
    typename ecs::Processor< T >::Handle addComponent( ecs::EntityManager::Handle& eHandle, TArgs&&... args )
    {
        auto procPtr( getProcessorForCompType< T >() );
        assert( procPtr && "Processor for given component type is not registered." );

        typename ecs::Processor< T >::Handle cHandle{ procPtr->addComponent( eHandle.getId(), std::forward< TArgs >( args )... ) };

//         const uint32_t index{ handle->getEntity().index() };
//         mCompByTypeMapList[ index ].insert< T >( handle.getIndex() );
        if ( eHandle->map.find< T >() != eHandle->map.end() )
        {
            assert( false && "Type already exists!" );
        }
        eHandle->map.insert< T >( cHandle.getId() );

        return ( cHandle );
    }

    template< typename T >
    void removeComponent( const ecs::EntityManager::Handle& eHandle )
    {
/*        // find Component index for given Entity corresponding to given component type T
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
*/

        std::cout << "[ECS::removeComponent] " << "get inside" << std::endl;

        if ( !eHandle.isValid() )
        {
            std::cout << "[ECS::removeComponent] " << "entity handle is invalid; returning" << std::endl;
            return;
        }
        std::cout << "[ECS::removeComponent] " << "entity handle is valid" << std::endl;
        auto procPtr( getProcessorForCompType< T >() );
        assert( procPtr && "Processor for given component type is not registered." );
        std::cout << "[ECS::removeComponent] " << "processor retreived" << std::endl;
        auto it = eHandle->map.find< T >();
        if ( it == eHandle->map.end() )
        {
            std::cout << "[ECS::removeComponent] " << "entity has not cId for type T" << std::endl;
        }
        else
        {
            std::cout << "[ECS::removeComponent] " << "entity has cId for T" << std::endl;
            util::Id64 cId{ eHandle->map.find< T >()->second };
            std::cout << "[ECS::removeComponent] " << "comp id retreived: " << ( cId.index() ) << std::endl;
            procPtr->destroyElement( cId );
            std::cout << "[ECS::removeComponent] " << "component by cId has destroyed" << std::endl;
            eHandle->map.remove< T >();
            std::cout << "[ECS::removeComponent] " << "T removed from entity map" << std::endl;
            auto it = eHandle->map.find< T >();
            if ( it != eHandle->map.end() )
            {
                std::cout << "[ECS::removeComponent] " << "entity still has T->cId in map" << std::endl;
            }
            else
            {
                std::cout << "[ECS::removeComponent] " << "T->cId hs removed from entity's map" << std::endl;
            }
        }
        std::cout << "[ECS::removeComponent] " << "all done; returning" << std::endl;
    }

    template< typename T >
    typename ecs::Processor< T >::Handle getComponent( const ecs::EntityManager::Handle& eHandle )
    {
/*        std::size_t compIndex;
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
*/
        typename ecs::Processor< T >::Handle invalidCHandle{};

        auto it = eHandle->map.find< T >();
        if ( it == eHandle->map.end() )
        {
            return ( invalidCHandle );
        }
        util::Id64 cId = it->second;
        //TODO: util::Id64 cId = handle->getComponent< T >

        ecs::Processor< T >* procPtr{ getProcessorForCompType< T >() };
        assert( procPtr && "Processor for given component type is not registered." );

        if ( !procPtr->isIdValid( cId ) )
        {
            return ( invalidCHandle );
        }

        typename ecs::Processor< T >::Handle cHandle{ cId, procPtr };
        return ( cHandle );
    }

private:

//     std::vector< util::TypeMap< std::size_t > > mCompByTypeMapList;

};

#endif /* end of include guard: ARCANOID_ECS_HPP */
