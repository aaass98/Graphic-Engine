#include "geometry/MeshSweeper.h"
#include "P4.h"

MeshMap P4::_defaultMeshes;

inline auto
normalize(const vec4f& p)
{
  return vec3f{p} * math::inverse(p.w);
}

inline auto
viewportToNDC(int x, int y)
{
  GLint v[4];

  glGetIntegerv(GL_VIEWPORT, v);

  const auto xn = float(x - v[0]) * 2.0f / float(v[2]) - 1.0f;
  const auto yn = float(y - v[1]) * 2.0f / float(v[3]) - 1.0f;

  return vec4f{xn, yn, -1, 1};
}

inline Ray
P4::makeRay(int x, int y) const
{
  auto c = _editor->camera();
  mat4f m{vpMatrix(c)};

  m.invert();

  auto p = normalize(m * viewportToNDC(x, height() - y));
  auto t = c->transform();
  Ray r{t->position(), -t->forward()};

  if (c->projectionType() == Camera::Perspective)
    r.direction = (p - r.origin).versor();
  else
    r.origin = p - r.direction * c->nearPlane();
  return r;
}

inline void
P4::buildDefaultMeshes()
{
  _defaultMeshes["None"] = nullptr;
  _defaultMeshes["Box"] = GLGraphics3::box();
  _defaultMeshes["Sphere"] = GLGraphics3::sphere();
}

inline Primitive*
makePrimitive(MeshMapIterator mit)
{
  return new Primitive(mit->second, mit->first);
}

inline void
P4::buildScene(int index)
{

    if (index == 1) {
        _current = _scene = new Scene{ "Room" };
        _editor = new SceneEditor{ *_scene };
        _editor->setDefaultView((float)width() / (float)height());

        //makes room
        SceneObject* room = new SceneObject("Room", _scene);

        SceneObject* floor = new SceneObject("Floor", room);
        floor->addComponent(makePrimitive(_defaultMeshes.find("Box")));
        Primitive* prim = dynamic_cast<Primitive*>(floor->getComponent("Primitive"));
        prim->material.diffuse = Color::darkGray;
        prim->material.spot = Color::white;
        floor->transform()->setLocalPosition(vec3f(0, -1.3, 0));
        floor->transform()->setLocalScale(vec3f(4, 0.2, 4));

        SceneObject* wall1 = new SceneObject("Wall 1", room);
        wall1->addComponent(makePrimitive(_defaultMeshes.find("Box")));
        prim = dynamic_cast<Primitive*>(wall1->getComponent("Primitive"));
        prim->material.diffuse = Color::white;
        prim->material.spot = Color::blue;
        wall1->transform()->setLocalPosition(vec3f(-3.9, 2.6, 0));
        wall1->transform()->setLocalScale(vec3f(0.2, 4, 4));

        SceneObject* wall2 = new SceneObject("Wall 2", room);
        wall2->addComponent(makePrimitive(_defaultMeshes.find("Box")));
        prim = dynamic_cast<Primitive*>(wall2->getComponent("Primitive"));
        prim->material.diffuse = Color::white;
        prim->material.spot = Color::red;
        prim->material.shine = 2;
        wall2->transform()->setLocalPosition(vec3f(0, 2.6, -4));
        wall2->transform()->setLocalScale(vec3f(4, 4, 0.2));


        //place ball, camera and lights
        SceneObject* rootObject1 = new SceneObject("Object 1", _scene);
        rootObject1->addComponent(makePrimitive(_defaultMeshes.find("Sphere")));
        prim = dynamic_cast<Primitive*>(rootObject1->getComponent("Primitive"));
        prim->material.diffuse = Color::red;
        prim->material.spot = Color::white;

        SceneObject* mainCamera = new SceneObject("Main Camera", _scene);
        mainCamera->addComponent(new Camera);
        mainCamera->transform()->setLocalPosition(vec3f(0, 2, 4));
        mainCamera->transform()->setLocalEulerAngles(vec3f(-27, 0, 0));

        SceneObject* pointLight = new SceneObject("Point Light", _scene);
        Light* light = new Light();
        light->setType(Light::Point);
        light->setFalloff(0.5);
        pointLight->addComponent(light);
        pointLight->transform()->setLocalPosition(vec3f(1, 2.7, 4.4));
        pointLight->transform()->setLocalEulerAngles(vec3f(-150, 0, 0));
        _current = mainCamera;
    }
    else if (index == 2) {
        _current = _scene = new Scene{ "Balls" };
        _editor = new SceneEditor{ *_scene };
        _editor->setDefaultView((float)width() / (float)height());

        SceneObject* ball1 = new SceneObject("Ball 1", _scene);
        ball1->transform()->setLocalPosition(vec3f(-1, 0, -1));
        ball1->addComponent(makePrimitive(_defaultMeshes.find("Sphere")));
        Primitive* prim = dynamic_cast<Primitive*>(ball1->getComponent("Primitive"));
        prim->material.diffuse = Color::red;
        prim->material.spot = Color::white;

        SceneObject* ball2 = new SceneObject("Ball 2", _scene);
        ball2->transform()->setLocalPosition(vec3f(2, 0, 0));
        ball2->addComponent(makePrimitive(_defaultMeshes.find("Sphere")));
        prim = dynamic_cast<Primitive*>(ball2->getComponent("Primitive"));
        prim->material.diffuse = Color::green;
        prim->material.spot = Color::white;

        SceneObject* ball3 = new SceneObject("Ball 3", _scene);
        ball3->transform()->setLocalPosition(vec3f(0, 0, 2));
        ball3->addComponent(makePrimitive(_defaultMeshes.find("Sphere")));
        prim = dynamic_cast<Primitive*>(ball3->getComponent("Primitive"));
        prim->material.diffuse = Color::blue;
        prim->material.spot = Color::white;

        SceneObject* mainCamera = new SceneObject("Main Camera", _scene);
        mainCamera->addComponent(new Camera);
        mainCamera->transform()->setLocalPosition(vec3f(0, 2, 4));
        mainCamera->transform()->setLocalEulerAngles(vec3f(-27, 0, 0));

        SceneObject* pointLight = new SceneObject("Point Light", _scene);
        Light* light = new Light();
        light->setType(Light::Point);
        pointLight->addComponent(light);
        pointLight->transform()->setLocalPosition(vec3f(0, 2.7, 4.4));
        pointLight->transform()->setLocalEulerAngles(vec3f(-150, 0, 0));
        _current = mainCamera;
    }
    else if (index == 3) {
        _current = _scene = new Scene{ "Scene 3" };
        _editor = new SceneEditor{ *_scene };
        _editor->setDefaultView((float)width() / (float)height());

        SceneObject* rootObject1 = new SceneObject("Object 1", _scene);
        rootObject1->addComponent(makePrimitive(_defaultMeshes.find("Sphere")));
        Primitive* prim = dynamic_cast<Primitive*>(rootObject1->getComponent("Primitive"));
        prim->material.diffuse = Color::red;
        prim->material.spot = Color::white;

        SceneObject* mainCamera = new SceneObject("Main Camera", _scene);
        mainCamera->addComponent(new Camera);
        mainCamera->transform()->setLocalPosition(vec3f(0, 2, 4));
        mainCamera->transform()->setLocalEulerAngles(vec3f(-27, 0, 0));

        SceneObject* pointLight = new SceneObject("Point Light", _scene);
        Light* light = new Light();
        light->setType(Light::Point);
        pointLight->addComponent(light);
        pointLight->transform()->setLocalPosition(vec3f(0, 2.7, 4.4));
        pointLight->transform()->setLocalEulerAngles(vec3f(-150, 0, 0));
        _current = mainCamera;
    }
}

