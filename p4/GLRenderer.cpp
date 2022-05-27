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
// OVERVIEW: GLRenderer.cpp
// ========
// Source file for OpenGL renderer.
//
// Author(s): Paulo Pagliosa (and your name)
// Last revision: 09/09/2019

#include "GLRenderer.h"
#include "Light.h"
#include "Primitive.h"
#include "graphics/Application.h"

namespace cg
{ // begin namespace cg


//////////////////////////////////////////////////////////
//
// GLRenderer implementation
// ==========
    GLRenderer::GLRenderer(Scene& scene, Camera* camera) :
        Renderer{ scene, camera },
        _program{ "Phong shader" }
    {

        Application::loadShaders(_program, "shaders/phong.vs", "shaders/phong.fs");
        _program.use();

        _ambientLightLoc = _program.uniformLocation("ambientLight");
        _NLLoc = _program.uniformLocation("NL");

        for (int i = 0; i < 10; i++)
        {
            char str[40];
            sprintf_s(str, "lights[%d].", i);
            char attribute[40];
            strcpy_s(attribute, str);
            strcat_s(attribute, "type");
            _lightLocs[i]._typeLoc = _program.uniformLocation(attribute);
            strcpy_s(attribute, str);
            strcat_s(attribute, "color");
            _lightLocs[i]._colorLoc = _program.uniformLocation(attribute);
            strcpy_s(attribute, str);
            strcat_s(attribute, "position");
            _lightLocs[i]._positionLoc = _program.uniformLocation(attribute);
            strcpy_s(attribute, str);
            strcat_s(attribute, "direction");
            _lightLocs[i]._directionLoc = _program.uniformLocation(attribute);
            strcpy_s(attribute, str);
            strcat_s(attribute, "falloff");
            _lightLocs[i]._falloffLoc = _program.uniformLocation(attribute);
            strcpy_s(attribute, str);
            strcat_s(attribute, "spotlightAngleRadians");
            _lightLocs[i]._spotlightAngleRadiansLoc = _program.uniformLocation(attribute);
        }

        _OaLoc = _program.uniformLocation("material.Oa");
        _OdLoc = _program.uniformLocation("material.Od");
        _OsLoc = _program.uniformLocation("material.Os");
        _nsLoc = _program.uniformLocation("material.s");

        _transformLoc = _program.uniformLocation("transform");
        _normalMatrixLoc = _program.uniformLocation("normalMatrix");

        _vpMatrixLoc = _program.uniformLocation("vpMatrix");
        _cameraPositionLoc = _program.uniformLocation("cameraPosition");
    }

    void
        GLRenderer::update()
    {
        Renderer::update();
        // TODO
    }

    void
        GLRenderer::render()
    {
        _program.use();
        const auto& bc = _scene->backgroundColor;

        glClearColor(bc.r, bc.g, bc.b, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        getLights();
        //set light and camera uniforms
        _program.setUniformMat4(_vpMatrixLoc, vpMatrix(_camera));
        _program.setUniformVec4(_ambientLightLoc, _scene->ambientLight);
        _program.setUniformVec3(_cameraPositionLoc, _camera->transform()->position());

        _program.setUniform(_NLLoc, _lightCount);
        for (int i = 0; i < _lightCount; i++) {
            _program.setUniform(_lightLocs[i]._typeLoc, _lightProps[i]._type);
            _program.setUniformVec4(_lightLocs[i]._colorLoc, _lightProps[i]._color);
            _program.setUniformVec3(_lightLocs[i]._positionLoc, _lightProps[i]._position);
            _program.setUniformVec3(_lightLocs[i]._directionLoc, _lightProps[i]._direction);
            _program.setUniform(_lightLocs[i]._falloffLoc, _lightProps[i]._falloff);
            _program.setUniform(_lightLocs[i]._spotlightAngleRadiansLoc, _lightProps[i]._spotLightAngleRadians);
            _program.setUniform(_lightLocs[i]._radialFalloffLoc, _lightProps[i]._radialFalloff);
        }

        //render all objects
        SceneObjectListIterator* objectIterator = _scene->objectIterator();
        //first object is root, so we can skip it
        objectIterator->start();
        SceneObject* obj = objectIterator->next();
        //call a recursive render function for each object and render it
        while (obj) {
            drawObject(obj);

            obj = objectIterator->next();
        }
        objectIterator->dispose();
    }

    void GLRenderer::drawObject(SceneObject* obj) { //draw objects and its children
        Primitive* primitive = dynamic_cast<Primitive*>(obj->getComponent("Primitive"));
        if (primitive) {
            auto m = glMesh(primitive->mesh());

            if (nullptr == m)
                return;

            auto t = primitive->transform();
            auto normalMatrix = mat3f{ t->worldToLocalMatrix() }.transposed();

            _program.setUniformMat4(_transformLoc, t->localToWorldMatrix());
            _program.setUniformMat3(_normalMatrixLoc, normalMatrix);
            //set material uniforms
            _program.setUniformVec4(_OaLoc, primitive->material.ambient);
            _program.setUniformVec4(_OdLoc, primitive->material.diffuse);
            _program.setUniformVec4(_OsLoc, primitive->material.spot);
            _program.setUniform(_nsLoc, primitive->material.shine);

            m->bind();
            //draws mesh
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            glDrawElements(GL_TRIANGLES, m->vertexCount(), GL_UNSIGNED_INT, 0);
        }

        //draws children
        SceneObjectListIterator* it = obj->objectIterator();
        SceneObject* newObj = it->start();
        while (newObj) {
            drawObject(newObj);
            newObj = it->next();
        }
        it->dispose();
    }

    void GLRenderer::getLights() {
        _lightCount = 0;
        SceneObjectListIterator* it = _scene->objectIterator();
        it->start(); //root is skipped
        SceneObject* obj = it->next();
        while (obj && _lightCount < MAX_LIGHTS) {
            recursiveGetLights(obj);
            obj = it->next();
        }
        it->dispose();
    }

    void GLRenderer::recursiveGetLights(SceneObject* obj) {
        Light* light = dynamic_cast<Light*>(obj->getComponent("Light"));
        if (light) {
            _lightProps[_lightCount]._type = light->type();
            _lightProps[_lightCount]._color = light->color;
            _lightProps[_lightCount]._position = light->transform()->position();
            _lightProps[_lightCount]._direction = light->getWorldDirection();
            _lightProps[_lightCount]._falloff = light->getFalloff();
            _lightProps[_lightCount]._spotLightAngleRadians = light->getSpotlightAngleRadians();
            _lightProps[_lightCount]._radialFalloff = light->getRadialFalloff();
            _lightCount++;
        }
        SceneObjectListIterator* it = obj->objectIterator();
        SceneObject* newObj = it->start();
        while (newObj && _lightCount < MAX_LIGHTS) {
            recursiveGetLights(newObj);
            newObj = it->next();
        }
        it->dispose();
    }

} // end namespace cg
