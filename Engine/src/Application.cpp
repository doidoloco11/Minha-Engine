#include "../Includes/Application.h"
#include "../Includes/ILayer.h"
#include "../Includes/IComponent.h"
#include "../Includes/Object.h"

Application::Application() : Running(true) {}
Application::~Application() {}

void Application::Run() {
	while (Running) {
		for (std::shared_ptr<ILayer>& layer : m_Layers.data) {
			layer->Exec(this);
		}
		m_Layers.processRemovals();
		m_Objects.processRemovals();
	}
}

void Application::AddLayer(ILayer* layer) {
	m_Layers.push_back(std::shared_ptr<ILayer>(layer));
}

void Application::RemoveLayer(ILayer* layer) {
	m_Layers.remove(std::shared_ptr<ILayer>(layer));
}

std::vector<ILayer*> Application::GetLayers(LayerCriterion criterion) {
	std::vector<ILayer*> layers;
	m_Layers.processRemovals();
	for (std::shared_ptr<ILayer>& layer : m_Layers.data) {
		if (criterion(layer.get())) {
			layers.push_back(layer.get());
		}
	}
	return layers;
}

Object* Application::CreateObject(const std::string& name) {
	std::shared_ptr<Object> object = std::make_shared<Object>(this, name);
	m_Objects.push_back(object);
	return object.get();
}

void Application::RemoveObject(Object* object) {
	m_Objects.remove(std::shared_ptr<Object>(object));
}

std::vector<Object*> Application::GetObjects(ObjectCriterion criterion) {
	std::vector<Object*> objects;
	m_Objects.processRemovals();
	for (std::shared_ptr<Object> object : m_Objects.data) {
		if (criterion(object.get())) {
			objects.push_back(object.get());
		}
	}
	return objects;
}