void
P4::initialize()
{
  Application::loadShaders(_program, "shaders/gouraud.vs", "shaders/P4.fs");
  Assets::initialize();
  buildDefaultMeshes();
  buildScene(1);
  _renderer = new GLRenderer{*_scene};
  _rayTracer = new RayTracer{*_scene};
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_POLYGON_OFFSET_FILL);
  glPolygonOffset(1.0f, 1.0f);
  glEnable(GL_LINE_SMOOTH);
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

inline void
P4::hierarchyWindow()
{
  ImGui::Begin("Hierarchy");
  if (ImGui::Button("Create###object"))
    ImGui::OpenPopup("CreateObjectPopup");
  if (ImGui::BeginPopup("CreateObjectPopup"))
  {
    int count = _current->getChildCount() + 1;
    if (ImGui::MenuItem("Empty Object"))
    {
        char name[30];
        sprintf_s(name, "Object %d", count);
        SceneObject* newObject = new SceneObject(name, _scene);
        newObject->setParent(dynamic_cast<SceneObject*>(_current));
    }
    if (ImGui::BeginMenu("3D Object"))
    {
      if (ImGui::MenuItem("Box"))
      {
          char name[30];
          sprintf_s(name, "Box %d", count);
          SceneObject* newObject = new SceneObject(name, _scene);
          newObject->setParent(dynamic_cast<SceneObject*>(_current));
          newObject->addComponent(makePrimitive(_defaultMeshes.find("Box")));
      }
      if (ImGui::MenuItem("Sphere"))
      {
          char name[30];
          sprintf_s(name, "Sphere %d", count);
          SceneObject* newObject = new SceneObject(name, _scene);
          newObject->setParent(dynamic_cast<SceneObject*>(_current));
          newObject->addComponent(makePrimitive(_defaultMeshes.find("Sphere")));
      }
      ImGui::EndMenu();
    }
    if (ImGui::BeginMenu("Light"))
    {
        if (ImGui::MenuItem("Directional Light"))
        {
            char name[30];
            sprintf_s(name, "Light %d", count);
            SceneObject* newObject = new SceneObject(name, _scene);
            newObject->setParent(dynamic_cast<SceneObject*>(_current));
            new Light(newObject);

        }
        if (ImGui::MenuItem("Point Light"))
        {
            char name[30];
            sprintf_s(name, "Light %d", count);
            SceneObject* newObject = new SceneObject(name, _scene);
            newObject->setParent(dynamic_cast<SceneObject*>(_current));
            Light* light = new Light(newObject);
            light->setType(Light::Point);
        }
        if (ImGui::MenuItem("Spotlight"))
        {
            char name[30];
            sprintf_s(name, "Light %d", count);
            SceneObject* newObject = new SceneObject(name, _scene);
            newObject->setParent(dynamic_cast<SceneObject*>(_current));
            Light* light = new Light(newObject);
            light->setType(Light::Spot);
        }
      ImGui::EndMenu();
    }
    if (ImGui::MenuItem("Camera"))
    {
        char name[30];
        sprintf_s(name, "Camera %d", count);
        SceneObject* newObject = new SceneObject(name, _scene);
        newObject->setParent(dynamic_cast<SceneObject*>(_current));
        new Camera(newObject);
    }
    ImGui::EndPopup();
  }
  ImGui::Separator();

  auto f = ImGuiTreeNodeFlags_OpenOnArrow;
  auto open = ImGui::TreeNodeEx(_scene,
    _current == _scene ? f | ImGuiTreeNodeFlags_Selected : f,
    _scene->name());

  if (ImGui::IsItemClicked())
    _current = _scene;
  if (ImGui::BeginDragDropTarget()) {
      if (auto payload = ImGui::AcceptDragDropPayload("SceneObject")) {
          SceneObject** payloadObj = (SceneObject**)payload->Data;
          if (payloadObj)
              (*payloadObj)->setParent(nullptr);
      }
      ImGui::EndDragDropTarget();
  }

  if (open)
  {
      openNodeHyerarchy(_scene);
  }
  ImGui::End();
}

