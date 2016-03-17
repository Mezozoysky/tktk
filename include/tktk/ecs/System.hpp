//  tktk
//
//  tktk - Copyright (C) 2015-2016 Stanislav Demyanovich <stan@angrybubo.com>
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
/// \brief This file provides ecs::System and ecs::Entity types
/// \author Stanislav Demyanovich <stan@angrybubo.com>
/// \date 2015-2016
/// \copyright tktk is released under the terms of zlib/png license; see full license text at https://opensource.org/licenses/Zlib


#ifndef TKTK_ECS_SYSTEM_HPP
#define TKTK_ECS_SYSTEM_HPP

#include <tktk/Config.hpp>
#include <type_traits>
#include <memory>
#include <algorithm>
#include <cstdint>
#include <tktk/ecs/Processor.hpp>
#include <tktk/util/TypeMap.hpp>
#include <tktk/util/Signal.hpp>
#include <tktk/mpool/MemoryPool.hpp>
#include <tktk/ecs/Component.hpp>

namespace tktk
{
namespace ecs
{

/// \brief Represents the 'entity' abstraction
///
/// Entity is dumb structure which just wraps the map of handles by component types for attached components.
/// The one can add the new entity into the system by calling `tktk::ecs::System::addEntity()` on the system instance.
struct Entity
{
    /// \brief Handle for entity
    ///
    /// Entities are created and stored in managed memory within `tktk::ecs::System` class, so using the handles to access them.
    /// Entity::Handle acts like 'value-type pointer' to the entity instance in managed memory.
    struct Handle
    {
        /// \brief Default constructor; creates invalid handle
        Handle() noexcept;

        /// \brief Constructor
        /// \param[in] eId The managed memory block id
        /// \param[in] systemPtr pointer to the entity's owner system
        /// \note Unless you are modifying the tktk-ecs library you should not construct handles manually, `System::addEntity()`will do it for you.
        explicit Handle( const mpool::Id64& eId, System* systemPtr ) noexcept;

        /// \brief Removes the entity from owner system
        ///
        /// Actually, this method call will mark entity's memory as 'dead' or 'free' and invalidates the handle itself.
        void remove() noexcept;
        /// \brief Pointer-like access to entity's methods
        /// \returns The raw pointer to the entity's memory
        ///
        /// Example of usage:
        /// \code{.cpp}
        /// #include <tktk/ecs/System>
        /// using namespace tktk;
        /// ...
        /// ecs::System system;
        /// auto handle = system.addEntity();
        /// auto iter = handle->map.begin() // access to the entity's map via handle's operator->()
        /// \endcode
        Entity* operator ->() const noexcept;

        /// \brief Adds component of given type T to the entity
        /// \param[in] args Arguments for component type's constructor
        /// \returns Typed component handle
        ///
        /// Adds new component of the given type, 'attachs' it to the entity and returns the 'typed' component handle
        template< typename T, typename... ArgsT >
        typename T::Handle addComp( ArgsT&&... args ) noexcept;

        /// \brief Gets the component handle of the given type if added
        /// \returns Typed component handle
        ///
        /// If entity has not component of type T added component handle will be invalid.
        template< typename T >
        typename T::Handle getComp() noexcept;

        /// \brief Removes the component of the given type from the entity if added
        ///
        /// Removes the component handle from entity's map and marks removed component's memory as 'free' within it's processor.
        template< typename T >
        void removeComp() noexcept;

        /// \brief Gets entity's id
        inline mpool::Id64 getId() const noexcept
        {
            return ( mId );
        }

        /// \brief Gets pointer to the `System` instance which created the handle
        inline System* getSystem() const noexcept
        {
            return ( mSystemPtr );
        }

        /// \brief Returns true if handle is valid, false othervise
        bool isValid() const noexcept;

        /// \brief Makes handle invalid
        void invalidate() noexcept;



