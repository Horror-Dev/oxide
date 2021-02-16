#ifndef _OXIDE_BUTTON_H
#define _OXIDE_BUTTON_H

#include <oxide/oxide.h>
#include <oxide/Renderer.h>

#include <functional>

namespace oxide {

	class button {
		
		double top;
		double left;
		double width;
		double height;
		double height;

		std::string text;
		oxide::GL::Renderer::Texture* texture;
		oxide::GL::Renderer::Texture* mouse_on_button_texture;
		oxide::GL::Renderer::Texture* pressed_texture;
		oxide::GL::Renderer::Shader* shader;

		oxide::GL::Renderer::VAO* vao;

		std::function<void> callback;

	public:
		button(double top, double left, double width, double height,
			const std::string& text,
			
			//button textures
			oxide::GL::Renderer::Texture* texture, // default state
			oxide::GL::Renderer::Texture* pressed_texture, // on pressed state
			oxide::GL::Renderer::Texture* mouse_on_button_texture, // when the cursor is on the button

			oxide::GL::Renderer::Shader* shader);

		void setCallBack(std::function<void> callback);

		void update_for_3d();

	};
}

#endif