void P4::openNodeHyerarchy(SceneNode* node) {
    if (!node) {
        return;
    }
    SceneObjectListIterator* it = node->objectIterator();
    SceneObject* object = it->start();

    if (object == _scene->root())
        object = it->next();

    int count = 0;
    while (object) {
        ImGuiTreeNodeFlags flag{ object->getChildCount() != 0 ? ImGuiTreeNodeFlags_OpenOnArrow : ImGuiTreeNodeFlags_Leaf };
        ImGui::PushID(object);
        auto open = ImGui::TreeNodeEx(object, _current == object ? flag | ImGuiTreeNodeFlags_Selected : flag, object->name());
        count++;
        if (ImGui::IsItemClicked())
            _current = object;

        //begin parent change
        if (ImGui::BeginDragDropSource()) {

            ImGui::SetDragDropPayload("SceneObject", &object, sizeof(SceneObject*));
            ImGui::EndDragDropSource();
        }

        if (ImGui::BeginDragDropTarget()) {
            if (auto payload = ImGui::AcceptDragDropPayload("SceneObject")) {
                SceneObject** payloadObj = (SceneObject**)payload->Data;
                if (payloadObj)
                    (*payloadObj)->setParent(object);
            }
            ImGui::EndDragDropTarget();
        }

        if (open) {
            openNodeHyerarchy(object);
        }
        //ImGui::TreePop();
        object = it->next();
    }

    while (count > 0) {
        ImGui::TreePop();
        count--;
    }
    ImGui::TreePop();
}

namespace ImGui
{ // begin namespace ImGui

void
ObjectNameInput(NameableObject* object)
{
  const int bufferSize{128};
  static NameableObject* current;
  static char buffer[bufferSize];

  if (object != current)
  {
    strcpy_s(buffer, bufferSize, object->name());
    current = object;
  }
  if (ImGui::InputText("Name", buffer, bufferSize))
    object->setName(buffer);
}

inline bool
ColorEdit3(const char* label, Color& color)
{
  return ImGui::ColorEdit3(label, (float*)&color);
}

inline bool
DragVec3(const char* label, vec3f& v)
{
  return DragFloat3(label, (float*)&v, 0.1f, 0.0f, 0.0f, "%.2g");
}

void
TransformEdit(Transform* transform)
{
  vec3f temp;

  temp = transform->localPosition();
  if (ImGui::DragVec3("Position", temp))
    transform->setLocalPosition(temp);
  temp = transform->localEulerAngles();
  if (ImGui::DragVec3("Rotation", temp))
    transform->setLocalEulerAngles(temp);
  temp = transform->localScale();
  if (ImGui::DragVec3("Scale", temp))
    transform->setLocalScale(temp);
}

} // end namespace ImGui

inline void
P4::sceneGui()
{
  auto scene = (Scene*)_current;

  ImGui::ObjectNameInput(_current);
  ImGui::Separator();
  if (ImGui::CollapsingHeader("Colors"))
  {
    ImGui::ColorEdit3("Background", scene->backgroundColor);
    ImGui::ColorEdit3("Ambient Light", scene->ambientLight);
  }
}

inline void
P4::inspectShape(Primitive& primitive)
{
  char buffer[16];

  snprintf(buffer, 16, "%s", primitive.meshName());
  ImGui::InputText("Mesh", buffer, 16, ImGuiInputTextFlags_ReadOnly);
  if (ImGui::BeginDragDropTarget())
  {
    if (auto* payload = ImGui::AcceptDragDropPayload("PrimitiveMesh"))
    {
      auto mit = *(MeshMapIterator*)payload->Data;
      primitive.setMesh(mit->second, mit->first);
    }
    ImGui::EndDragDropTarget();
  }
  ImGui::SameLine();
  if (ImGui::Button("...###PrimitiveMesh"))
    ImGui::OpenPopup("PrimitiveMeshPopup");
  if (ImGui::BeginPopup("PrimitiveMeshPopup"))
  {
    auto& meshes = Assets::meshes();

    if (!meshes.empty())
    {
      for (auto mit = meshes.begin(); mit != meshes.end(); ++mit)
        if (ImGui::Selectable(mit->first.c_str()))
          primitive.setMesh(Assets::loadMesh(mit), mit->first);
      ImGui::Separator();
    }
    for (auto mit = _defaultMeshes.begin(); mit != _defaultMeshes.end(); ++mit)
      if (ImGui::Selectable(mit->first.c_str()))
        primitive.setMesh(mit->second, mit->first);
    ImGui::EndPopup();
  }
}

