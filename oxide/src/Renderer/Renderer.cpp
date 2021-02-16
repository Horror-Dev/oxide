#include "Renderer.h"
#include "..//oxide/oxide.h"
#include "..//stb_image/stb_image.h"

#include "..//Window/Window.h"

#include <fstream>
#define GLEW_STATIC
#include <gl/glew.h>
#include <glm/gtc/type_ptr.hpp>

#include<iostream>


#include "..//Events/Events.h"

int oxide::GL::Renderer::active_texture_vault = 0;

void oxide::GL::Renderer::activeTexture(GLenum texture){
    glActiveTexture(texture);
}

void oxide::GL::Renderer::activeTexture(){
    active_texture_vault++;
    glActiveTexture(GL_TEXTURE0 + active_texture_vault);
}

bool oxide::GL::Renderer::initalize(){
	oxide::Engine::Logger* logger = oxide::Engine::getLastInstance()->getLogger();
	glewExperimental = GL_TRUE;
	GLenum code = glewInit();

	if (GLEW_OK != code) {
		*logger << "glew initalization error:" << glewGetString(code) << "\n";
        logger->add_log(oxide::Engine::Logger::log_type::lt_error);
        return false;
	}

	return true;
}

void oxide::GL::Renderer::SetClearColor(const float red, const float green, const float blue, const float alpha){
	glClearColor(red, green, blue, alpha);
}

void oxide::GL::Renderer::Enable(GLenum cap){
	glEnable(cap);
}

void oxide::GL::Renderer::Disable(GLenum cap){
	glDisable(cap);
}

void oxide::GL::Renderer::Clear(GLuint mask){
	glClear(mask);
}

oxide::GL::Renderer::VAO::VAO(){
	glGenVertexArrays(1, &vao);
	bind();
}

void oxide::GL::Renderer::VAO::bind(){
	glBindVertexArray(vao);
}

void oxide::GL::Renderer::VAO::addVertexBufferObject(const std::vector<float>& data){
	GLuint vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(float), data.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(mBuffers.size(), 1, GL_FLOAT, GL_FALSE, 0, nullptr);
	mBuffers.push_back(vbo);
}

void oxide::GL::Renderer::VAO::addVertexBufferObject(const std::vector<glm::vec2>& data) {
	GLuint vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(glm::vec2), data.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(mBuffers.size(), 2, GL_FLOAT, GL_FALSE, 0, nullptr);
	mBuffers.push_back(vbo);
}

void oxide::GL::Renderer::VAO::addVertexBufferObject(const std::vector<glm::vec3>& data) {
	GLuint vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(glm::vec3), data.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(mBuffers.size(), 3, GL_FLOAT, GL_FALSE, 0, nullptr);
	mBuffers.push_back(vbo);
}

void oxide::GL::Renderer::VAO::addVertexBufferObject(const std::vector<glm::vec4>& data) {
	GLuint vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(glm::vec4), data.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(mBuffers.size(), 4, GL_FLOAT, GL_FALSE, 0, nullptr);
	mBuffers.push_back(vbo);
}

void oxide::GL::Renderer::VAO::draw(float count){
	bind();
	for (size_t i = 0; i < mBuffers.size(); i++) {
		glEnableVertexAttribArray(i);
	}
	glDrawArrays(GL_TRIANGLES, 0, count);
	for (size_t i = 0; i < mBuffers.size(); i++) {
		glDisableVertexAttribArray(i);
	}
}

oxide::GL::Renderer::VAO::~VAO(){
	glDeleteBuffers(mBuffers.size(), mBuffers.data());
	glDeleteVertexArrays(1, &vao);
}

oxide::GL::Renderer::Shader::Shader(GLuint id) : id(id){}

void oxide::GL::Renderer::Shader::use(){
	glUseProgram(id);
}

oxide::GL::Renderer::Shader::~Shader(){
	glDeleteProgram(id);
}

