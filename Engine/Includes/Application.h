#pragma once

#include <string>
#include <functional>
#include <vector>

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
	std::vector<ILayer*> GetAllLayers() { return m_Layers; };

	Object* CreateObject(const std::string& name);
	void RemoveObject(Object* object);
	std::vector<Object*> GetObjects(ObjectCriterion criterion);
	std::vector<Object*> GetAllObjects() { return m_Objects; };
private:
	std::vector<ILayer*> m_Layers;
	std::vector<Object*> m_Objects;
};