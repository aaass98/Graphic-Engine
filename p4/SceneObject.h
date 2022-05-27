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
// OVERVIEW: SceneObject.h
// ========
// Class definition for scene object.
//
// Author(s): Paulo Pagliosa (and your name)
// Last revision: 23/09/2019

#ifndef __SceneObject_h
#define __SceneObject_h

#include "SceneNode.h"
#include "Transform.h"
#include "ComponentList.h"

namespace cg
{ // begin namespace cg

// Forward definition
class Scene;


/////////////////////////////////////////////////////////////////////
//
// SceneObject: scene object class
// ===========
class SceneObject: public SceneNode
{
public:
  bool visible{true};

  /// Constructs an empty scene object.
  SceneObject(const char* name, Scene* scene);

  SceneObject(const char* name, SceneObject* sceneObject);

  SceneObject(const char* name, Scene& scene);

  ~SceneObject();

  /// Returns the scene which this scene object belong to.
  auto scene() const
  {
    return _scene;
  }

  /// Returns the parent of this scene object.
  auto parent() const
  {
    return _parent;
  }

  /// Sets the parent of this scene object.
  void setParent(SceneObject* parent);

  /// Returns the transform of this scene object.
  auto transform() const
  {
    return &_transform;
  }

  auto transform()
  {
    return _transform;
  }

  /// Add a new component to this object's list.
  bool addComponent(Component* component) {
      if (_components->getComponent(component->typeName()) == nullptr) {
          _components->add(component);
          component->_sceneObject = this;
          return true;
      }
      else {
          return false;
      }
  }

  inline Component* getComponent(const char* typeName) {
      return _components->getComponent(typeName);
  }

  /// Remove a component from this object's list.
  inline void removeComponent(Component* component) {
      _components->remove(component);
  }

  //returns a iterator over the child object list
  ComponentListIterator* componentIterator() {
      return new ComponentListIterator(_components);
  }

private:
  Scene* _scene;
  SceneObject* _parent;
  Transform* _transform;
  SceneObject* _previous;
  SceneObject* _next;
  ComponentList* _components;

  friend class Scene;
  friend class SceneObjectList;
  friend class SceneObjectListIterator;
  friend class ComponentList;

}; // SceneObject

/// Returns the transform of a component.
inline Transform*
Component::transform() // declared in Component.h
{
  return sceneObject()->transform();
}

/// Returns the parent of a transform.
inline Transform*
Transform::parent() const // declared in Transform.h
{
  if (auto p = sceneObject()->parent())
    return p->transform();
   return nullptr;
}

} // end namespace cg

#endif // __SceneObject_h