GLuint oxide::GL::Renderer::Shader::getLocation(const std::string& name){
	return glGetUniformLocation(id, name.c_str());
}

void oxide::GL::Renderer::Shader::setUniform(GLuint location, const float value){
	glUniform1f(location, value);
}

void oxide::GL::Renderer::Shader::setUniform(GLuint location, const glm::vec3& value){
	glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));
}

void oxide::GL::Renderer::Shader::setUniform(GLuint location, const glm::mat4& value){
	glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));
}

void oxide::GL::Renderer::Shader::setUniform(GLuint location, const float* value){
	glUniformMatrix4fv(location, 1, GL_FALSE, value);
}

void oxide::GL::Renderer::Shader::setUniform(GLuint location, Texture* texture){
    glUniform1i(location, texture->id);
}

void oxide::GL::Renderer::Shader::bindAttribute(GLuint index, const std::string& name){
    glBindAttribLocation(this->id, index, name.c_str());
}

oxide::GL::Renderer::Shader* oxide::GL::Renderer::Shader::loadShader(const std::string& vertexShaderPath, const std::string& fragmentShaderPath){
    oxide::Engine::Logger* logger = oxide::Engine::getLastInstance()->getLogger();

    std::string vertexCode;
    std::string fragmentCode;
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;

    vShaderFile.exceptions(std::ifstream::badbit);
    fShaderFile.exceptions(std::ifstream::badbit);
    try {
        vShaderFile.open(vertexShaderPath);
        fShaderFile.open(fragmentShaderPath);
        std::stringstream vShaderStream, fShaderStream;

        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();

        vShaderFile.close();
        fShaderFile.close();

        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();
    }
    catch (std::ifstream::failure& e) {
        *logger << "oxide::OpenGL::Renderer::Shader:" << "nulled::OpenGL::Renderer::Shader::FILE_NOT_SUCCESFULLY_READ";
        logger->add_log(oxide::Engine::Logger::log_type::lt_error);
        return nullptr;
    }

    if (vertexCode.empty() || fragmentCode.empty()) {
        *logger << "oxide::OpenGL::Renderer::Shader:" << "nulled::OpenGL::Renderer::Shader::FILE_DONT_READED";
        logger->add_log(oxide::Engine::Logger::log_type::lt_error);
        return nullptr;
    }

    const GLchar* vShaderCode = vertexCode.c_str();
    const GLchar* fShaderCode = fragmentCode.c_str();

    GLuint vertex, fragment;
    GLint success;
    GLchar infolog[512];

    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, nullptr);
    glCompileShader(vertex);
    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertex, 512, nullptr, infolog);
        *logger << "oxide::OpenGL::Renderer::Shader::VERTEX: compilation failed:\n" << infolog;
        logger->add_log(oxide::Engine::Logger::log_type::lt_error);
        return nullptr;
    }

    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, nullptr);
    glCompileShader(fragment);
    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragment, 512, nullptr, infolog);
        *logger << "oxide::OpenGL::Renderer::Shader::PROGRAM: Linking failed:\n" << infolog;
        logger->add_log(oxide::Engine::Logger::log_type::lt_error);
        return nullptr;
    }

    GLuint id = glCreateProgram();
    glAttachShader(id, vertex);
    glAttachShader(id, fragment);
    glLinkProgram(id);

    glGetProgramiv(id, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(id, 512, nullptr, infolog);
        glDeleteShader(vertex);
        glDeleteShader(fragment);
        *logger << "oxide::OpenGL::Renderer::Shader::PROGRAM: Linking failed:\n" << infolog;
        logger->add_log(oxide::Engine::Logger::log_type::lt_error);
        return nullptr;
    }

    glDeleteShader(vertex);
    glDeleteShader(fragment);

    return new oxide::GL::Renderer::Shader(id);
}

oxide::GL::Renderer::Texture::Texture(const GLuint id, const int width, const int height) : id(id), width(width), height(height){
}

