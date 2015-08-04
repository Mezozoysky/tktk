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

#ifndef TKTK_SCENE2D_NODE_HPP
#define TKTK_SCENE2D_NODE_HPP

#include <list>
#include <memory>

namespace tktk
{
namespace scene2d
{

class Node;
using NodeUPtr = std::unique_ptr<Node>;

class Node
{
public:

    Node();
    virtual ~Node();

    bool isEnabled() const noexcept;
    void setEnabled( bool isEnabled ) noexcept;

    virtual void update();
    virtual void updateStep( float deltaTime );
    virtual void redraw();

    virtual void onUpdate();
    virtual void onUpdateStep( float deltaTime );
    virtual void onRedraw();

    virtual void onAttached();
    virtual void onDetach();
    // virtual void onSubscribe() { /*stub*/ };
    // virtual void onUnsubscribe() { /*stub*/ };
    virtual void onEnabled();
    virtual void onDisabled();

    Node* getParent() const noexcept;
    void setParent( Node* node ) noexcept;

    void attachChild( Node* node ) noexcept;
    void detachChild( Node* node ) noexcept;

    virtual void clear() noexcept;

private:

    Node* mParentPtr{ nullptr };
    std::list< NodeUPtr > mChildren;

    bool mIsEnabled{ true };
    std::size_t mId;

};

} //namespace scene2d
} //namespace tktk

#endif /* end of include guard: TKTK_SCENE2D_NODE_HPP */