inline void
P4::inspectMaterial(Material& material)
{
  ImGui::ColorEdit3("Ambient", material.ambient);
  ImGui::ColorEdit3("Diffuse", material.diffuse);
  ImGui::ColorEdit3("Spot", material.spot);
  ImGui::DragFloat("Shine", &material.shine, 1, 0, 1000.0f);
  ImGui::ColorEdit3("Specular", material.specular);
}

inline void
P4::inspectPrimitive(Primitive& primitive)
{
  const auto flag = ImGuiTreeNodeFlags_NoTreePushOnOpen;

  if (ImGui::TreeNodeEx("Shape", flag))
    inspectShape(primitive);
  if (ImGui::TreeNodeEx("Material", flag))
    inspectMaterial(primitive.material);
}

inline void
P4::inspectLight(Light& light)
{
  static const char* lightTypes[]{"Directional", "Point", "Spot"};
  auto lt = light.type();

  if (ImGui::BeginCombo("Type", lightTypes[lt]))
  {
    for (auto i = 0; i < IM_ARRAYSIZE(lightTypes); ++i)
    {
      bool selected = lt == i;

      if (ImGui::Selectable(lightTypes[i], selected))
        lt = (Light::Type)i;
      if (selected)
        ImGui::SetItemDefaultFocus();
    }
    ImGui::EndCombo();
  }
  light.setType(lt);
  ImGui::ColorEdit3("Color", light.color);

  if (lt == 0) {
      //directional light
      vec3f dir = light.getLocalEulerAngles();
      if (ImGui::DragVec3("Light Rotation", dir))
          light.setLocalEulerAngles(dir);
  }
  else if (lt == 1) {
      //point light
      float falloff = light.getFalloff();
      if (ImGui::DragFloat("Falloff Factor", &falloff, 0.0025f, 0.0f, 2.0f, "%.2f", 1.0f))
      {
          light.setFalloff(falloff);
      }
  }
  else if (lt == 2) {
      //spot light
      vec3f dir = light.getLocalEulerAngles();
      if (ImGui::DragVec3("Light Rotation", dir))
          light.setLocalEulerAngles(dir);
      float falloff = light.getFalloff();
      if (ImGui::DragFloat("Falloff Factor", &falloff, 0.0025f, 0.0f, 2.0f, "%.2f", 1.0f))
      {
          light.setFalloff(falloff);
      }
      float radialFalloff = light.getRadialFalloff();
      if (ImGui::DragFloat("Radial Falloff Factor", &radialFalloff, 0.0025f, 0.0f, 2.0f, "%.2f", 1.0f))
      {
          light.setRadialFalloff(radialFalloff);
      }
      float angle = light.getSpotlightAngle();
      if (ImGui::DragFloat("Spotlight Angle", &angle, 0.1f, 0.0f, 90.0f, "%.1f", 1.0f))
          light.setSpotlightAngle(angle);
  }
}

void
P4::inspectCamera(Camera& camera)
{
  static const char* projectionNames[]{"Perspective", "Orthographic"};
  auto cp = camera.projectionType();

  if (ImGui::BeginCombo("Projection", projectionNames[cp]))
  {
    for (auto i = 0; i < IM_ARRAYSIZE(projectionNames); ++i)
    {
      auto selected = cp == i;

      if (ImGui::Selectable(projectionNames[i], selected))
        cp = (Camera::ProjectionType)i;
      if (selected)
        ImGui::SetItemDefaultFocus();
    }
    ImGui::EndCombo();
  }
  camera.setProjectionType(cp);
  if (cp == View3::Perspective)
  {
    auto fov = camera.viewAngle();

    if (ImGui::SliderFloat("View Angle",
      &fov,
      MIN_ANGLE,
      MAX_ANGLE,
      "%.0f deg",
      1.0f))
      camera.setViewAngle(fov <= MIN_ANGLE ? MIN_ANGLE : fov);
  }
  else
  {
    auto h = camera.height();

    if (ImGui::DragFloat("Height",
      &h,
      MIN_HEIGHT * 10.0f,
      MIN_HEIGHT,
      math::Limits<float>::inf()))
      camera.setHeight(h <= 0 ? MIN_HEIGHT : h);
  }

  float n;
  float f;

  camera.clippingPlanes(n, f);

  if (ImGui::DragFloatRange2("Clipping Planes",
    &n,
    &f,
    MIN_DEPTH,
    MIN_DEPTH,
    math::Limits<float>::inf(),
    "Near: %.2f",
    "Far: %.2f"))
  {
    if (n <= 0)
      n = MIN_DEPTH;
    if (f - n < MIN_DEPTH)
      f = n + MIN_DEPTH;
    camera.setClippingPlanes(n, f);
  }
}

inline void
P4::addComponentButton(SceneObject& object)
{
  if (ImGui::Button("Add Component"))
    ImGui::OpenPopup("AddComponentPopup");
  if (ImGui::BeginPopup("AddComponentPopup"))
  {
    if (ImGui::MenuItem("Primitive"))
    {
        Primitive* primitive = makePrimitive(_defaultMeshes.find("Box"));
        if (!object.addComponent(primitive)) {
            delete primitive;
        }
    }
    if (ImGui::MenuItem("Light"))
    {
        Light* light = new Light();
        if (!object.addComponent(light)) {
            delete light;
        }
    }
    if (ImGui::MenuItem("Camera"))
    {
        Camera* camera = new Camera();
        if (!object.addComponent(camera)) {
            delete camera;
        }
    }
    ImGui::EndPopup();
  }
}

