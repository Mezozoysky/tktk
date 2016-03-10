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
/// \brief This file provides 'component' of 'entity-component system'
/// \author Stanislav Demyanovich <stan@angrybubo.com>
/// \date 2015-2016
/// \copyright tktk is released under the terms of zlib/png license; see full license text at https://opensource.org/licenses/Zlib

#ifndef TKTK_ECS_COMPONENT_HPP
#define TKTK_ECS_COMPONENT_HPP

#include <tktk/Config.hpp>
#include <tktk/util/Id64.hpp>
#include <memory>
#include <string>

namespace tktk
{
namespace ecs
{

// forward declarations
class Processor;

template < typename T >
class Proc;


/// \brief Plain base class for Comp<Compt> template
struct Component
{
    /// \brief Component value-type handle for copying and passing away
    struct Handle
    {
        /// \brief Default constructor constructs an invalide handle
        Handle() noexcept;

        /// \brief Constructor from proc and id
        Handle( const util::Id64& cId, Processor* procPtr ) noexcept;

        /// \brief Access to handling component via pointer
        /// \returns The handling component pointer if handle is valid, nullptr othervise
        /// Behaves like an std smart pointers operator->()
        /// \code
        /// ecs::System ecSystem;
        /// auto e( ecSystem.addEntity() );
        /// auto c( e.addComp< SomeCompType >() );
        /// if ( c->isValid() )   // here
        /// {
        ///     // do something this component
        /// }
        /// \endcode
        Component* operator ->() const noexcept;

        /// \brief Returns handling component id
        inline util::Id64 getId() const noexcept
        {
            return ( mId );
        }

        /// \brief Returns the corresponding proc pointer
        inline Processor* getProc() const noexcept
        {
            return ( mProcPtr );
        }

        /// \brief Returns true if component is valid, false othervise
        bool isValid() const noexcept;
        /// \brief Makes handle invalid
        void invalidate() noexcept;

    private:
        util::Id64 mId { util::ID64_INVALID }; ///< \brief Handling component's id; invalid by default
        Processor* mProcPtr{ nullptr }; ///< \brief Pointer to the according processor
    };

    /// \brief Explitit constructor from entity id
    explicit Component( const util::Id64& entityId ) noexcept;

    /// \brief Virtual destructor for further inheritance
    virtual ~Component() noexcept;

    /// \brief Gets the owner entity id
    inline const util::Id64& getEntityId() const noexcept
    {
        return mEntityId;
    }

private:
    /// \brief Owner entity id
    util::Id64 mEntityId;
};

/// \brief Base class for all component You want
template< typename CompT >
struct Comp
: public Component
{
    /// \brief Wrapper for derived component type
    using CompTypeT = CompT;
    /// \brief Wrapper for base component type
    using BaseTypeT = Comp< CompTypeT >;

    /// typed Comp<CompT>::Handle
    struct Handle
    {
        /// \brief Default constructor constructs an invalid handle
        Handle() noexcept;

        /// \brief Constructor from untyped Component::Handle
        Handle( const Component::Handle& ucHandle ) noexcept;

        /// \brief Constructor from id and processor pointer
        Handle( const util::Id64& cId, Processor* procPtr ) noexcept;

        /// \brief Access to handling component via pointer
        /// \returns The handling component pointer if handle is valid, nullptr othervise
        /// Behaves like an std smart pointers operator->()
        /// \code
        /// ecs::System ecSystem;
        /// auto e( ecSystem.addEntity() );
        /// auto trform( e.addComp< Transform >() );
        /// trform->x = 8   // here
        /// trform->y = 16  // and here
        /// \endcode
        inline CompTypeT* operator ->() const noexcept
        {
            return ( static_cast< CompTypeT* >( mUntypedCHandle.operator->() ) );
        }

        /// \brief Returns the id of the handling component
        inline util::Id64 getId() const noexcept
        {
            return ( mUntypedCHandle.getId() );
        }

        /// \brief Returns pointer to the processor, which created this handle
        inline Proc< CompTypeT >* getProc() const noexcept
        {
            return ( static_cast< Proc< CompTypeT >* >( mUntypedCHandle.getProc() ) );
        }

        /// \brief Returns true if handle is valid ("points" to alive component), false othervise
        inline bool isValid() const noexcept
        {
            return ( mUntypedCHandle.isValid() );
        }

        /// \brief Makes handle invalid
        inline void invalidate() noexcept
        {
            mUntypedCHandle.invalidate();
        }

        /// \brief Returns inner untyped handle
        inline Component::Handle getUntyped() const noexcept
        {
            return ( mUntypedCHandle );
        }

    private:
        Component::Handle mUntypedCHandle{ util::ID64_INVALID, nullptr }; ///< Plain component handle
    };

    /// \brief Excplicit constructor from entity id
    explicit Comp( const util::Id64& entityId ) noexcept;

    /// \brief Virtual destructor for further inheritance
    virtual ~Comp() noexcept;
};

//
// Template-methods realisations
//

// Comp<CompT>

template < typename CompT >
Comp< CompT >::Comp( const util::Id64& entityId ) noexcept
: Component{ entityId }
{
}

template < typename CompT >
Comp< CompT >::~Comp() noexcept
{
}

// Comp<CompT>::Handle

template < typename CompT >
Comp< CompT >::Handle::Handle() noexcept
{
}

template < typename CompT >
Comp< CompT >::Handle::Handle( const Component::Handle& ucHandle ) noexcept
: mUntypedCHandle{ ucHandle }
{
}

template < typename CompT >
Comp< CompT >::Handle::Handle( const util::Id64& cId, Processor* procPtr ) noexcept
: mUntypedCHandle{ cId, procPtr }
{
}


} //namespace ecs
} //namespace tktk

#endif /* end of include guard: TKTK_ECS_COMPONENT_HPP */
