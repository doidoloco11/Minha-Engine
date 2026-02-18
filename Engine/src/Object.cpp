#include "../Includes/Application.h"
#include "../Includes/ILayer.h"
#include "../Includes/IComponent.h"
#include "../Includes/Object.h"

Object::Object(Application* app, const std::string& name) : m_App(app), m_Name(name) {}
Object::~Object() {
	for (BaseComponent* component : m_Components) {
		delete component;
	}
}