    private:
        mpool::Id64 mId{ mpool::ID64_INVALID }; // id with the system
        System* mSystemPtr{ nullptr }; // pointer to the system
    };

    /// \brief Map for added components: untyped component handles by component types
    /// \todo make it more efficient and more friendly (meybe bitset)
    util::TypeMap< Component::Handle > map;
};


/// \brief Represents the 'entity-component system' as a unit
///
/// System type provides the common functionality of an 'entity-component system':
/// management of entities, processors/components and their relationships.
///
/// Usage example:
/// \code{.cpp}
/// #include <tktk/ecs/System.hpp>
/// ...
/// using namespace tktk;
/// ecs::System system;
/// auto entity( system.addEntity() );
/// entity.remove();
/// ...
/// \endcode
class System
{
public:
    /// \brief Alias for actualy used memory pool type
    using PoolTypeT = mpool::MemoryPool< Entity >;

    /// \brief This signal will be sent to subscribers when somebody call `update(float)` method on instance.
    util::Signal< float > updateSignal;

    /// \brief Default constructor
    ///
    /// Does nothing
    System() noexcept;

    /// \brief Virtual destructor for further inheritance
    ///
    /// Destroys all the registered processors.
    virtual ~System() noexcept;

    /// \brief Initialises the system's inner state
    ///
    /// Calls `ecs::Proc<CompT>::setup( System* )` on the managing processors.
    virtual void setup() noexcept;

    /// \brief Sends `update` signal to all the subscribers
    /// \param[in] deltaTime seconds elapsed from last frame
    /// \todo Should this method be defined with base System class?
    virtual void update( float deltaTime ) noexcept
    {
        updateSignal( deltaTime );
    }

    //
    // About entity
    //

    /// \brief Adds 'new' entity
    /// \returns The handle for added entity.
    ///
    /// If there is no 'dead' entities in the memory the new entity will be created, othervise first of the 'dead' entities will be reused.
    Entity::Handle addEntity() noexcept;

    /// \brief Removes the given entity
    /// \param[in,out] eHandle handle specifies the entity to remove
    ///
    /// Adds the entity specified with handle to the 'dead-list' and invalidates the handle.
    void removeEntity( Entity::Handle& eHandle ) noexcept;

    /// \brief Remove the entity by entity's id
    /// \param[in] eId Specifies the entity to remove
    ///
    /// Adds the entity specified with id to the 'dead-list'.
    /// \note Unless you are modifying the tktk-ecs library you should not use this method, use `Entity::Handle::remove()` instead.
    void removeEntity( const mpool::Id64& eId  ) noexcept;

    /// \brief Tests if handle is valid
    /// \param[in] eHandle The entity handle to test
    /// \returns `true` if handle is valid, `false` othervise
    bool isEntityValid( const Entity::Handle& eHandle ) const noexcept;

    /// \brief Tests if id is valid
    /// \param[in] eId The id to test
    /// \returns `true` if id is valid, `false` othervise
    /// \note Unless you are modifying the tktk-ecs library you should not use this method, use `Entity::Handle::isValid()` instead.
    bool isIdValid( const mpool::Id64& eId ) const noexcept;

    /// \brief Gets the raw pointer to the handling entity
    /// \param[in] eHandle Specifies the entity
    /// \returns The pointer to the entity's memory
    Entity* getEntityPtr( const Entity::Handle& eHandle ) const noexcept;

    /// \brief Gets the raw pointer to the entity by entity's id
    /// \param[in] eId entity's id
    /// \returns The pointer to the entity's memory
    /// \note Unless you are modifying the tktk-ecs library you should not use this method, use `Entity::Handle::operator->()` instead.
    Entity* getEntityPtr( const mpool::Id64& eId ) const noexcept;

    //
    // About components
    //

