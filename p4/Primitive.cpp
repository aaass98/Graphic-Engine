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
// OVERVIEW: Primitive.cpp
// ========
// Source file for primitive.
//
// Author(s): Paulo Pagliosa (and your name)
// Last revision: 30/10/2018

#include "Primitive.h"
#include "Transform.h"

namespace cg
{ // begin namespace cg
bool intersectTriangle(const Ray& ray, const vec3f& v0, const vec3f& v1, const vec3f& v2, float& distance, float& b1, float& b2) 
{
    //calculate b1 and b2
    auto e1 = v1 - v0;
    auto e2 = v2 - v0;
    auto s1 = ray.direction.cross(e2);

    auto s1e1 = s1.dot(e1);
    if (s1e1==0)
        return false;

    auto s = ray.origin - v0;
    auto s2 = s.cross(e1);
    distance = s2.dot(e2) / s1e1;
    if (distance < 0)
        return false;

    b1 = s1.dot(s) / s1e1;

    b2 = s2.dot(ray.direction) / s1e1;
    if (b2 < 0 || b1 < 0 || b1 + b2 >1)
        return false;

    return true;
}

bool intersectMesh(const Ray& ray, const TriangleMesh& mesh, Intersection& hit) {
    auto& data = mesh.data();
    bool isHitting = false;

    hit.distance = ray.tMax;//math::Limits<float>::inf();
    for (int i = 0; i < data.numberOfTriangles; i++) {
        auto n = data.triangles + i;
        const auto& v0 = data.vertices[n->v[0]];
        const auto& v1 = data.vertices[n->v[1]];
        const auto& v2 = data.vertices[n->v[2]];
        float d, b1, b2;

        if (intersectTriangle(ray, v0, v1, v2, d, b1, b2) && d < hit.distance) {
            isHitting = true;
            hit.triangleIndex = i;
            hit.distance = d;
            hit.p = { 1 - b1 - b2, b1, b2 };
        }
    }
    return isHitting;
}


bool
Primitive::intersect(const Ray& ray, Intersection& hit) const
{
  if (_mesh == nullptr)
    return false;

  auto t = const_cast<Primitive*>(this)->transform();
  Ray localRay{ray, t->worldToLocalMatrix()};
  auto d = math::inverse(localRay.direction.length());
  float tMin;
  float tMax;

  localRay.direction *= d;
  //if (_mesh->bounds().intersect(localRay, tMin, tMax))
  //{
    if (intersectMesh(ray, *_mesh, hit)) {
        hit.object = this;
        return true;
    }
    /*
    if (tMin >= ray.tMin && tMin <= ray.tMax)
    {
      hit.distance = tMin * d;
      return true;
    }
    if (tMax >= ray.tMin && tMax <= ray.tMax)
    {
      hit.distance = tMax * d;
      return true;
    }
    */
  //}
  return false;
}
} // end namespace cg