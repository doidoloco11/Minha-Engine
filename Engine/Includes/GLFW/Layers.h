#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "../AssetsHelper/AssetRegister.h"
#include "../ILayer.h"
#include "../Application.h"

class StartLayer : public ILayer {
public:
	char* windowName = const_cast<char*>("window");
	int width = 800;
	int height = 600;

	const std::string& name() const override { return "Start"; };
	void Exec(Application* app) override {
		glfwInit();

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		GLFWwindow* window = glfwCreateWindow(width, height, windowName, nullptr, nullptr);

		glfwMakeContextCurrent(window);
		gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

		glEnable(GL_DEPTH_TEST);
		glEnable(GL_BLEND);
		glEnable(GL_CULL_FACE);

		glDepthFunc(GL_LESS);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glCullFace(GL_BACK);
		glFrontFace(GL_CCW);

		ILayer::Exec(app);

		app->RemoveLayer(this);
	};
};
class UpdateLayer : public ILayer {
public:
	float time = 0;
	float deltaTime = 0.1;
	const std::string& name() const override { return "Update"; };
	void Exec(Application* app) override {
		deltaTime = glfwGetTime() - time;
		time = glfwGetTime();
		glfwPollEvents();
		ILayer::Exec(app);
	};
};
class FixedUpdateLayer : public ILayer {
public:
	const int fixedUpdateRate = 90;
	const int maxIterations = 10;
	const float fixedDeltaTime = 1.0f / fixedUpdateRate;
	float time = 0;
	const std::string& name() const override { return "FixedUpdate"; };
	void Exec(Application* app) override {
		float deltaTime = glfwGetTime() - time;
		int iterations = 0;
		while (deltaTime > fixedDeltaTime && iterations < maxIterations) {
			ILayer::Exec(app);
			deltaTime -= fixedDeltaTime;
			iterations++;
		}
		time = glfwGetTime() - deltaTime;
	};
};
class LateUpdateLayer : public ILayer {
public:
	const std::string& name() const override { return "LateUpdate"; };
	void Exec(Application* app) override {
		ILayer::Exec(app);
	};
};
class ClearScreenLayer : public ILayer {
public:
	const std::string& name() const override { return "ClearScreen"; };
	void Exec(Application* app) override {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		ILayer::Exec(app);
	};
};
class RenderLayer : public ILayer {
public:
	const std::string& name() const override { return "Render"; };
	void Exec(Application* app) override {
		ILayer::Exec(app);
	};
};
class SwapBufferLayer : public ILayer {
public:
	const std::string& name() const override { return "SwapBuffer"; };
	void Exec(Application* app) override {
		glfwSwapBuffers(glfwGetCurrentContext());
		ILayer::Exec(app);
	};
};
class TerminateLayer : public ILayer {
public:
	const std::string& name() const override { return "Terminate"; };
	void Exec(Application* app) override {
		if (glfwWindowShouldClose(glfwGetCurrentContext())) {
			ILayer::Exec(app);
			app->Running = false;
			AssetRegister::getInstance().Unload();
			glfwTerminate();
		}
	};
};
