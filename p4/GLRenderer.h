//[]---------------------------------------------------------------[]
//|                                                                 |
//| Copyright (C) 2018, 2109 Orthrus Group.                         |
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
// OVERVIEW: GLRenderer.h
// ========
// Class definition for OpenGL renderer.
//
// Author(s): Paulo Pagliosa (and your name)
// Last revision: 21/09/2019

#ifndef __GLRenderer_h
#define __GLRenderer_h
#define MAX_LIGHTS 10


#include "Renderer.h"
#include "graphics/GLGraphics3.h"

namespace cg
{ // begin namespace cg


//////////////////////////////////////////////////////////
//
// GLRenderer: OpenGL renderer class
// ==========
class GLRenderer: public Renderer
{
public:
	GLRenderer(Scene& scene, Camera* camera = nullptr);

	void update() override;
	void render() override;
	void getLights();

private:
    void recursiveGetLights(SceneObject* obj);
    void drawObject(SceneObject* obj);

    GLSL::Program _program;

    struct LightPropLoc
    {
        GLint _typeLoc;
        GLint _colorLoc;
        GLfloat _positionLoc;
        GLfloat _directionLoc;
        GLfloat _falloffLoc;
        GLfloat _spotlightAngleRadiansLoc;
        GLfloat _radialFalloffLoc;
    };

    struct LightProps {
        int _type;
        Color _color;
        vec3f _position;
        vec3f _direction;
        float _falloff;
        float _spotLightAngleRadians;
        float _radialFalloff;
    };

    int _lightCount = 0;
    GLint _ambientLightLoc;
    GLint _NLLoc;
    LightPropLoc _lightLocs[MAX_LIGHTS];
    LightProps _lightProps[MAX_LIGHTS];

    //materials
    GLint _OaLoc;
    GLint _OdLoc;
    GLint _OsLoc;
    GLint _nsLoc;

    //primitive
    GLfloat _transformLoc;
    GLfloat _normalMatrixLoc;

    //camera
    GLfloat _vpMatrixLoc;
    GLfloat _cameraPositionLoc;
}; // GLRenderer

} // end namespace cg

#endif // __GLRenderer_h
