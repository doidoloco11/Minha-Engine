#pragma once

#include "../IComponent.h"
#include "../Object.h"
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

using namespace glm;

class Transform : public IComponent<Transform> {
public:
	vec3 position;
	vec3 rotation;
	vec3 scale;
	Transform* Parent = nullptr;
	Transform(Object* obj) : IComponent<Transform>(obj), position(0.0f), rotation(0.0f), scale(1.0f) {
	};
	~Transform() override = default;
	mat4 GetModelMatrix() {
		mat4 model = mat4(1.0f);
		model = translate(model, position);
		model = model * mat4_cast(quat(rotation));
		model = glm::scale(model, scale);
		if (Parent != nullptr) 	model = Parent->GetModelMatrix() * model;
		return model;
	}
	// Adiciona métodos GetForward e GetUp para uso na câmera
	vec3 GetForward() const {
		quat q = quat(rotation);
		return q * vec3(0.0f, 0.0f, -1.0f);
	}
	vec3 GetRight() const {
		quat q = quat(rotation);
		return q * vec3(1.0f, 0.0f, 0.0f);
	}
	vec3 GetUp() const {
		quat q = quat(rotation);
		return q * vec3(0.0f, 1.0f, 0.0f);
	}
};
class Script : public IComponent<Script> {
public:
	Script(Object* obj) : IComponent<Script>(obj) {};
	virtual ~Script() override = default;
	virtual void Start(Application* app) {};
	virtual void Update(Application* app) {};
	virtual void FixedUpdate(Application* app) {};
	virtual void LateUpdate(Application* app) {};
	virtual void OnRender(Application* app) {};

	void ConnectMethods(Application* app) override {
		IComponent<Script>::ConnectMethods(app);
		app->GetLayers([](ILayer* layer) -> bool {return layer->name() == std::string("Start"); })[0]->AddMethod(this, static_cast<BaseComponentMethod>(&Script::Start));
		app->GetLayers([](ILayer* layer) -> bool {return layer->name() == std::string("Update"); })[0]->AddMethod(this, static_cast<BaseComponentMethod>(&Script::Update));
		app->GetLayers([](ILayer* layer) -> bool {return layer->name() == std::string("FixedUpdate"); })[0]->AddMethod(this, static_cast<BaseComponentMethod>(&Script::FixedUpdate));
		app->GetLayers([](ILayer* layer) -> bool {return layer->name() == std::string("LateUpdate"); })[0]->AddMethod(this, static_cast<BaseComponentMethod>(&Script::LateUpdate));
		app->GetLayers([](ILayer* layer) -> bool {return layer->name() == std::string("Render"); })[0]->AddMethod(this, static_cast<BaseComponentMethod>(&Script::OnRender));
	}
	virtual void OnTerminate(Application* app) override {
		IComponent<Script>::OnTerminate(app);
		app->GetLayers([](ILayer* layer) -> bool {return layer->name() == std::string("Start"); })[0]->RemoveMethod(this, static_cast<BaseComponentMethod>(&Script::Start));
		app->GetLayers([](ILayer* layer) -> bool {return layer->name() == std::string("Update"); })[0]->RemoveMethod(this, static_cast<BaseComponentMethod>(&Script::Update));
		app->GetLayers([](ILayer* layer) -> bool {return layer->name() == std::string("FixedUpdate"); })[0]->RemoveMethod(this, static_cast<BaseComponentMethod>(&Script::FixedUpdate));
		app->GetLayers([](ILayer* layer) -> bool {return layer->name() == std::string("LateUpdate"); })[0]->RemoveMethod(this, static_cast<BaseComponentMethod>(&Script::LateUpdate));
		app->GetLayers([](ILayer* layer) -> bool {return layer->name() == std::string("Render"); })[0]->RemoveMethod(this, static_cast<BaseComponentMethod>(&Script::OnRender));
	}
};