    /// \brief Adds 'new' component of given type `T` to the entity specified by handle
    /// \param[in] eHandle Specifies the entity to add component to
    /// \param[in] args Arguments for `T` constructor
    /// \returns The handle for the added component
    template< typename T, typename... ArgsT >
    typename T::Handle addComp( const Entity::Handle& eHandle, ArgsT&&... args )
    {
        return ( addComp< T >( eHandle.getId(), std::forward< ArgsT >( args )... ) );
    }

    /// \brief Adds 'new' component of given type `T` to the entity specified by id
    /// \param[in] eId Specifies the entity to add component to
    /// \param[in] args Arguments for `T` constructor
    /// \returns The handle for the added component
    /// \note Unless you are modifying the tktk-ecs library you should not use this method, use `Entity::Handle::addComp<CompT>(...)` instead.
    template< typename T, typename... ArgsT >
    typename T::Handle addComp( const mpool::Id64& eId, ArgsT&&... args )
    {
        typename T::Handle invalidCHandle{};

        if ( !isIdValid( eId ) )
        {
            return ( invalidCHandle );
        }

        auto procPtr( getProcForCompType< T >() );
        assert( procPtr && "Processor for given component type is not registered." );

        typename T::Handle cHandle{ procPtr->addComp( eId, std::forward< ArgsT >( args )... ) };

        Entity* ePtr{ getEntityPtr( eId ) };
        if ( ePtr->map.find< T >() != ePtr->map.end() )
        {
            return ( invalidCHandle );
        }
        ePtr->map.insert< T >( cHandle.getUntyped() );

        return ( cHandle );
    }

    /// \brief Gets the component of given type `T` if specified entity has it added
    /// \param[in] eHandle Specifies the entity to get component from
    /// \returns The handle for the component
    template< typename T >
    typename T::Handle getComp( const Entity::Handle& eHandle )
    {
        return ( getComp< T >( eHandle.getId() ) );
    }

    /// \brief Gets the component of given type `T` if specified entity has it added
    /// \param[in] eId Specifies the entity to get component from
    /// \returns The handle for the component
    /// \note Unless you are modifying the tktk-ecs library you should not use this method, use `Entity::Handle::getComp<CompT>()` instead.
    template< typename T >
    typename T::Handle getComp( const mpool::Id64& eId )
    {
        typename T::Handle invalidCHandle{};

        if ( !isIdValid( eId ) )
        {
            return ( invalidCHandle );
        }

        Entity* ePtr{ getEntityPtr( eId ) };
        auto it = ePtr->map.find< T >();
        if ( it == ePtr->map.end() )
        {
            return ( invalidCHandle );
        }
        Component::Handle ucHandle{ it->second };

        Proc< T >* procPtr{ getProcForCompType< T >() };
        assert( procPtr && "Processor for given component type is not registered." );

        if ( !procPtr->isIdValid( ucHandle.getId() ) )
        {
            return ( invalidCHandle );
        }

        typename T::Handle cHandle{ ucHandle };
        return ( cHandle );
    }

    /// \brief Removes the component of given type `T` from the specified entity
    /// \param[in] eHandle Specifies the entity
    ///
    /// If entity has a component of given type `T`, marks component as 'dead', removes entry for `T` from entiti's components, invalidates the handle.
    template< typename T >
    void removeComp( const Entity::Handle& eHandle )
    {
        removeComp< T >( eHandle.getId() );
    }

    /// \brief Removes the component of given type `T` from the specified entity
    /// \param[in] eId Specifies the entity
    ///
    /// If entity has a component of given type `T`, marks component as 'dead', removes entry for `T` from entity's components.
    /// \note Unless you are modifying the tktk-ecs library you should not use this method, use `Entity::Handle::removeComp<CompT>()` instead.
    template< typename T >
    void removeComp( const mpool::Id64& eId )
    {
        if ( !isIdValid( eId ) )
        {
            return;
        }
        auto procPtr( getProcForCompType< T >() );
        assert( procPtr && "Processor for given component type is not registered." );

        Entity* ePtr{ getEntityPtr( eId ) };
        auto it = ePtr->map.find< T >();
        if ( it == ePtr->map.end() )
        {
            return;
        }

        Component::Handle ucHandle{ ePtr->map.find< T >()->second };
        procPtr->destroyElement( ucHandle.getId() );
        ePtr->map.erase( it );
    }

