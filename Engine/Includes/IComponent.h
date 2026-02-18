#pragma once


class BaseComponent {
public:
	virtual ~BaseComponent() = default;
};

template<class T>
class IComponent : public BaseComponent{
public:
	IComponent() {
		static_assert(std::is_base_of<IComponent<T>, T>::value, "T must be derived from IComponent");
	}
	virtual void ConnectMethods(Application* app) {
		app->GetLayers([](ILayer* layer) -> bool {return layer->name() == std::string("Terminate"); })[0]->AddMethod(this, static_cast<BaseComponentMethod>(&IComponent::OnTerminate));
	};

	virtual void OnTerminate(Application* app) {
		app->GetLayers([](ILayer* layer) -> bool {return layer->name() == std::string("Terminate"); })[0]->RemoveMethod(this, static_cast<BaseComponentMethod>(&IComponent::OnTerminate));
	};
};