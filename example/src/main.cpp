#include <oxide/oxide.h>
#include <oxide/Window.h>
#include <oxide/Events.h>
#include <oxide/Renderer.h>

#include <GLFW/glfw3.h>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/gtc/type_ptr.hpp>

int main() {
	oxide::Engine* engine = new oxide::Engine();
	oxide::Engine::Logger* logger = engine->getLogger();

	logger->setLogOutPutFile("log.txt");

	oxide::Window::pre_initialize();

	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	oxide::Window::initialize("oxide", 1280, 640);
	oxide::Events::init();
	
	if (!oxide::GL::Renderer::initalize()) {
		return -1;
	}

	oxide::Engine::ResourceManager* resource_manager = new oxide::Engine::ResourceManager();

	if (!resource_manager->loadResources("resources/resources.json")) {
		delete resource_manager;
		delete engine;
		return -1;
	}

	oxide::GL::Renderer::Enable(GL_DEPTH_TEST);

	oxide::GL::Renderer::Shader* shader = reinterpret_cast<oxide::GL::Renderer::Shader*>(resource_manager->getResource("example_3d"));
	oxide::GL::Renderer::Texture* texture = reinterpret_cast<oxide::GL::Renderer::Texture*>(resource_manager->getResource("t"));
	oxide::GL::Renderer::VAO* vao = new oxide::GL::Renderer::VAO();

	oxide::GL::Renderer::Camera* cam = new oxide::GL::Renderer::Camera();
	oxide::GL::Renderer::CameraTransform* cam_transform = new oxide::GL::Renderer::CameraTransform(cam, 0.1f);

	vao->addVertexBufferObject({
		{-0.5f, -0.5f, -0.5f },
		{ 0.5f, -0.5f, -0.5f },
		{ 0.5f, 0.5f, -0.5f  },
		{ 0.5f,  0.5f, -0.5f },
		{-0.5f,  0.5f, -0.5f },
		{-0.5f, -0.5f, -0.5f },

		{-0.5f, -0.5f,  0.5f },
		{ 0.5f, -0.5f,  0.5f },
		{ 0.5f,  0.5f,  0.5f },
		{ 0.5f,  0.5f,  0.5f },
		{-0.5f,  0.5f,  0.5f },
		{-0.5f, -0.5f,  0.5f },

		{-0.5f,  0.5f,  0.5f },
		{-0.5f,  0.5f, -0.5f },
		{-0.5f, -0.5f, -0.5f },
		{-0.5f, -0.5f, -0.5f },
		{-0.5f, -0.5f,  0.5f },
		{-0.5f,  0.5f,  0.5f },

		{0.5f,  0.5f,  0.5f  },
		{ 0.5f,  0.5f, -0.5f  },
		{ 0.5f, -0.5f, -0.5f  },
		{0.5f, -0.5f, -0.5f  },
		{ 0.5f, -0.5f,  0.5f  },
		{0.5f,  0.5f,  0.5f  },

		{-0.5f, -0.5f, -0.5f },
		{ 0.5f, -0.5f, -0.5f },
		{ 0.5f, -0.5f,  0.5f },
		{ 0.5f, -0.5f,  0.5f },
		{-0.5f, -0.5f,  0.5f },
		{-0.5f, -0.5f, -0.5f },

		{-0.5f,  0.5f, -0.5f },
		{ 0.5f,  0.5f, -0.5f },
		{ 0.5f,  0.5f,  0.5f },
		{ 0.5f,  0.5f,  0.5f },
		{-0.5f,  0.5f,  0.5f },
		{-0.5f,  0.5f, -0.5f }
	});

	vao->addVertexBufferObject({
		{0.0f, 0.0f},
		{1.0f, 0.0f},
		{1.0f, 1.0f},
		{1.0f, 1.0f},
		{0.0f, 1.0f},
		{0.0f, 0.0f},

		{0.0f, 0.0f},
		{1.0f, 0.0f},
		{1.0f, 1.0f},
		{1.0f, 1.0f},
		{0.0f, 1.0f},
		{0.0f, 0.0f},

		{1.0f, 0.0f},
		{1.0f, 1.0f},
		{0.0f, 1.0f},
		{0.0f, 1.0f},
		{0.0f, 0.0f},
		{1.0f, 0.0f},

		{1.0f, 0.0f},
		{1.0f, 1.0f},
		{0.0f, 1.0f},
		{0.0f, 1.0f},
		{0.0f, 0.0f},
		{1.0f, 0.0f},

		{0.0f, 1.0f},
		{1.0f, 1.0f},
		{1.0f, 0.0f},
		{1.0f, 0.0f},
		{0.0f, 0.0f},
		{0.0f, 1.0f},

		{0.0f, 1.0f},
		{1.0f, 1.0f},
		{1.0f, 0.0f},
		{1.0f, 0.0f},
		{0.0f, 0.0f},
		{0.0f, 1.0f}
	});


	GLuint modelLocation = shader->getLocation("model");
	GLuint viewLocation = shader->getLocation("view");
	GLuint projectionLocation = shader->getLocation("projection");

	bool isToggled = true;

	oxide::Events::ToggleCursor();

	while (!oxide::Window::isShouldClose()){
		oxide::GL::Renderer::SetClearColor(0.5f, 0.5f, 0.5f, 0.5f);
		oxide::GL::Renderer::Clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		if (oxide::Events::jpressed(GLFW_KEY_ESCAPE)) {
			oxide::Window::setShouldClose(true);
		}

		if (oxide::Events::jpressed(GLFW_KEY_TAB)) {
			isToggled = !isToggled;
			oxide::Events::ToggleCursor();
		}
		
		if (oxide::Events::pressed(GLFW_KEY_W)) {
			cam_transform->keyboard_transform(oxide::GL::Renderer::CameraTransform::CameraMovement::CM_FORWARD);
		}
		if (oxide::Events::pressed(GLFW_KEY_S)) {
			cam_transform->keyboard_transform(oxide::GL::Renderer::CameraTransform::CameraMovement::CM_BACKWARD);
		}
		if (oxide::Events::pressed(GLFW_KEY_A)) {
			cam_transform->keyboard_transform(oxide::GL::Renderer::CameraTransform::CameraMovement::CM_LEFT);
		}
		if (oxide::Events::pressed(GLFW_KEY_D)) {
			cam_transform->keyboard_transform(oxide::GL::Renderer::CameraTransform::CameraMovement::CM_RIGHT);
		}

		texture->bind();

		shader->use();

		if (isToggled) {
			cam_transform->mouse_transform();
		}

		glm::mat4 model = glm::mat4(1.0f);
		glm::mat4 view = glm::mat4(1.0f);
		glm::mat4 projection = glm::mat4(1.0f);
		model = glm::rotate(model, (float)oxide::Events::getTime() / 2 * glm::radians(50.0f), glm::vec3(0.f, 0.2f, 0.f));
		view = cam->getViewMatrix();
		projection = cam->getProjection();

		shader->setUniform(modelLocation, model);
		shader->setUniform(viewLocation,  view);
		shader->setUniform(projectionLocation, projection);
		
		vao->draw(36);

		oxide::Window::swapBuffers();
		oxide::Events::pullEvents();
	}

	oxide::Window::destroy();

	resource_manager->unloadResources();
	
	delete resource_manager;
	delete vao;
	delete engine;

	return 0;
}