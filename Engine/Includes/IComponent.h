#pragma once

#include "Application.h"
#include "ILayer.h"

class BaseComponent {
public:
	virtual ~BaseComponent() = default;
};

template<class T>
class IComponent : public BaseComponent{
public:
	Object* obj;
	IComponent(Object* obj) : obj(obj) {
		static_assert(std::is_base_of<IComponent<T>, T>::value, "T must be derived from IComponent");
	}
	virtual ~IComponent() = default;
	virtual void ConnectMethods(Application* app) {
		app->AddLayerMethod("Terminate", 0, this, static_cast<BaseComponentMethod>(&IComponent::OnTerminate)));
	};

	virtual void OnTerminate(Application* app) {
		app->RemoveLayerMethod("Terminate", 0, this, static_cast<BaseComponentMethod>(&IComponent::OnTerminate)));
	};
};