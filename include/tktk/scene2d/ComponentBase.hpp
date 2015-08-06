/**
    tktk

    tktk - Copyright (C) 2015 Stanislav Demyanovich <stan@angrybubo.com>

    This software is provided 'as-is', without any express or
    implied warranty. In no event will the authors be held
    liable for any damages arising from the use of this software.

    Permission is granted to anyone to use this software for any purpose,
    including commercial applications, and to alter it and redistribute
    it freely, subject to the following restrictions:

    1. The origin of this software must not be misrepresented;
       you must not claim that you wrote the original software.
       If you use this software in a product, an acknowledgment
       in the product documentation would be appreciated but
       is not required.

    2. Altered source versions must be plainly marked as such,
       and must not be misrepresented as being the original software.

    3. This notice may not be removed or altered from any
       source distribution.
*/

#ifndef TKTK_SCENE2D_COMPONENT_BASE_HPP
#define TKTK_SCENE2D_COMPONENT_BASE_HPP

#include <memory>

namespace tktk
{
namespace scene2d
{

class SceneObject;
class ComponentBase;

using ComponentBaseUPtr = std::unique_ptr< ComponentBase >;

using UniqueCompType = std::string;

#define PROMISE_UNIQUE_COMP_TYPE \
public: \
    virtual const ::tktk::scene2d::UniqueCompType& getUniqueCompType() const noexcept = 0; \
private: \

#define DECLARE_UNIQUE_COMP_TYPE \
public: \
    static const ::tktk::scene2d::UniqueCompType uniqueCompType; \
    inline virtual const ::tktk::scene2d::uniqueCompType& getUniqueCompType() const noexcept override final \
    { \
        return ( uniqueCompType ); \
    } \

#define INITIALIZE_UNIQUE_COMP_TYPE(CLASS) \
const ::tktk::scene2d::UniqueCompType CLASS::uniqueCompType = #CLASS;

class ComponentBase
{
PROMISE_UNIQUE_COMP_TYPE

public:

    virtual ~ComponentBase()
    {
    }

    void setSceneObject( SceneObject* sco ) noexcept;
    SceneObject& getSceneObject() const noexcept;

    virtual void onInit() = 0;
    virtual void onUpdate() = 0;
    virtual void onUpdateStep( float deltaTime ) = 0;
    virtual void onRedraw() = 0;

private:

    SceneObject* mSCO{ nullptr };
};

} //namespace scene2d
} //namespace tktk

#endif /* end of include guard: TKTK_SCENE2D_COMPONENT_BASE_HPP */