inline void
P4::sceneObjectGui()
{
  auto object = (SceneObject*)_current;

  addComponentButton(*object);
  ImGui::Separator();
  ImGui::ObjectNameInput(object);
  ImGui::SameLine();
  ImGui::Checkbox("###visible", &object->visible);
  ImGui::Separator();
  if (ImGui::CollapsingHeader(object->transform()->typeName()))
    ImGui::TransformEdit(object->transform());
  ComponentListIterator* componentIterator = object->componentIterator();
  auto component = componentIterator->start();

  while (component) {

      if (auto p = dynamic_cast<Primitive*>(component))
      {
          auto notDelete{ true };
          auto open = ImGui::CollapsingHeader(p->typeName(), &notDelete);

          if (!notDelete)
          {
              Component* tempComponent = componentIterator->next();
              object->removeComponent(component);
              component = tempComponent;
              continue;
          }
          else if (open)
              inspectPrimitive(*p);
      }
      else if (auto l = dynamic_cast<Light*>(component))
      {
          auto notDelete{ true };
          auto open = ImGui::CollapsingHeader(l->typeName(), &notDelete);

          if (!notDelete)
          {
              Component* tempComponent = componentIterator->next();
              object->removeComponent(component);
              component = tempComponent;
              continue;
          }
          else if (open)
              inspectLight(*l);
      }
      else if (auto c = dynamic_cast<Camera*>(component))
      {
          auto notDelete{ true };
          auto open = ImGui::CollapsingHeader(c->typeName(), &notDelete);

          if (!notDelete)
          {
              Component* tempComponent = componentIterator->next();
              object->removeComponent(component);
              component = tempComponent;
              continue;
          }
          else if (open)
          {
              auto isCurrent = c == Camera::current();

              ImGui::Checkbox("Current", &isCurrent);
              Camera::setCurrent(isCurrent ? c : nullptr);
              inspectCamera(*c);
          }
      }
      component = componentIterator->next();
  }
  componentIterator->dispose();
}

inline void
P4::objectGui()
{
  if (_current == nullptr)
    return;
  if (dynamic_cast<SceneObject*>(_current))
  {
    sceneObjectGui();
    return;
  }
  if (dynamic_cast<Scene*>(_current))
    sceneGui();
}

inline void
P4::inspectorWindow()
{
  ImGui::Begin("Inspector");
  objectGui();
  ImGui::End();
}

inline void
P4::editorViewGui()
{
  if (ImGui::Button("Set Default View"))
    _editor->setDefaultView(float(width()) / float(height()));
  ImGui::Separator();

  auto t = _editor->camera()->transform();
  vec3f temp;

  temp = t->localPosition();
  if (ImGui::DragVec3("Position", temp))
    t->setLocalPosition(temp);
  temp = t->localEulerAngles();
  if (ImGui::DragVec3("Rotation", temp))
    t->setLocalEulerAngles(temp);
  inspectCamera(*_editor->camera());
  ImGui::Separator();
  {
    static int sm;

    ImGui::Combo("Shading Mode", &sm, "None\0Flat\0Gouraud\0\0");
    // TODO

    static Color edgeColor;
    static bool showEdges;

    ImGui::ColorEdit3("Edges", edgeColor);
    ImGui::SameLine();
    ImGui::Checkbox("###showEdges", &showEdges);
  }
  ImGui::Separator();
  ImGui::Checkbox("Show Ground", &_editor->showGround);
}

inline void
P4::assetsWindow()
{
  ImGui::Begin("Assets");
  if (ImGui::CollapsingHeader("Meshes"))
  {
    auto& meshes = Assets::meshes();

    for (auto mit = meshes.begin(); mit != meshes.end(); ++mit)
    {
      auto meshName = mit->first.c_str();
      auto selected = false;

      ImGui::Selectable(meshName, &selected);
      if (ImGui::BeginDragDropSource())
      {
        Assets::loadMesh(mit);
        ImGui::Text(meshName);
        ImGui::SetDragDropPayload("PrimitiveMesh", &mit, sizeof(mit));
        ImGui::EndDragDropSource();
      }
    }
  }
  ImGui::Separator();
  if (ImGui::CollapsingHeader("Textures"))
  {
    // next semester
  }
  ImGui::End();
}

inline void
P4::editorView()
{
  if (!_showEditorView)
    return;
  ImGui::Begin("Editor View Settings");
  editorViewGui();
  ImGui::End();
}

inline void
P4::fileMenu()
{
  if (ImGui::MenuItem("New"))
  {
    // TODO
  }
  if (ImGui::MenuItem("Open...", "Ctrl+O"))
  {
    // TODO
  }
  ImGui::Separator();
  if (ImGui::MenuItem("Save", "Ctrl+S"))
  { 
    // TODO
  }
  if (ImGui::MenuItem("Save As..."))
  {
    // TODO
  }
  ImGui::Separator();
  if (ImGui::MenuItem("Exit", "Alt+F4"))
  {
    shutdown();
  }
}

inline bool
showStyleSelector(const char* label)
{
  static int style = 1;

  if (!ImGui::Combo(label, &style, "Classic\0Dark\0Light\0"))
    return false;
  switch (style)
  {
    case 0: ImGui::StyleColorsClassic();
      break;
    case 1: ImGui::StyleColorsDark();
      break;
    case 2: ImGui::StyleColorsLight();
      break;
  }
  return true;
}

