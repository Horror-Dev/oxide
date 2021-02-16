#ifndef _INC_OXIDE_H
#define _INC_OXIDE_H
#include <sstream>
#include <unordered_map>

namespace oxide {
	class Engine {

	public:
		class Logger {
			bool have_file_name;

			std::stringstream buffer;
			std::string file_name;

			void add_log_to_file(const char* log);

		public:
			Logger();

			template<typename T>
			Logger& operator<<(T data);

			enum log_type {
				lt_error,
				lt_warning,
				lt_info,
				lt_success,
			};

			void add_log(log_type lt);

			void setLogOutPutFile(const char* file_name);

		};

		class ResourceManager {

			enum resouce_type {
				rt_shader,
				rt_texture
			};

			struct resource {
				void* ptr;
				resouce_type type;
			};

			std::unordered_map<std::string, resource*>resources;

		public:

			ResourceManager();

			bool loadResources(const std::string& path);

			bool loadShader(const std::string& name, const std::string& vertex_shader_path, const std::string& fragment_shader_path);

			bool loadTexture(const std::string& name, const std::string& path);

			void* getResource(const std::string& name);

			void unloadResources();

		};

	private:

		Logger* logger;

		static Engine* Instance;

	public:

		Engine();

		~Engine();

		oxide::Engine::Logger* getLogger();

		static Engine* getLastInstance();
	};
	template<typename T>
	inline oxide::Engine::Logger& oxide::Engine::Logger::operator<<(T data) {
		buffer << data;

		return *this;
	}
}
#endif // !_INC_OXIDE_H