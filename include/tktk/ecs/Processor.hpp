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

#ifndef TKTK_ECS_PROCESSOR_HPP
#define TKTK_ECS_PROCESSOR_HPP

#include <tktk/Config.hpp>
#include <tktk/util/MemoryPool.hpp>
#include <tktk/ecs/Component.hpp>
#include <memory>

namespace tktk
{
namespace ecs
{

// Forward declarations
struct Entity;
class System;



class Processor
{
    Processor( const Processor& ) = delete;
    Processor& operator= ( const Processor& ) = delete;

public:

    Processor() noexcept = default;
    virtual ~Processor() noexcept = default;

    virtual void setup( System* systemPtr ) = 0;

    virtual bool isIdValid( const util::Id64& id ) const noexcept = 0;

    virtual Component* getCompPtr( const util::Id64& id ) const noexcept = 0;

    virtual void destroyElement( const util::Id64& id ) noexcept = 0;
};


template< typename CompT >
class Proc
: public Processor
{

public:
    /// \brief Type of processor's components
    using CompTypeT = CompT;

    Proc( System* systemPtr ) noexcept
    : Processor()
    , mSystemPtr{ systemPtr }
    {
    }

    virtual ~Proc() noexcept
    {
    }

    /// \brief Adds component with constructructor arguments \em args and returns a handle of type CompTypeT::Handle
    template< typename... ArgsT >
    typename CompTypeT::Handle addComp( ArgsT&&... args )
    {
        util::Id64 cId{ mPool.createElement( std::forward< ArgsT >( args )... ) };

        typename CompTypeT::Handle cHandle( cId, this );
        return ( cHandle );
    }

    /// \brief Marks the 'pointed' component as destroyed and invalidates the handle if handle is valid
    void removeComp( typename CompTypeT::Handle& cHandle )
    {
        if ( cHandle.isValid() )
        {
            destroyElement( cHandle.getId() );
            cHandle.invalidate();
        }
    }

    /// \brief Returns true if given id is valid
    virtual bool isIdValid( const util::Id64& cId ) const noexcept override final
    {
        return ( mPool.isIdValid( cId ) );
    }

    /// \brief Returns raw pointer to the component with id \em cId
    inline virtual CompTypeT* getCompPtr( const util::Id64& cId ) const noexcept override final
    {
        return ( mPool.getPtr( cId.index() ) );
    }

    ///\brief Marks component with id \em cId as 'destroyed'
    inline virtual void destroyElement( const util::Id64& cId ) noexcept override final
    {
        mPool.destroyElement( cId );
    }

protected:
    using PoolTypeT = util::MemoryPool<CompTypeT>;
    PoolTypeT mPool;
    System* mSystemPtr{ nullptr };
};

} //namespace ecs
} //namespace tktk

#endif /* end of include guard: TKTK_ECS_PROCESSOR_HPP */
