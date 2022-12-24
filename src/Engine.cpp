#include "Engine.hpp"
#include "TraceException.hpp"
#include "Logger.hpp"
#include "js/Helpers.hpp"
#include "Utils.hpp"
#include "js/Format.hpp"
#include "TransformComponent.hpp"
#include "ScriptManager.hpp"
#include "WindowManager.hpp"
#include "ArchManager.hpp"
#include "GameObjectManager.hpp"
#include "Component.hpp"
#include "RenderSystem.hpp"

namespace DarkDescent
{
	std::optional<Engine*> Engine::instance_;

	Engine& Engine::initialize(const char* gamePath_)
	{
		if (instance_.has_value())
			throw TraceException("Engine is already initialized!");

		std::string gamePath;
		std::filesystem::path gameJsonPath;

		if (gamePath_ == nullptr)
		{
			gamePath = "game.json";
		}
		else
		{
			gamePath = gamePath_;
		}

		if (gameJsonPath.is_relative())
		{
			std::vector<std::filesystem::path> checkPaths = {
				std::filesystem::current_path(),
				Utils::getExecutablePath() / ".."
			};

			for (auto& path : checkPaths)
			{
				gameJsonPath = path / gamePath;
				if (std::filesystem::exists(gameJsonPath))
					break;
			}
		}

		if (!std::filesystem::exists(gameJsonPath))
			throw TraceException("Could not load the game!");

		Config config;

		ScriptManager::initializeV8();

		ScriptManager::execStandAlone([ & ](const JS::Env& env)
		{
			auto json = env.readJsonFile(gameJsonPath).ToLocalChecked();

		auto read = [ & ](const char* key, v8::Local<v8::Value> obj = v8::Local<v8::Value>())
		{
			if (obj.IsEmpty())
				obj = json;
			return JS::getFromObject(env, obj, key);
		};

		Logger::get().debug("Game json: ", JS::Format::parse(env, json));

		config.name = JS::parseString(env, read("name").ToLocalChecked());
		});

		instance_.emplace(new Engine(std::move(config)));
		return *(instance_.value());
	}

	Engine& Engine::getInstance()
	{
		if (!instance_.has_value())
			throw TraceException("Engine is not initialized yet!");

		return *(instance_.value());
	}

	bool Engine::terminate()
	{
		if (!instance_.has_value())
			throw TraceException("Engine is not initialized!");


		Engine* engine = instance_.value();
		delete engine;
		instance_.reset();

		ScriptManager::terminateV8();

		return true;
	}

	Engine::Engine(Config&& config):
		logger(Logger::get()),
		config(config),
		mainThreadID(std::this_thread::get_id()),
		eventManager(),
		subSystems_(),
		initializationOrder_()
	{
		const Logger& logger = Logger::get();
		logger.info("Initializing engine...");

		initializeSubSystem<ArchManager>();
		initializeSubSystem<GameObjectManager>();
		initializeSubSystem<ScriptManager>();
		initializeSubSystem<WindowManager>();
		initializeSubSystem<RenderSystem>();

		logger.info("Engine initialized!");
	}

	Engine::~Engine()
	{
		const Logger& logger = Logger::get();
		logger.info("Terminating engine...");

		for (int i = static_cast<int>(initializationOrder_.size()) - 1; i >= 0; i--)
		{
			initializationOrder_[i]->terminate();
		}

		initializationOrder_.clear();

		logger.info("Engine terminated!");
	}

	struct A: public Component<A>
	{
		A(std::size_t a = 0): Component(), a(a) { }
		std::size_t a;

		std::string log()
		{
			return std::format("struct A\n  a = {}", a);
		}
	};

	struct B: public Component<B>
	{
		B(std::size_t x = 0, std::size_t y = 0): Component(), x(x), y(y) { }
		std::size_t x;
		std::size_t y;

		std::string log()
		{
			return std::format("struct B\n  x = {}\n  y = {}", x, y);
		}
	};

	void Engine::run()
	{
		WindowManager* wm = getSubSystem<WindowManager>();

		wm->createWindow(config.name);
		wm->enterEventLoop();
	}
}