    //
    // About processors
    //

    /// \brief Registers the processor of given type `T` and creates the instance
    /// \param[in] args Arguments for `T` constructor
    /// \returns The raw pointer to the instance
    template< typename T, typename... ArgsT >
    T* registerProc( ArgsT&&... args ) noexcept
    {
        static_assert(
                      std::is_base_of< Proc< typename T::CompTypeT >, T >::value
                      , "T should extend tktk::ecs::Proc<> template"
        );

        if ( mProcsByType.count< typename T::CompTypeT >() == 1 )
        {
            ll_error( "Trying to register Processor which is already registered!" );
            return ( nullptr );
        }

        Processor* procPtr{ new T( this, std::forward< ArgsT >( args )... ) };
        mProcessors.push_back( procPtr );
        mProcsByType.insert< typename T::CompTypeT >( mProcessors.size() - 1 );

        return ( static_cast< T* >( procPtr ) );
    }

    /// \brief Gets the processor instance of given type `T`
    /// \returns The raw pointer to the instance if `T` is registered, `nullptr` othervise
    template< typename T >
    T* getProc() noexcept
    {
        static_assert(
                      std::is_base_of< Proc< typename T::CompTypeT >, T >::value
                      , "T should extend tktk::ecs::Proc<> template"
        );

        static std::int32_t cachedProcIndex{ -1 };
        T* procPtr{ nullptr };
        if ( cachedProcIndex < 0 /*|| shouldRecacheIndices */ )
        {
            auto it( mProcsByType.find< typename T::CompTypeT >() );
            if ( it == mProcsByType.end() )
            {
                return ( procPtr ); //nullptr
            }
            cachedProcIndex = it->second;
        }
        return ( static_cast< T* >( mProcessors[ cachedProcIndex ] ) );
    }

    /// \brief Gets the processor instance by given processor's component type `CompT`
    /// \returns The raw pointer to the instance if according processor type is registered, `nullptr` othervise
    template< typename CompT >
    Proc< CompT >* getProcForCompType() noexcept
    {
        static_assert(
            std::is_base_of< Processor, Proc< CompT > >::value
            , "Proc< CompT > type derived from Processor doesnt exist for given CompT"
        );

        static std::int32_t cachedProcIndex{ -1 };
        Proc<CompT>* procPtr{ nullptr };
        if ( cachedProcIndex < 0 /*|| shouldRecacheIndices */ )
        {
            auto it( mProcsByType.find< CompT >() );
            if ( it == mProcsByType.end() )
            {
                return ( procPtr );
            }
            cachedProcIndex = it->second;
        }
        return ( static_cast< Proc< CompT >* >( mProcessors[ cachedProcIndex ] ) );
    }

protected:
    PoolTypeT mEntityPool; ///< Memory pool for storing entities

    std::vector< Processor* > mProcessors; ///< Registered processors in registering order
    util::TypeMap< std::size_t > mProcsByType; ///< Processors' indexes by accoding component types map
};

//
// Template-methods realisations
//

// Entity::Handle

template< typename T, typename... ArgsT >
typename T::Handle Entity::Handle::addComp( ArgsT&&... args ) noexcept
{
    return ( mSystemPtr->addComp< T >( mId, std::forward< ArgsT >( args )... ) );
}

template< typename T >
typename T::Handle Entity::Handle::getComp() noexcept
{
    return ( mSystemPtr->getComp< T >( mId ) );
}

template< typename T >
void Entity::Handle::removeComp() noexcept
{
    mSystemPtr->removeComp< T >( mId );
}

} //namespace ecs
} //namespace tktk

#endif /* end of include guard: TKTK_ECS_SYSTEM_HPP */
