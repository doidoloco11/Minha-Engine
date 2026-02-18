#pragma once

#include <algorithm>
#include "IComponent.h"

class Application;

class Object {
public:
	Object(Application* app, const std::string& name);
	~Object();
	
	template<class T>
	T* AddComponent() {
		static_assert(std::is_base_of<IComponent<T>, T>::value, "T must be derived from IComponent");
		T* component = new T();
		component->ConnectMethods(m_App);
		m_Components.push_back(component);
		return component;
	}
	template<class T>
	std::vector<T*> GetComponents() {
		static_assert(std::is_base_of<IComponent<T>, T>::value, "T must be derived from IComponent");
		std::vector<T*> components;
		for (BaseComponent* component : m_Components) {
			if (T* casted = dynamic_cast<T*>(component)) {
				components.push_back(casted);
			}
		}
		return components;
	}
	void RemoveComponent(BaseComponent* component) {
		m_Components.erase(std::remove(m_Components.begin(), m_Components.end(), component), m_Components.end());
		delete component;
	}
private:
	Application* m_App;
	std::string m_Name;
	std::vector<BaseComponent*> m_Components;
};