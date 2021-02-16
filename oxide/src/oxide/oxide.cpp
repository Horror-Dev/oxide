#include "oxide.h"

#include "..//Renderer/Renderer.h"
#include <iostream>
#include <fstream>
#include <nlohmann/json.hpp>

oxide::Engine* oxide::Engine::Instance;

oxide::Engine::Engine(){
	logger = new oxide::Engine::Logger();

	Instance = this;
}

oxide::Engine::~Engine()
{
}

oxide::Engine::Logger* oxide::Engine::getLogger(){
	return logger;
}

oxide::Engine* oxide::Engine::getLastInstance(){
	return Instance;
}

void oxide::Engine::Logger::add_log_to_file(const char* log){
	std::ofstream writer;
	writer.open(file_name.c_str(), std::ios::app);
	writer << log;
	writer.close();
}

oxide::Engine::Logger::Logger() :have_file_name(false) {
}

const char* log_type_to_char(oxide::Engine::Logger::log_type lt) {
	switch (lt)
	{
		case oxide::Engine::Logger::log_type::lt_error:
			return "[oxide][error]";
			break;
		case oxide::Engine::Logger::log_type::lt_warning:
			return "[oxide][warning]";
			break;
		case oxide::Engine::Logger::log_type::lt_info:
			return "[oxide][info]";
			break;
		case oxide::Engine::Logger::log_type::lt_success:
			return "[oxide][success]";
			break;

	}
}

void oxide::Engine::Logger::add_log(log_type lt){
	std::string final_log = log_type_to_char(lt);
	final_log += buffer.str();

	std::cout << final_log;

	if (have_file_name) {
		add_log_to_file(final_log.c_str());
	}

	buffer.str("");
}

void oxide::Engine::Logger::setLogOutPutFile(const char* file_name){
	std::ifstream check_file;

	check_file.open(file_name);

	if (check_file.is_open()) {
		this->file_name = file_name;
		this->have_file_name = true;
		check_file.close();
	}
	else {
		check_file.close();
		this->file_name = file_name;
		this->have_file_name = true;
		std::ofstream create;
		create.open(file_name);
		create << "";
		create.close();
	}
}

oxide::Engine::ResourceManager::ResourceManager(){
	
}

bool oxide::Engine::ResourceManager::loadResources(const std::string& path) {
	nlohmann::json resources_document;

	std::ifstream m_ifstream(path);

	if (!m_ifstream.is_open()) {
		oxide::Engine::Logger* logger = oxide::Engine::getLastInstance()->getLogger();

		*logger << "cannot open file:" << path << "\n";
		logger->add_log(oxide::Engine::Logger::log_type::lt_error);
		return false;
	}

	try {
		m_ifstream >> resources_document;
	}
	catch (const nlohmann::json::parse_error& ex) {
		oxide::Engine::Logger* logger = oxide::Engine::getLastInstance()->getLogger();
		*logger << "parser error:\n" << path << "\n" << ex.what() << "\n";
		logger->add_log(oxide::Engine::Logger::log_type::lt_error);
	}

	m_ifstream.close();

	std::string resources_directory;
	
	if (!resources_document["oxide"]["resources_directory"].is_null()) {
		resources_directory += resources_document["oxide"]["resources_directory"].get<std::string>();
	}
	else {
		resources_directory = "";
	}

	nlohmann::json shaders = resources_document["oxide"]["Shaders"];

	if (!shaders.is_null()) {
		if (!resources_directory.empty()) {
			for (size_t i = 0; i < shaders.size(); i++) {
				std::string finalVertexShaderPath = resources_directory;
				finalVertexShaderPath += shaders[i]["vertex_shader_path"];
			
				std::string finalFragmentShaderPath = resources_directory;
				finalFragmentShaderPath += shaders[i]["fragment_shader_path"];

				if (!this->loadShader(shaders[i]["name"], finalVertexShaderPath, finalFragmentShaderPath))
					return false;

				finalVertexShaderPath.clear();
				finalFragmentShaderPath.clear();
			}
		}
		else {
			for (size_t i = 0; i < shaders.size(); i++) {
				if (!loadShader(shaders[i]["name"], shaders[i]["vertex_shader_file"], shaders[i]["fragment_shader_file"]))
					return false;
			}
		}
	}

	nlohmann::json textures = resources_document["oxide"]["Textures"];

	if (!textures.is_null()) {
		if (!resources_directory.empty()) {
			for (size_t i = 0; i < textures.size(); i++){
				std::string finalTexturePath = resources_directory;
				finalTexturePath += textures[i]["path"];

				if (!this->loadTexture(textures[i]["name"], finalTexturePath))
					return false;
			}
		}
		else {
			for (size_t i = 0; i < textures.size(); i++) 
				if (!this->loadTexture(textures[i]["name"], textures[i]["path"]))
					return false;
		}
	}

	return true;
}