inline void
P4::showOptions()
{
  ImGui::PushItemWidth(ImGui::GetWindowWidth() * 0.6f);
  showStyleSelector("Color Theme##Selector");
  ImGui::ColorEdit3("Selected Wireframe", _selectedWireframeColor);
  ImGui::PopItemWidth();
}

inline void
P4::mainMenu()
{
  if (ImGui::BeginMainMenuBar())
  {
    if (ImGui::BeginMenu("File"))
    {
      fileMenu();
      ImGui::EndMenu();
    }
    if (ImGui::BeginMenu("View"))
    {
      if (Camera::current() == 0)
        ImGui::MenuItem("Edit View", nullptr, true, false);
      else
      {
        static const char* viewLabels[]{"Editor", "OpenGL", "Ray Tracer"};

        if (ImGui::BeginCombo("View", viewLabels[_viewMode]))
        {
          for (auto i = 0; i < IM_ARRAYSIZE(viewLabels); ++i)
          {
            if (ImGui::Selectable(viewLabels[i], _viewMode == i))
              _viewMode = (ViewMode)i;
          }
          ImGui::EndCombo();
          // TODO: change mode only if scene has changed
          if (_viewMode == ViewMode::Editor)
            _image = nullptr;
        }
      }
      ImGui::Separator();
      ImGui::MenuItem("Assets Window", nullptr, &_showAssets);
      ImGui::MenuItem("Editor View Settings", nullptr, &_showEditorView);
      ImGui::EndMenu();
    }
    if (ImGui::BeginMenu("Tools"))
    {
      if (ImGui::BeginMenu("Options"))
      {
        showOptions();
        ImGui::EndMenu();
      }
      ImGui::EndMenu();
    }
    if (ImGui::BeginMenu("Examples")) {
        if (ImGui::BeginMenu("Load"))
        {
            if (ImGui::MenuItem("Room"))
            {
                buildScene(1);
            }
            if (ImGui::MenuItem("Balls"))
            {
                buildScene(2);
            }
            ImGui::Separator();
            if (ImGui::MenuItem("Scene 3"))
            {
                buildScene(3);
            }
            ImGui::EndMenu();
        }
        ImGui::EndMenu();
    }
    ImGui::EndMainMenuBar();

  }
}

void
P4::gui()
{
  mainMenu();
  if (_viewMode == ViewMode::RT_Renderer)
    return;
  hierarchyWindow();
  inspectorWindow();
  assetsWindow();
  editorView();
}

inline void
drawMesh(GLMesh* mesh, GLuint mode)
{
  glPolygonMode(GL_FRONT_AND_BACK, mode);
  glDrawElements(GL_TRIANGLES, mesh->vertexCount(), GL_UNSIGNED_INT, 0);
}

inline void
P4::drawPrimitive(Primitive& primitive)
{
  auto mesh = primitive.mesh();
  auto m = glMesh(mesh);

  if (nullptr == m)
    return;

  auto t = primitive.transform();
  auto normalMatrix = mat3f{t->worldToLocalMatrix()}.transposed();


  _program.setUniformMat4(_transformLoc, t->localToWorldMatrix());
  _program.setUniformMat3(_normalMatrixLoc, normalMatrix);
  _program.setUniformVec4(_OaLoc, primitive.material.ambient);
  _program.setUniformVec4(_OdLoc, primitive.material.diffuse);
  _program.setUniformVec4(_OsLoc, primitive.material.spot);
  _program.setUniform(_nsLoc, primitive.material.shine);

  m->bind();
  drawMesh(m, GL_FILL);

  if (primitive.sceneObject() != _current)
      return;
  _program.setUniformVec4("wireframeColor", _selectedWireframeColor);
  _program.setUniform("drawWireframe", true);

  drawMesh(m, GL_LINE);

  _program.setUniform("drawWireframe", false);
  


  //for drawing bvh on scene editor
  /*
  BVH* bvh = _rayTracer->getBVH(mesh);
  if (bvh) {
      bvh->iterate([this, t](const BVHNodeInfo& node)
          {
              _editor->setLineColor(node.isLeaf ? Color::yellow : Color::magenta);
              _editor->drawBounds(node.bounds, t->localToWorldMatrix());
          });
  }
  */
}

inline void
P4::drawLight(Light& light)
{
    auto lt = light.type();
    _editor->setLineColor(light.color);
    if (lt == 0) {
        //Directional Light
        _editor->drawVector(light.transform()->position(), light.getWorldDirection().normalize(), 3);
    }
    else if (lt == 1) {
        //Point Light
        //float radius = 5 - (light.getFalloff() * light.getFalloff()); //more beautiful
        float radius = 10 / pow(3, light.getFalloff()); //more accurate
        _editor->drawCircle(light.transform()->position(), radius, vec3f(1, 0, 0));
        _editor->drawCircle(light.transform()->position(), radius, vec3f(0, 1, 0));
        _editor->drawCircle(light.transform()->position(), radius, vec3f(0, 0, 1));
    }
    else if (lt == 2) {
        //Spot Light
        float coneHeight = 10 / pow(3, light.getFalloff());
        _editor->drawVector(light.transform()->position(), light.getWorldDirection().normalize(), coneHeight);
        vec3f coneCenter = light.transform()->position() + light.getWorldDirection() * coneHeight;
        float radius = (coneCenter - light.transform()->position()).length() * tan(light.getSpotlightAngleRadians());
        _editor->drawLine(light.transform()->position(), coneCenter);
        _editor->drawCircle(coneCenter, radius, light.getWorldDirection());
        _editor->drawLine(light.transform()->position(), coneCenter + light.getWorldDirection().up() * radius);
        _editor->drawLine(light.transform()->position(), coneCenter - light.getWorldDirection().up() * radius);
    }
}

