#pragma once

#include <algorithm>

class Application;
class BaseComponent;

using BaseComponentMethod = void(BaseComponent::*)(Application*);
using ComponentMethod = std::tuple<BaseComponent*, BaseComponentMethod>;

class ILayer {
public:
	virtual std::string& name() const = 0;
	virtual ~ILayer() = default;
	virtual void Exec(Application* app) {
		for (ComponentMethod method : componentMethods)
		{
			(std::get<0>(method)->*std::get<1>(method))(app);
		}
	};
	void AddMethod(BaseComponent* component, void(BaseComponent::* func)(Application*)) { componentMethods.push_back(std::make_tuple(component, func)); }
	void RemoveMethod(BaseComponent* component, void(BaseComponent::* func)(Application*)) {
		componentMethods.erase(std::remove(componentMethods.begin(), componentMethods.end(), std::make_tuple(component, func)), componentMethods.end());
	}
protected:
	std::vector<ComponentMethod> componentMethods;
};