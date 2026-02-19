#pragma once

#include "../IComponent.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <glm/glm.hpp>

using namespace glm;

class MeshRenderer : public IComponent<MeshRenderer> {
public:
	std::vector<vec3> vertices;
	std::vector<vec3> normals;
	std::vector<vec2> uvs;
	std::vector<unsigned int> indices;
	MeshRenderer() : IComponent<MeshRenderer>() {
		glGenVertexArrays(1, &VAO);
		glGenBuffers(2, VEBO);
	};

private:
	GLuint VAO;
	GLuint VEBO[2];
};