void oxide::GL::Renderer::Texture::bind(){
    glBindTexture(GL_TEXTURE_2D, id);
}

int oxide::GL::Renderer::Texture::get_width(){
    return this->width;
}

int oxide::GL::Renderer::Texture::get_height(){
    return this->height;
}

void oxide::GL::Renderer::Texture::add_Subtexture(std::string name, const glm::vec2& leftBottomUV, const glm::vec2& rightTopUV){
    m_subTextures.emplace(std::move(name), subTexture(leftBottomUV, rightTopUV));
}

const oxide::GL::Renderer::Texture::subTexture& oxide::GL::Renderer::Texture::getSubTexture(const std::string& name){
    return m_subTextures[name];
}

oxide::GL::Renderer::Texture* oxide::GL::Renderer::Texture::loadTexture(const std::string& path){
    int width, height, nrChannels;
    unsigned char* data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);

    if (!data) {
        oxide::Engine::Logger* logger = oxide::Engine::getLastInstance()->getLogger();
        *logger << "Cannot load texture: \"" << path << "\" \n";
        logger->add_log(oxide::Engine::Logger::log_type::lt_error);
        return nullptr;
    }

    GLuint texture;
    glGenTextures(1, &texture);

    glBindTexture(GL_TEXTURE_2D, texture);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, 0);

    stbi_image_free(data);

    return new oxide::GL::Renderer::Texture(texture, width, height);
}

void oxide::GL::Renderer::Camera::updateAttributes(){
    glm::vec3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    this->front = glm::normalize(front);

    right = glm::normalize(glm::cross(front, worldUp));
    up    = glm::normalize(glm::cross(right, front));
}

oxide::GL::Renderer::Camera::Camera
(glm::vec3 position,
 glm::vec3 up,
 float yaw, float pitch
) : speed(2.5f), zoom(45.0f), front(glm::vec3(0.0f, 0.0f, -1.0f)){
    this->position = position;
    this->worldUp = up;
    this->yaw = yaw;
    this->pitch = pitch;

    this->updateAttributes();
}

glm::mat4 oxide::GL::Renderer::Camera::getViewMatrix(){
    return glm::lookAt(position, position + front, up);
}

glm::mat4 oxide::GL::Renderer::Camera::getProjection(){
    return glm::perspective(glm::radians(zoom), (float)oxide::Window::get_WindowWidth() / (float)oxide::Window::get_WindowHeight(), 0.1f, 100.0f);
}

oxide::GL::Renderer::CameraTransform::CameraTransform(Camera* camera, float mouse_sensitivity) 
    : camera(camera), mouse_sensitivity(mouse_sensitivity)
{}

void oxide::GL::Renderer::CameraTransform::mouse_transform(GLboolean constrainPitch){
    float x_offset = oxide::Events::deltaX;
    float y_offset = -oxide::Events::deltaY;

    x_offset *= this->mouse_sensitivity;
    y_offset *= this->mouse_sensitivity;

    camera->yaw += x_offset;
    camera->pitch += y_offset;

    if (constrainPitch){
        if (camera->pitch > 89.0f)
            camera->pitch = 89.0f;
        if (camera->pitch < -89.0f)
            camera->pitch = -89.0f;
    }
    
    this->camera->updateAttributes();
}

void oxide::GL::Renderer::CameraTransform::keyboard_transform(CameraMovement movement){
    float velocity = camera->speed * oxide::Events::deltaTime;

    switch (movement)
    {
        case oxide::GL::Renderer::CameraTransform::CM_FORWARD:
            camera->position += camera->front * velocity;
            break;
        case oxide::GL::Renderer::CameraTransform::CM_BACKWARD:
            camera->position -= camera->front * velocity;
            break;
        case oxide::GL::Renderer::CameraTransform::CM_LEFT:
            camera->position -= camera->right * velocity;
            break;
        case oxide::GL::Renderer::CameraTransform::CM_RIGHT:
            camera->position += camera->right * velocity;
            break;
    }
}