inline void
P4::drawCamera(Camera& camera)
{
    _editor->setLineColor(_selectedWireframeColor);

    const int toRadians = M_PI / 180;

    mat4f inv = camera.cameraToWorldMatrix();


    float z1 = -camera.F();
    float x1 = camera.aspectRatio() * camera.height() * z1 / 2;
    float y1 = camera.height() * z1 / 2;

    float z2 = -camera.B();
    float x2 = camera.aspectRatio() * camera.height() * z2 / 2;
    float y2 = camera.height() * z2 / 2;

    /*now we define 8 points, using the following pattern:
    The first 4 points define the front clipping plane
    The next 4 define the back clipping plane
    For each plane we start from upper right corner and rotate clockwise
    */
    vec3f points[8];

    //near face
    points[0] = vec3f(x1, y1, z1);
    points[1] = vec3f(-x1, y1, z1);
    points[2] = vec3f(x1, -y1, z1);
    points[3] = vec3f(-x1, -y1, z1);

    //far face
    points[4] = vec3f(x2, y2, z2);
    points[5] = vec3f(-x2, y2, z2);
    points[6] = vec3f(x2, -y2, z2);
    points[7] = vec3f(-x2, -y2, z2);

    /*
    //and now we can draw the first plane
    _editor->drawLine(points[0], points[1]);
    _editor->drawLine(points[1], points[2]);
    _editor->drawLine(points[2], points[3]);
    _editor->drawLine(points[3], points[0]);

    //and the second plane
    _editor->drawLine(points[4], points[5]);
    _editor->drawLine(points[5], points[6]);
    _editor->drawLine(points[6], points[7]);
    _editor->drawLine(points[7], points[4]);

    //and finally we draw lines linking then
    _editor->drawLine(points[0], points[4]);
    _editor->drawLine(points[1], points[5]);
    _editor->drawLine(points[2], points[6]);
    _editor->drawLine(points[3], points[7]);
    */
}

void
P4::drawViewport(Camera& camera) {
    //we prepare to render the preview
    float aspectRatio = camera.aspectRatio();
    int vpHeight = 220;
    int x = 400, y = 30;
    glViewport(x, y, vpHeight * camera.aspectRatio(), vpHeight);
    glScissor(x, y, vpHeight * camera.aspectRatio(), vpHeight);
    glEnable(GL_SCISSOR_TEST);
    //render it
    _viewMode = ViewMode::GL_Renderer;
    renderScene();

    //and then go back to rendering the editor camera view
    _viewMode = ViewMode::Editor;
    glDisable(GL_SCISSOR_TEST);
    glViewport(0, 0, width(), height());
    _program.use();
}

inline void
P4::renderScene()
{
  if (auto camera = Camera::current())
  {
    if (_viewMode == ViewMode::GL_Renderer)
    {
      _renderer->setCamera(camera);
      _renderer->setImageSize(width(), height());
      _renderer->render();
      return;
    }
    if (_image == nullptr)
    {
      const auto w = width(), h = height();

      _image = new GLImage{w, h};
      _rayTracer->setImageSize(w, h);
      _rayTracer->setCamera(camera);
      _rayTracer->renderImage(*_image);
    }
    _image->draw(0, 0);
  }
}

constexpr auto CAMERA_RES = 0.01f;
constexpr auto ZOOM_SCALE = 1.01f;

void
P4::render()
{
    _program.use();
    if (_viewMode != ViewMode::Editor)
    {
        renderScene();
        return;
    }
    if (_moveFlags)
    {
        const auto delta = _editor->orbitDistance() * CAMERA_RES;
        auto d = vec3f::null();

        if (_moveFlags.isSet(MoveBits::Forward))
            d.z -= delta;
        if (_moveFlags.isSet(MoveBits::Back))
            d.z += delta;
        if (_moveFlags.isSet(MoveBits::Left))
            d.x -= delta;
        if (_moveFlags.isSet(MoveBits::Right))
            d.x += delta;
        if (_moveFlags.isSet(MoveBits::Up))
            d.y += delta;
        if (_moveFlags.isSet(MoveBits::Down))
            d.y -= delta;
        _editor->pan(d);
    }
    _editor->newFrame();

    auto ec = _editor->camera();
    const auto& p = ec->transform()->position();
    auto vp = vpMatrix(ec);

    getLights();
    _program.setUniformMat4(_vpMatrixLoc, vp);
    _program.setUniformVec4(_ambientLightLoc, _scene->ambientLight);
    _program.setUniformVec3(_cameraPositionLoc, p);

    //set lights
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

    //render objects
    SceneObjectListIterator* objectIterator = _scene->objectIterator();
    //first object is root, so we can skip it
    objectIterator->start();
    SceneObject* currentObject = objectIterator->next();
    //call a recursive render function for each object and render it
    while (currentObject) {
        recursiveRender(currentObject);

        currentObject = objectIterator->next();
    }
    objectIterator->dispose();

    if (_viewMode == ViewMode::Editor)
    {
        SceneObject* currentObj = dynamic_cast<SceneObject*>(_current);
        if (currentObj) {

            //draw axis over this object
            auto t = currentObj->transform();
            _editor->drawAxes(t->position(), mat3f{ t->rotation() });

            //draw light component
            Light* light = dynamic_cast<Light*>(currentObj->getComponent("Light"));
            if (light)
                drawLight(*light);

            //draw camera component
            Camera* camera = dynamic_cast<Camera*>(currentObj->getComponent("Camera"));
            if (camera) {
                drawViewport(*camera);
                camera->setCurrent(camera);
                drawCamera(*camera);
            }
        }
    }
}

