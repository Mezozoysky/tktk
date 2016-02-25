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
    /// plain Component::Handle
    struct Handle
    {
        Handle()
        {
        }

        Handle( const util::Id64& cId, Processor* procPtr ) noexcept;

        bool isValid() const noexcept;
        void invalidate() noexcept;

        inline util::Id64 getId() const noexcept
        {
            return ( mId );
        }

        inline Processor* getProcessor() const noexcept
        {
            return ( mProcPtr );
        }

        //         void remove() noexcept;

        Component* operator ->() const noexcept;

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
    using Type = CompT;
    using BasalType = Comp< Type >;

    /// plain Component::Handle
    struct Handle
    {
        Handle()
        {
        }

        /// Construct with untyped Component::Handle
        Handle( const Component::Handle& ucHandle )
        : mUntypedCHandle{ ucHandle }
        {
        }

        Handle( const util::Id64& cId, Processor* procPtr ) noexcept
        : mUntypedCHandle{ cId, procPtr }
        {
//             Component::Handle untracked{ cId, procPtr };
        }

        inline bool isValid() const noexcept
        {
            return ( mUntypedCHandle.isValid() );
        }

        inline void invalidate() noexcept
        {
            mUntypedCHandle.invalidate();
        }

        inline util::Id64 getId() const noexcept
        {
            return ( mUntypedCHandle.getId() );
        }

        inline Proc< Type >* getProcessor() const noexcept
        {
            return ( static_cast< Proc< Type >* >( mUntypedCHandle.getProcessor() ) );
        }

        //         void remove() noexcept;

        inline Type* operator ->() const noexcept
        {
            return ( static_cast< Type* >( mUntypedCHandle.operator->() ) );
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


// struct ComponentHandleBase
// {
//     bool isValid() const noexcept;
//     void invalidate() noexcept;
// };

/*
template < typename T >
struct ComponentHandle
{
    using CompType = T;

    ComponentHandle( std::size_t index, typename std::shared_ptr< ecs::Processor< T > > processor )
    : mIndex{ index }
    , mProcessor{ processor }
    {
    }

    inline bool isValid() const noexcept
    {
        return ( mProcessor && mProcessor->isComponentAlive( mIndex ) );
    }

    void invalidate() noexcept
    {
        mIndex = -1;
        mProcessor = nullptr;
    }

    inline CompType* operator ->() const noexcept
    {
        return ( mProcessor->getPtr( mIndex ) );
    }

    inline std::size_t getIndex() const noexcept
    {
        return ( mIndex );
    }


private:

    std::size_t mIndex{ 0 };
    std::shared_ptr< ecs::Processor< T > > mProcessor{ nullptr }; //TODO: it should be rather raw pointer
};
*/

} //namespace ecs
} //namespace tktk

#endif /* end of include guard: TKTK_ECS_COMPONENT_HPP */