bool oxide::Engine::ResourceManager::loadShader(const std::string& name, const std::string& vertex_shader_path, const std::string& fragment_shader_path){
	oxide::GL::Renderer::Shader* p_Shader = oxide::GL::Renderer::Shader::loadShader(vertex_shader_path, fragment_shader_path);

	if (p_Shader == nullptr) {
		oxide::Engine::Logger* logger = oxide::Engine::getLastInstance()->getLogger();
		*logger << "cannot load shader:\n" <<
			"vertex_shader_path:" << vertex_shader_path << "\n" <<
			"fragment_shader_path:" << fragment_shader_path << "\n";
		
		return false;
	}

	resource* ShaderRes = new resource();
	ShaderRes->ptr = p_Shader;
	ShaderRes->type = rt_shader;

	resources[name] = ShaderRes;

	return true;
}

bool oxide::Engine::ResourceManager::loadTexture(const std::string& name, const std::string& path){
	oxide::GL::Renderer::Texture* m_Texture = oxide::GL::Renderer::Texture::loadTexture(path);
	
	if (m_Texture == nullptr) {
		return false;
	}

	resource* TextureRes = new resource();
	TextureRes->ptr = m_Texture;
	TextureRes->type = rt_texture;

	resources[name] = TextureRes;

	return true;
}

bool oxide::Engine::ResourceManager::loadTextureAtlas(const std::string& name, const std::string& path, const std::vector<std::string>& sub_textures_names,
	uint32_t sub_texture_width, uint32_t sub_texture_height){
	
	if (!loadTexture(name, path))
		return false;

	oxide::GL::Renderer::Texture* p_texture = reinterpret_cast<oxide::GL::Renderer::Texture*>(getResource(name));

	uint32_t t_width = p_texture->get_width();
	uint32_t t_height = p_texture->get_height();

	uint32_t current_texture_offsetX = 0;
	uint32_t current_texture_offsetY = sub_texture_height;

	for (auto& current_subTexture_name : sub_textures_names){
		glm::vec2 leftBottomUV(static_cast<float>(current_texture_offsetX + 0.01f) / t_width, static_cast<float>(current_texture_offsetY - sub_texture_height + 0.01f) / sub_texture_height);
		glm::vec2 rightTopUV(static_cast<float>(current_texture_offsetX + t_width - 0.01f) / t_width, static_cast<float>(current_texture_offsetY - 0.01f) / sub_texture_height);

		p_texture->add_Subtexture(current_subTexture_name, leftBottomUV, rightTopUV);

		current_texture_offsetY += sub_texture_width;
		if (current_texture_offsetY >= t_width){
			current_texture_offsetX = 0;
			current_texture_offsetY -= sub_texture_height;
		}
	}

	return true;
}


void* oxide::Engine::ResourceManager::getResource(const std::string& name){
	oxide::Engine::ResourceManager::resource* res = reinterpret_cast<oxide::Engine::ResourceManager::resource*>(resources[name]);

	return res->ptr;
}

void oxide::Engine::ResourceManager::unloadResources(){
	for (auto it : resources ){
		if(it.second)
		if (it.second->ptr) {

			switch (it.second->type) {
			case oxide::Engine::ResourceManager::resouce_type::rt_shader:
				delete reinterpret_cast<oxide::GL::Renderer::Shader*>(it.second->ptr);
				break;
			case oxide::Engine::ResourceManager::resouce_type::rt_texture:
				delete reinterpret_cast<oxide::GL::Renderer::Texture*>(it.second->ptr);
			}

			delete it.second;
		}
	}
}
