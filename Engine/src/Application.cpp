#include "../Includes/Application.h"
#include "../Includes/ILayer.h"
#include "../Includes/IComponent.h"
#include "../Includes/Object.h"

Application::Application() : Running(true) {}
Application::~Application() {}

void Application::Run() {
	while (Running) {
		for (ILayer* layer : m_Layers) {
			layer->Exec(this);
		}
	}
}

void Application::AddLayer(ILayer* layer) {
	m_Layers.push_back(layer);
}

void Application::RemoveLayer(ILayer* layer) {
	m_Layers.erase(std::remove(m_Layers.begin(), m_Layers.end(), layer), m_Layers.end());
}

std::vector<ILayer*> Application::GetLayers(LayerCriterion criterion) {
	std::vector<ILayer*> layers;
	for (ILayer* layer : m_Layers) {
		if (criterion(layer)) {
			layers.push_back(layer);
		}
	}
	return layers;
}

Object* Application::CreateObject(const std::string& name) {
	Object* object = new Object(this, name);
	m_Objects.push_back(object);
	return object;
}

void Application::RemoveObject(Object* object) {
	m_Objects.erase(std::remove(m_Objects.begin(), m_Objects.end(), object), m_Objects.end());
	delete object;
}

std::vector<Object*> Application::GetObjects(ObjectCriterion criterion) {
	std::vector<Object*> objects;
	for (Object* object : m_Objects) {
		if (criterion(object)) {
			objects.push_back(object);
		}
	}
	return objects;
}