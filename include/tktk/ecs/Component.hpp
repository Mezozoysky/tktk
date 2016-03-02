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

#ifndef TKTK_ECS_COMPONENT_HPP
#define TKTK_ECS_COMPONENT_HPP

#include <tktk/Config.hpp>
#include <tktk/util/MemoryPool.hpp> // for Id64 type
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

struct Component
{
    /// untyped Component::Handle
    struct Handle
    {
        Handle()
        {
        }

        Handle( const util::Id64& cId, Processor* procPtr ) noexcept;

        Component* operator ->() const noexcept;

        inline util::Id64 getId() const noexcept
        {
            return ( mId );
        }

        inline Processor* getProc() const noexcept
        {
            return ( mProcPtr );
        }

        bool isValid() const noexcept;
        void invalidate() noexcept;

    private:
        util::Id64 mId { util::ID64_INVALID };
        Processor* mProcPtr{ nullptr };
    };

    Component() = default;
    virtual ~Component() = default;

    virtual const util::Id64& getEntityId() const noexcept = 0;
};

template< typename CompT >
struct Comp
: public Component
{
    using CompTypeT = CompT;
    using BaseTypeT = Comp< CompTypeT >;

    /// typed Component<CompT>::Handle
    struct Handle
    {
        Handle()
        {
        }

        /// Construct with untyped Component::Handle
        Handle( const Component::Handle& ucHandle ) noexcept
        : mUntypedCHandle{ ucHandle }
        {
        }

        /// Construct with id and processor pointer
        Handle( const util::Id64& cId, Processor* procPtr ) noexcept
        : mUntypedCHandle{ cId, procPtr }
        {
        }

        /// Returns the pointer to the handling component handle is valid, returns nullptr othervise
        inline CompTypeT* operator ->() const noexcept
        {
            return ( static_cast< CompTypeT* >( mUntypedCHandle.operator->() ) );
        }

        /// Returns the id of the handling component
        inline util::Id64 getId() const noexcept
        {
            return ( mUntypedCHandle.getId() );
        }

        /// Returns pointer to the processor, which created this handle
        inline Proc< CompTypeT >* getProc() const noexcept
        {
            return ( static_cast< Proc< CompTypeT >* >( mUntypedCHandle.getProc() ) );
        }

        /// Returns true if handle is valid ("points" to alive component), false othervise
        inline bool isValid() const noexcept
        {
            return ( mUntypedCHandle.isValid() );
        }

        /// Makes handle invalid
        inline void invalidate() noexcept
        {
            mUntypedCHandle.invalidate();
        }

        inline Component::Handle getUntyped() const noexcept
        {
            return ( mUntypedCHandle );
        }

    private:
        Component::Handle mUntypedCHandle{ util::ID64_INVALID, nullptr };
    };

    explicit Comp( const util::Id64& entityId ) noexcept
    : mEntityId{ entityId }
    {
    }

    virtual ~Comp()
    {
    }

    virtual const util::Id64& getEntityId() const noexcept override final
    {
        return mEntityId;
    }

private:
    util::Id64 mEntityId;
};


} //namespace ecs
} //namespace tktk

#endif /* end of include guard: TKTK_ECS_COMPONENT_HPP */
