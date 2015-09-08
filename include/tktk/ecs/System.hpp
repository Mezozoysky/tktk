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

#ifndef TKTK_ECS_SYSTEM_HPP
#define TKTK_ECS_SYSTEM_HPP

#include <type_traits>
#include <memory>
#include <algorithm>
#include <tktk/ecs/ProcessorBase.hpp>
#include <tktk/ecs/EntityManager.hpp>

namespace tktk
{
namespace ecs
{

class System
{
public:

    System();
    virtual ~System();

    virtual void update( float deltaTime );

    EntityManager* getEntityManager() const noexcept;

//     template< typename T >
//     T* getProcessor()
//     {
//         static_assert(
//             std::is_base_of< ProcessorBase, T >::value
//             , "T should extend tktk::ecs::ProcessorBase"
//         );
//
//         auto it(
//             std::find_if(
//                 mProcessors.begin()
//                 , mProcessors.end()
//                 , []( std::unique_ptr< ProcessorBase >& processor ) { return ( T::uniqueSystemType == system->getUniqueSystemType() ); }
//             )
//         );
//     }

    template< typename T, typename... TArgs >
    T* addProcessor( TArgs&&... args )
    {
        static_assert(
            std::is_base_of< ProcessorBase, T >::value
            , "T should extend tktk::ecs::ProcessorBase"
        );

        T* processor{ new T( std::forward< TArgs >( args )... ) };

        std::unique_ptr< T > uPtr{ processor };
        mProcessors.emplace_back( std::move( uPtr ) );

        return ( processor );
    }

private:

    std::vector< std::unique_ptr< ProcessorBase > > mProcessors;
    std::unique_ptr<EntityManager> mEntityManager;
};

} //namespace ecs
} //namespace tktk

#endif /* end of include guard: TKTK_ECS_SYSTEM_HPP */
