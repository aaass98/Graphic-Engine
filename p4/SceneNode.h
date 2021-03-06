//[]---------------------------------------------------------------[]
//|                                                                 |
//| Copyright (C) 2018 Orthrus Group.                               |
//|                                                                 |
//| This software is provided 'as-is', without any express or       |
//| implied warranty. In no event will the authors be held liable   |
//| for any damages arising from the use of this software.          |
//|                                                                 |
//| Permission is granted to anyone to use this software for any    |
//| purpose, including commercial applications, and to alter it and |
//| redistribute it freely, subject to the following restrictions:  |
//|                                                                 |
//| 1. The origin of this software must not be misrepresented; you  |
//| must not claim that you wrote the original software. If you use |
//| this software in a product, an acknowledgment in the product    |
//| documentation would be appreciated but is not required.         |
//|                                                                 |
//| 2. Altered source versions must be plainly marked as such, and  |
//| must not be misrepresented as being the original software.      |
//|                                                                 |
//| 3. This notice may not be removed or altered from any source    |
//| distribution.                                                   |
//|                                                                 |
//[]---------------------------------------------------------------[]
//
// OVERVIEW: SceneNode.h
// ========
// Class definition for generic scene node.
//
// Author(s): Paulo Pagliosa (and your name)
// Last revision: 25/08/2018

#ifndef __SceneNode_h
#define __SceneNode_h

#include "core/NameableObject.h"
#include "SceneObjectList.h"

namespace cg
{ // begin namespace cg


/////////////////////////////////////////////////////////////////////
//
// SceneNode: generic scene node class
// =========
    class SceneNode : public NameableObject
    {
    public:
        using NameableObject::NameableObject;

        /// Adds a new child SceneObject
        inline void addChildSceneObject(SceneObject* child) {
            _childs->add(child);
        }

        /// removes a child SceneObject
        inline void removeChildSceneObject(SceneObject* child) {
            _childs->remove(child);
        }

        //returns a iterator over the child object list
        SceneObjectListIterator* objectIterator() {
            return new SceneObjectListIterator(_childs);
        }

        inline int getChildCount() {
            return _childs->getCount();
        }

        template <typename T>
        T* as()
        {
            return dynamic_cast<T*>(this);
        }

        template <typename T>
        const T* as() const
        {
            return dynamic_cast<const T*>(this);
        }

    protected:
        SceneObjectList* _childs;

        SceneNode() : NameableObject{ "New Scene" } {

            _childs = new SceneObjectList(this);
        }

        SceneNode(const char* name) :
            NameableObject{ name }
        {
            _childs = new SceneObjectList(this);
        }

        ~SceneNode() {
            delete _childs;
        }

    }; // SceneNode

} // end namespace cg

#endif // __SceneNode_h
