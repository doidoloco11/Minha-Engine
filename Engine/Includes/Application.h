#pragma once

#include <string>
#include <functional>
#include <vector>
#include <stack>
#include <memory>
#include "fvector.h"

class ILayer;
class Object;

using ObjectCriterion = std::function<bool(Object*)>;
using LayerCriterion = std::function<bool(ILayer*)>;

class Application {
public:
	bool Running;

	Application();
	~Application();

	void Run();

	void AddLayer(ILayer* layer);
	void RemoveLayer(ILayer* layer);
	std::vector<ILayer*> GetLayers(LayerCriterion criterion);
	std::vector<std::shared_ptr<ILayer>> GetAllLayers() { m_Layers.processRemovals(); return m_Layers.data; };

	Object* CreateObject(const std::string& name);
	void RemoveObject(Object* object);
	std::vector<Object*> GetObjects(ObjectCriterion criterion);
	std::vector<std::shared_ptr<Object>> GetAllObjects() { m_Objects.processRemovals(); return m_Objects.data; };
private:
	fvector<std::shared_ptr<ILayer>> m_Layers;
	fvector<std::shared_ptr<Object>> m_Objects;
};