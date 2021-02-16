#include "button.h"

#include <oxide/Events.h>
#include <glfw/glfw3.h>
 
oxide::button::button(
	double top, double left, double width, double height, 
	const std::string& text, 
	oxide::GL::Renderer::Texture* texture, 
	oxide::GL::Renderer::Texture* pressed_texture, 
	oxide::GL::Renderer::Texture* mouse_on_button_texture,
	oxide::GL::Renderer::Shader* shader)
	:
	top(top),
	left(left),
	width(width),
	height(height),
	text(text),
	texture(texture),
	pressed_texture(pressed_texture),
	mouse_on_button_texture(mouse_on_button_texture),
	shader(shader){
	vao = new oxide::GL::Renderer::VAO();
	
	vao->addVertexBufferObject({
		{ -1.0f, -1.0f, 0.0f },
		{ -1.0f,  1.0f, 0.0f },
		{  1.0f,  1.0f, 0.0f },
		{  1.0f, -1.0f, 0.0f },
		{ -1.0f, -1.0f, 0.0f }
	});

	vao->addVertexBufferObject({
		{ 1.0f, 1.0f },
		{ 1.0f, 0.0f },
		{ 0.0f, 0.0f },
		{ 0.0f, 1.0f }
	});
}

void oxide::button::setCallBack(std::function<void> callback){
	this->callback = callback;
}

void oxide::button::update_for_3d(){
	oxide::GL::Renderer::Disable(GL_DEPTH);

	if (oxide::Events::deltaX >= left && oxide::Events::deltaX <= left + width &&
		oxide::Events::deltaY >= top && oxide::Events::deltaY <= top + height) {

		if (oxide::Events::jclicked(GLFW_MOUSE_BUTTON_1)) {
			pressed_texture->bind();
			callback();
		}
		else {
			mouse_on_button_texture->bind();
		}
	}
	else {
		texture->bind();
	}

	shader->use();
	vao->draw();

	oxide::GL::Renderer::Enable(GL_DEPTH);
}
