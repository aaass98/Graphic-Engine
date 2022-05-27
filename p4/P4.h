#ifndef __P4_h
#define __P4_h

#include "Assets.h"
#include "BVH.h"
#include "GLRenderer.h"
#include "Light.h"
#include "Primitive.h"
#include "SceneEditor.h"
#include "RayTracer.h"
#include "core/Flags.h"
#include "graphics/Application.h"
#include "graphics/GLImage.h"

using namespace cg;

class P4: public GLWindow
{
public:
  P4(int width, int height):
    GLWindow{"cg2021 - P4", width, height},
    _program{"P4"}
  {
    // do nothing
  }

  /// Initialize the app.
  void initialize() override;

  /// Update the GUI.
  void gui() override;

  /// Render the scene.
  void render() override;

  void openNodeHyerarchy(SceneNode* node);

  void deleteCurrentObject();

private:
  enum ViewMode
  {
    Editor = 0,
    GL_Renderer = 1,
    RT_Renderer = 2
  };

  enum class MoveBits
  {
    Left = 1,
    Right = 2,
    Forward = 4,
    Back = 8,
    Up = 16,
    Down = 32
  };

  enum class DragBits
  {
    Rotate = 1,
    Pan = 2
  };

  using BVHRef = Reference<BVH>;
  using BVHMap = std::map<TriangleMesh*, BVHRef>;

  GLSL::Program _program;
  Reference<Scene> _scene;
  Reference<SceneEditor> _editor;
  Reference<GLRenderer> _renderer;

  SceneNode* _current{};
  Color _selectedWireframeColor{255, 102, 0};
  Flags<MoveBits> _moveFlags{};
  Flags<DragBits> _dragFlags{};
  int _pivotX;
  int _pivotY;
  int _mouseX;
  int _mouseY;
  bool _showAssets{true};
  bool _showEditorView{true};
  ViewMode _viewMode{ViewMode::Editor};
  Reference<RayTracer> _rayTracer;
  Reference<GLImage> _image;
  BVHMap bvhMap;

  static MeshMap _defaultMeshes;

  void buildScene(int index);
  void renderScene();
  void recursiveRender(SceneObject*);

  void mainMenu();
  void fileMenu();
  void showOptions();

  void hierarchyWindow();
  void inspectorWindow();
  void assetsWindow();
  void editorView();
  void sceneGui();
  void sceneObjectGui();
  void objectGui();
  void editorViewGui();
  void inspectPrimitive(Primitive&);
  void inspectShape(Primitive&);
  void inspectMaterial(Material&);
  void inspectLight(Light&);
  void inspectCamera(Camera&);
  void addComponentButton(SceneObject&);

  void drawPrimitive(Primitive&);
  void drawLight(Light&);
  void drawCamera(Camera&);
  void drawViewport(Camera&);

  bool windowResizeEvent(int, int) override;
  bool keyInputEvent(int, int, int) override;
  bool scrollEvent(double, double) override;
  bool mouseButtonInputEvent(int, int, int) override;
  bool mouseMoveEvent(double, double) override;

  Ray makeRay(int, int) const;

  static void buildDefaultMeshes();

  //for uniforms;
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

  void getLights() {
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

  void recursiveGetLights(SceneObject* obj) {
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
}; // P4

#endif // __P4_h