void
P4::recursiveRender(SceneObject* object) {
    //render this object's primitive
    Primitive* primitive = dynamic_cast<Primitive*>(object->getComponent("Primitive"));
    if (primitive && object->visible) {
        drawPrimitive(*primitive);
    }
    //render its children
    SceneObjectListIterator* it = object->objectIterator();
    SceneObject* newObj = it->start();
    while (newObj) {
        recursiveRender(newObj);
        newObj = it->next();
    }
    it->dispose();
}

bool
P4::windowResizeEvent(int width, int height)
{
  _editor->camera()->setAspectRatio(float(width) / float(height));
  _viewMode = ViewMode::Editor;
  _image = nullptr;
  return true;
}

bool
P4::keyInputEvent(int key, int action, int mods)
{
    auto active = action != GLFW_RELEASE && mods == GLFW_MOD_ALT;

    switch (key)
    {
    case GLFW_KEY_W:
        _moveFlags.enable(MoveBits::Forward, active);
        break;
    case GLFW_KEY_S:
        _moveFlags.enable(MoveBits::Back, active);
        break;
    case GLFW_KEY_A:
        _moveFlags.enable(MoveBits::Left, active);
        break;
    case GLFW_KEY_D:
        _moveFlags.enable(MoveBits::Right, active);
        break;
    case GLFW_KEY_Q:
        _moveFlags.enable(MoveBits::Up, active);
        break;
    case GLFW_KEY_Z:
        _moveFlags.enable(MoveBits::Down, active);
        break;
    case GLFW_KEY_DELETE:
        deleteCurrentObject();
        break;
    case GLFW_KEY_F:
        if (isKeyPressed(GLFW_KEY_LEFT_ALT)) {
            SceneObject* object = dynamic_cast<SceneObject*>(_current);
            if (object)
                _editor->requestFocus(object->transform()->position());
        }
        break;
    }
    return false;
}

void
P4::deleteCurrentObject() {
    SceneObject* sceneObject = dynamic_cast<SceneObject*>(_current);
    if (sceneObject && sceneObject != _scene->root()) {
        Camera* camera = dynamic_cast<Camera*>(sceneObject->getComponent("Camera"));
        if (camera) {
            if (camera->current) {
                _viewMode = ViewMode::Editor;
            }
        }

        if (sceneObject->parent()) {
            sceneObject->parent()->removeChildSceneObject(sceneObject);
        }
        else {
            sceneObject->scene()->removeChildSceneObject(sceneObject);
        }
        _current = _scene;
        return;
    }
}

bool
P4::scrollEvent(double, double yOffset)
{
  if (ImGui::GetIO().WantCaptureMouse)
    return false;
  _editor->zoom(yOffset < 0 ? 1.0f / ZOOM_SCALE : ZOOM_SCALE);
  return true;
}

bool
P4::mouseButtonInputEvent(int button, int actions, int mods)
{
  if (ImGui::GetIO().WantCaptureMouse)
    return false;
  (void)mods;

  auto active = actions == GLFW_PRESS;

  /*
  if (button == GLFW_MOUSE_BUTTON_LEFT)
  {
    if (active)
    {
      cursorPosition(_pivotX, _pivotY);

      const auto ray = makeRay(_pivotX, _pivotY);
      auto minDistance = math::Limits<float>::inf();

      // **Begin picking of temporary scene objects
      // It should be replaced by your picking code
      for (const auto& o : _objects)
      {
        if (!o->visible)
          continue;

        auto component = o->component();
        float distance;

        if (auto p = dynamic_cast<Primitive*>(component))
          if (p->intersect(ray, distance) && distance < minDistance)
          {
            minDistance = distance;
            _current = o;
          }
      }
      // **End picking of temporary scene objects
    }
    return true;
  }*/
  if (button == GLFW_MOUSE_BUTTON_RIGHT)
    _dragFlags.enable(DragBits::Rotate, active);
  else if (button == GLFW_MOUSE_BUTTON_MIDDLE)
    _dragFlags.enable(DragBits::Pan, active);
  if (_dragFlags)
    cursorPosition(_pivotX, _pivotY);
  return true;
}

bool
P4::mouseMoveEvent(double xPos, double yPos)
{
  if (!_dragFlags)
    return false;
  _mouseX = (int)xPos;
  _mouseY = (int)yPos;

  const auto dx = (_pivotX - _mouseX);
  const auto dy = (_pivotY - _mouseY);

  _pivotX = _mouseX;
  _pivotY = _mouseY;
  if (dx != 0 || dy != 0)
  {
    if (_dragFlags.isSet(DragBits::Rotate))
    {
      const auto da = -_editor->camera()->viewAngle() * CAMERA_RES;
      isKeyPressed(GLFW_KEY_LEFT_ALT) ?
        _editor->orbit(dy * da, dx * da) :
        _editor->rotateView(dy * da, dx * da);
    }
    if (_dragFlags.isSet(DragBits::Pan))
    {
      const auto dt = -_editor->orbitDistance() * CAMERA_RES;
      _editor->pan(-dt * math::sign(dx), dt * math::sign(dy), 0);
    }
  }
  return true;
}
