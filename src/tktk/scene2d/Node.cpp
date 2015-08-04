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

#include <tktk/scene2d/Node.hpp>
#include <iostream>

namespace
{

inline std::size_t getUniqueNodeId()
{
    static std::size_t lastId{ 0u };
    return ( lastId++ );
}

} //namespace

namespace tktk
{
namespace scene2d
{

Node::Node()
: mId{ getUniqueNodeId() }
{
    std::cout << "Node[ " << mId << " ] CTOR" << std::endl;
}

Node::~Node()
{
    std::cout << "Node[ " << mId << " ]  DTOR" << std::endl;
}

bool Node::isEnabled() const noexcept
{
    return ( mIsEnabled );
}

void Node::setEnabled( bool isEnabled ) noexcept
{
    if ( mIsEnabled != isEnabled )
    {
        mIsEnabled = isEnabled;
        mIsEnabled ? onEnabled() : onDisabled();
    }
}

void Node::update()
{
    onUpdate();

    for ( auto& node: mChildren )
    {
        node->update();
    }
}

void Node::updateStep( float deltaTime )
{
    onUpdateStep( deltaTime );

    for ( auto& node: mChildren )
    {
        node->updateStep( deltaTime );
    }
}

void Node::redraw()
{
    onRedraw();

    for ( auto& node: mChildren )
    {
        node->redraw();
    }
}

void Node::onUpdate()
{
    // std::cout << "NODE [ " << mId << " ] onUpdate()" << std::endl;
}

void Node::onUpdateStep( float deltaTime )
{
    // std::cout << "NODE [ " << mId << " ] onUpdateStep()" << std::endl;
}

void Node::onRedraw()
{
    // std::cout << "NODE [ " << mId << " ] onRedraw()" << std::endl;
}

void Node::onAttached()
{ /*stub*/
}
void Node::onDetach()
{ /*stub*/
}
void Node::onEnabled()
{ /*stub*/
}
void Node::onDisabled()
{ /*stub*/
}

Node* Node::getParent() const noexcept
{
    return ( mParentPtr );
}

void Node::setParent( Node* node ) noexcept
{
    mParentPtr = node;
}

void Node::attachChild( Node* node ) noexcept
{
    mChildren.emplace_back( node );
    node->mParentPtr = this;
    node->onAttached();
}

void Node::detachChild( Node* node ) noexcept
{
    //TODO: make sure detachChild method is really needed
    node->onDetach();

    node->mParentPtr = nullptr; // ??

    mChildren.remove_if(
        [=]( const NodeUPtr& value ) -> bool
        {
            return ( value.get() == node );
        }
    );
}

void Node::clear() noexcept
{
    //TODO: implement clearing the node
}

} //namespace scene2d
} //namespace tktk
