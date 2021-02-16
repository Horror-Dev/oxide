#ifndef _INC_OXIDE_RENDERER_H
#define _INC_OXIDE_RENDERER_H

#include <string>
#include <glm/glm.hpp>
#include <GL/glew.h>
#include <vector>

namespace oxide {
	class Engine;
	namespace GL {
		class Renderer {

			class Texture;

			static int active_texture_vault;

		public:

			static void activeTexture(GLenum texture);
			static void activeTexture();

			static bool initalize();

			static void SetClearColor(const float red, const float green, const float blue, const float alpha);

			static void Enable(GLenum cap);
			static void Disable(GLenum cap);

			static void Clear(GLuint mask);

			class VAO {
				GLuint vao;
				std::vector<GLuint>mBuffers;
			public:
				VAO();
				void bind();
				void addVertexBufferObject(const std::vector<float>& data);
				void addVertexBufferObject(const std::vector<glm::vec2>& data);
				void addVertexBufferObject(const std::vector<glm::vec3>& data);
				void addVertexBufferObject(const std::vector<glm::vec4>& data);
				void draw(float count);
				~VAO();
			};

			class Shader {
				GLuint id;
			public:
				Shader(GLuint id);
				void use();
				~Shader();
				GLuint getLocation(const std::string& name);
				void setUniform(GLuint location, const float value);
				void setUniform(GLuint location, const glm::vec3& value);
				void setUniform(GLuint location, const glm::mat4& value);
				void setUniform(GLuint location, const float* value);
				void setUniform(GLuint location, Texture* texture);
				void bindAttribute(GLuint index, const std::string& name);

				static Shader* loadShader(const std::string& vertexShaderPath, const std::string& fragmentShaderPath);
			};

			class Texture {
				friend Shader;

			public:
				struct subTexture {
					glm::vec2 leftBottomUV;
					glm::vec2 rightTopUV;

					subTexture(const glm::vec2& _leftBottomUV, const glm::vec2& _rightTopUV)
						: leftBottomUV(_leftBottomUV)
						, rightTopUV(_rightTopUV)
					{}


					subTexture()
						: leftBottomUV(0.f)
						, rightTopUV(1.f)
					{}
				};

				Texture(const GLuint id, const int width, const int height);

				void bind();

				void add_Subtexture(std::string name, const glm::vec2& leftBottomUV, const glm::vec2& rightTopUV);
				const oxide::GL::Renderer::Texture::subTexture& getSubTexture(const std::string& name);

				static oxide::GL::Renderer::Texture* loadTexture(const std::string& path);

			private:

				std::unordered_map<std::string, subTexture> m_subTextures;
				GLuint id;
				int width;
				int height;

			};

			class Camera {

			public:
				void updateAttributes();

				//положение в мире
				glm::vec3 position;
				glm::vec3 front;
				glm::vec3 up;
				glm::vec3 right;
				glm::vec3 worldUp;

				//обзор
				float yaw;
				float pitch;

				float speed;
				float zoom;

				Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = -90.0f, float pitch = 0.0f);

				glm::mat4 getViewMatrix();

				glm::mat4 getProjection();

			};

			class CameraTransform {

				Camera* camera;

			public:

				enum CameraMovement {
					CM_FORWARD,
					CM_BACKWARD,
					CM_LEFT,
					CM_RIGHT
				};

				float mouse_sensitivity;

				CameraTransform(Camera* camera, float mouse_sensitivity);

				void mouse_transform(GLboolean constrainPitch = true);

				void keyboard_transform(CameraMovement movement);

			};

		};
	}
}

#endif