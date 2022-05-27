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
// OVERVIEW: Light.h
// ========
// Class definition for light.
//
// Author(s): Paulo Pagliosa (and your name)
// Last revision: 14/10/2019

#ifndef __Light_h
#define __Light_h

#include "Component.h"
#include "graphics/Color.h"

namespace cg
{ // begin namespace cg


/////////////////////////////////////////////////////////////////////
//
// Light: light class
// =====
    class Light : public Component
    {
    public:
        enum Type
        {
            Directional,
            Point,
            Spot
        };

        Color color{ Color::white };

        Light(SceneObject* sceneObject) :
            Component{ "Light", sceneObject },
            _type{ Directional }
        {
            // do nothing
        }

        Light() :
            Component{ "Light" },
            _type{ Directional }
        {
            // do nothing
        }

        auto type() const
        {
            return _type;
        }

        void setType(Type type)
        {
            _type = type;
        }

        void update() {

        }


        //Directional Light
        vec3f getWorldDirection() {
            _worldRotation = transform()->rotation() * quatf::eulerAngles(_localEulerAngles);
            vec3f worldDirection = _worldRotation * vec3f(0, 0, 1);
            return worldDirection.normalize();
        }

        void setLocalEulerAngles(vec3f angles) {
            _localEulerAngles = angles;
            _worldRotation = transform()->rotation() * quatf::eulerAngles(angles);
        }

        vec3f getLocalEulerAngles() {
            return _localEulerAngles;
        }

        //Point Light
        float getFalloff() {
            return _falloff;
        }

        void setFalloff(float falloff) {
            if (falloff < 0) {
                falloff = 0;
            }
            else if (falloff > 2) {
                falloff = 2;
            }
            _falloff = falloff;
        }

        //Spot Light
        float getSpotlightAngle() {
            return _spotlightAngle;
        }

        float getSpotlightAngleRadians() {
            return _spotlightAngleRadians;
        }

        void setSpotlightAngle(float angle) {
            _spotlightAngle = angle;
            _spotlightAngleRadians = angle * M_PI / 180;
        }

        float getRadialFalloff() {
            return _radialFalloff;
        }

        void setRadialFalloff(float f) {
            _radialFalloff = f;
        }

    private:
        Type _type;

        //Directional Light
        vec3f _localEulerAngles = vec3f(0, 0, 0); //also used for spot light
        quatf _worldRotation = quatf(0, 0, 0, 1);;


        //Point Light
        float _falloff = 1; //0~2 also used for Spot Light

        //Spot Light
        float _spotlightAngle = 30;
        float _spotlightAngleRadians = M_PI / 6;
        float _radialFalloff = 1;

    }; // Light

} // end namespace cg

#endif // __Light_h
