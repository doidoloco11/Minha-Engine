#pragma once

#include <algorithm>
#include <string>
#include <vector>
#include <stack>
#include <tuple>
#include "fvector.h"

class Application;
class BaseComponent;

using BaseComponentMethod = void(BaseComponent::*)(Application*);
using ComponentMethod = std::tuple<BaseComponent*, BaseComponentMethod>;

class ILayer {
public:
	virtual const std::string& name() const = 0;
	virtual ~ILayer() = default;
	virtual void Exec(Application* app) {
		for (ComponentMethod& method : componentMethods.data)
		{
			(std::get<0>(method)->*std::get<1>(method))(app);
		}
		componentMethods.processRemovals();
	};
	void AddMethod(BaseComponent* component, void(BaseComponent::* func)(Application*)) { componentMethods.push_back(std::make_tuple(component, func)); }
	void RemoveMethod(BaseComponent* component, void(BaseComponent::* func)(Application*)) {
		componentMethods.remove(std::make_tuple(component, func));
	}
protected:
	fvector<ComponentMethod> componentMethods;
};