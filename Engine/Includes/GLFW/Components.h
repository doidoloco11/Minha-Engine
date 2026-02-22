#pragma once

#include "../IComponent.h"
#include "../Object.h"
#include "Assets.h"
#include "../General/Components.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <functional>

using namespace glm;

class Material : public IComponent<Material> {
public:
	Shader* shader;
	std::function<void(Shader*)> SetUniforms;
	Material(Object* obj) : IComponent<Material>(obj) {};
	~Material() override = default;

	void Use() {
		glUseProgram(shader->Id);
		SetUniforms(shader);
	}
};
class MeshRenderer : public IComponent<MeshRenderer> {
public:
	std::vector<vec3> vertices;
	std::vector<vec3> normals;
	std::vector<vec2> uvs;
	std::vector<unsigned int> indices;

	Material* material;
	Transform* transform;
	MeshRenderer(Object* obj) : IComponent<MeshRenderer>(obj) {
		glGenVertexArrays(1, &VAO);
		glGenBuffers(3, VEBO);
		std::vector<Material*> mat = obj->GetComponents<Material>();
		assert(mat.size() > 0 && "object must have material");
		material = mat[0];

		std::vector<Transform*> tra = obj->GetComponents<Transform>();
		assert(mat.size() > 0 && "object must have transform");
		transform = tra[0];
	};
	~MeshRenderer() override {
		glDeleteVertexArrays(1, &VAO);
		glDeleteBuffers(3, VEBO);
	};
	void ConnectMethods(Application* app) override{
		IComponent<MeshRenderer>::ConnectMethods(app);
		app->GetLayers([](ILayer* layer) -> bool {return layer->name() == std::string("Render"); })[0]->AddMethod(this, static_cast<BaseComponentMethod>(&MeshRenderer::Render));
	}
	void OnTerminate(Application* app) override {
		IComponent<MeshRenderer>::OnTerminate(app);
		app->GetLayers([](ILayer* layer) -> bool {return layer->name() == std::string("Render"); })[0]->RemoveMethod(this, static_cast<BaseComponentMethod>(&MeshRenderer::Render));
	}
	void UpdateData() {
		std::vector<float> data;
		data.insert(data.end(), vertices.begin(), vertices.end());
		data.insert(data.end(), normals.begin(), normals.end());
		data.insert(data.end(), uvs.begin(), uvs.end());

		glBindVertexArray(VAO);

		glBindBuffer(GL_ARRAY_BUFFER, VEBO[0]);
		glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(float), data.data(), GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vec3), (void*)0);

		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(vec3), (void*)(vertices.size() * sizeof(vec3)));

		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(vec2), (void*)(vertices.size() * sizeof(vec3) + normals.size() * sizeof(vec3)));
		
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VEBO[1]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
	void RecalculateNormals() {
		normals.clear();
		normals.resize(vertices.size());
		memset(normals.data(), 0, normals.size() * sizeof(vec3));

		for (size_t i = 0; i < indices.size(); i+=3)
		{
			vec3 v0 = vertices[indices[i]];
			vec3 v1 = vertices[indices[i + 1]];
			vec3 v2 = vertices[indices[i + 2]];

			vec3 e0 = v1 - v0;
			vec3 e1 = v2 - v0;

			vec3 normal = cross(e0, e1);

			normals[indices[i]] += normal;
			normals[indices[i + 1]] += normal;
			normals[indices[i + 2]] += normal;
		}

		for (size_t i = 0; i < normals.size(); i++)
		{
			normals[i] = normalize(normals[i]);
		}
	}
	void Render(Application* app) {
		if (uvs.size() != vertices.size() || normals.size() != vertices.size()) {
			if (uvs.size() != vertices.size()) {
				uvs.resize(vertices.size());
				memset(uvs.data(), 0, uvs.size() * sizeof(vec2));
			}
			if (normals.size() != vertices.size()) {
				RecalculateNormals();
			}
			UpdateData();
		}
		mat4 m_model = transform->GetModelMatrix();
		glUniformMatrix4fv(material->shader->GetUniformId("m_model"), 1, GL_FALSE, glm::value_ptr(m_model));
		material->Use();
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}
private:
	GLuint VAO;
	GLuint VEBO[2];
};
class Camera : public IComponent<Camera> {
	Transform* transform;
	GLuint UBO;
public:
	float fov = pi<float>() / 3.0f;
	float nearPlane = 0.1f;
	float farPlane = 1000.0f;
	Camera(Object* obj) : IComponent<Camera>(obj) {
		transform = obj->GetComponents<Transform>()[0];
		glGenBuffers(1, &UBO);
		glBindBuffer(GL_UNIFORM_BUFFER, UBO);
		glBufferData(GL_UNIFORM_BUFFER, sizeof(float) * 40, nullptr, GL_DYNAMIC_DRAW);
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
	};
	~Camera() override {
		glDeleteBuffers(1, &UBO);
	};

	void UpdateCamera(Application* app){
		GLFWwindow* window = glfwGetCurrentContext();
		int width, height;
		glfwGetFramebufferSize(window, &width, &height);

		mat4 m_projection = perspective(fov, (float)width / (float)height, nearPlane, farPlane);
		mat4 m_view = lookAt(transform->position, transform->position + transform->GetForward(), transform->GetUp());

		//0 - 15: projection matrix
		//16 - 31: view matrix
		//32 - 34: camera position
		//35: fov
		//36 - 37: near and far planes
		//38 - 39: width and height
		GLfloat data[40];
		memcpy(data, value_ptr(m_projection), sizeof(float) * 16);
		memcpy(&data[16], value_ptr(m_view), sizeof(float) * 16);
		memcpy(&data[32], value_ptr(transform->position), sizeof(float) * 3);
		data[35] = fov;
		data[36] = nearPlane;
		data[37] = farPlane;
		memcpy(&data[38], &width, sizeof(int));
		memcpy(&data[39], &height, sizeof(int));

		glBindBuffer(GL_UNIFORM_BUFFER, UBO);
		glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(float) * 40, data);
		glBindBufferBase(GL_UNIFORM_BUFFER, 0, UBO);
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
	}

	void ConnectMethods(Application* app) override {
		IComponent<Camera>::ConnectMethods(app);
		app->GetLayers([](ILayer* layer) -> bool {return layer->name() == std::string("ClearScreen"); })[0]->AddMethod(this, static_cast<BaseComponentMethod>(&Camera::UpdateCamera));
	}

	void OnTerminate(Application* app) override {
		IComponent<Camera>::OnTerminate(app);
		app->GetLayers([](ILayer* layer) -> bool {return layer->name() == std::string("ClearScreen"); })[0]->RemoveMethod(this, static_cast<BaseComponentMethod>(&Camera::UpdateCamera));
	}
};