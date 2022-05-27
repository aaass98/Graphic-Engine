//[]---------------------------------------------------------------[]
//|                                                                 |
//| Copyright (C) 2019 Orthrus Group.                               |
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
// OVERVIEW: SceneEditor.cpp
// ========
// Source file for scene editor.
//
// Author(s): Paulo Pagliosa (and your name)
// Last revision: 23/09/2019

#include "SceneEditor.h"

namespace cg
{ // begin namespace cg


/////////////////////////////////////////////////////////////////////
//
// SceneEditor impementation
// ===========
void
SceneEditor::setDefaultView(float aspect)
{
  auto t = _camera->transform();

  t->reset();
  t->setLocalPosition({0, 0, 10});
  _camera->reset(aspect);
}

void
SceneEditor::zoom(float s)
{
  if (s > 0)
    if (_camera->projectionType() == Camera::Perspective)
      _camera->setViewAngle(_camera->viewAngle() / s);
    else
      _camera->setHeight(_camera->height() / s);
}

void
SceneEditor::rotateView(float ax, float ay)
{
  auto t = _camera->transform();
  auto q = quatf{ay, vec3f{0, 1, 0}} * t->rotation();

  t->setRotation(quatf{ax, q.rotate({1, 0, 0})} * q);
}

void
SceneEditor::orbit(float ax, float ay)
{
  auto t = _camera->transform();
  auto p = t->position() - t->forward() * _orbitDistance;

  rotateView(ax, ay);
  t->setPosition(p + t->forward() * _orbitDistance);
}

void
SceneEditor::pan(const vec3f& d)
{
  _camera->transform()->translate(d, Transform::Space::Local);
}

void
SceneEditor::requestFocus(vec3f objectPosition) {
    ////Make editor camera face object
    //we start by finding the distance beetwen the editor and the object
    Transform* t = _camera->transform();
    vec3f distanceVector = objectPosition - t->position();
    distanceVector = distanceVector.normalize();

    //now with some simple trigonometry we rotate on x and y axis
    vec3f targetRotation = t->localEulerAngles();
    targetRotation.x = asin(distanceVector.y) * 180 / M_PI;
    targetRotation.y = asin(-distanceVector.x) * 180 / M_PI;

    t->setLocalEulerAngles(targetRotation);

    //Move camera along z axis through some distance
    vec3f targetPosition = objectPosition + 10 * t->forward();
    _camera->transform()->setLocalPosition(targetPosition);
}

void
SceneEditor::newFrame()
{
  const auto& bc = _scene->backgroundColor;

  glClearColor(bc.r, bc.g, bc.b, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  setView(_camera->transform()->position(), vpMatrix(_camera));
  if (showGround)
    drawXZPlane(10, 1);
}

} // end namespace cg
