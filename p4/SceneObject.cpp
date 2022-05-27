//[]---------------------------------------------------------------[]
//|                                                                 |
//| Copyright (C) 2018, 2019 Orthrus Group.                         |
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
// OVERVIEW: SceneObject.cpp
// ========
// Source file for scene object.
//
// Author(s): Paulo Pagliosa (and your name)
// Last revision: 07/09/2019

#include "SceneObject.h"
#include "Scene.h"

namespace cg
{ // begin namespace cg


/////////////////////////////////////////////////////////////////////
//
// SceneObject implementation
// ===========
    SceneObject::SceneObject(const char* name, Scene* scene) :
        SceneNode{ name },
        _scene{ scene },
        _parent{}
    {
        _previous = _next = nullptr;
        scene->addChildSceneObject(this);
        _components = new ComponentList(this);
    }

    SceneObject::SceneObject(const char* name, Scene& scene) :
        SceneNode{ name },
        _scene{ &scene },
        _parent{}
    {
        _previous = _next = nullptr;
        _scene->addChildSceneObject(this);
        _components = new ComponentList(this);
    }

    SceneObject::SceneObject(const char* name, SceneObject* sceneObject) :
        SceneNode{ name },
        _scene{ sceneObject->_scene },
        _parent{ sceneObject }
    {
        _previous = _next = nullptr;
        sceneObject->addChildSceneObject(this);
        _components = new ComponentList(this);
    }

    void
        SceneObject::setParent(SceneObject* parent)
    {
        SceneObject* oldParent = _parent;
        this->_parent = parent;

        //in case no change was made
        if (parent == oldParent) {
            return;
        }

        //temporary makeuse to avoid unintentional delete
        SceneObject::makeUse(this);

        if (oldParent) {
            oldParent->removeChildSceneObject(this);
        }
        else {
            _scene->removeChildSceneObject(this);
        }

        if (parent) {
            parent->addChildSceneObject(this);
        }
        else {
            _scene->addChildSceneObject(this);
        }

        //removing temporary makeuse
        SceneObject::release(this);

        this->transform()->update();
    }

    SceneObject::~SceneObject() {
        delete _components;
    }

} // end namespace cg
