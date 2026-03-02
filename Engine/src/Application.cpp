#include "../Includes/Application.h"
#include "../Includes/ILayer.h"
#include "../Includes/IComponent.h"
#include "../Includes/Object.h"

Application::Application() : Running(true) {}
Application::~Application() {}

void Application::Run() {
	while (Running) {
		if (m_Layers.size() == 0) continue;
		for (size_t i = 0; i < m_Layers.size(); i++) {
			std::shared_ptr<ILayer>& layer = m_Layers[i];
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
		if (criterion(layer.get())) { // Corrigido: chamada explícita do operador()
			layers.push_back(layer.get());
		}
	}
	return layers;
}

bool Application::AddLayerMethod(std::string name, unsigned int idx, BaseComponent* component, void(BaseComponent::* method)(Application*)) {
	std::vector<ILayer*> layers = GetLayers([name](ILayer* layer) -> bool { return layer->name() == name; });
	if (idx >= layers.size()) return false;
	layers[idx]->AddMethod(component, method);
	return true;
}

bool Application::RemoveLayerMethod(std::string name, unsigned int idx, BaseComponent* component, void(BaseComponent::* method)(Application*)) {
	std::vector<ILayer*> layers = GetLayers([name](ILayer* layer) -> bool { return layer->name() == name; });
	if (idx >= layers.size()) return false;
	layers[idx]->RemoveMethod(component, method);
	return true;
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