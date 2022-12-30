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
#include "ResourceManager.hpp"
#include "SceneManager.hpp"

namespace DarkDescent
{
	std::optional<Engine*> Engine::instance_;

	Engine& Engine::initialize(std::vector<const char*>&& args)
	{
		if (instance_.has_value())
			throw TraceException("Engine is already initialized!");

		std::filesystem::path gamePath;
		std::filesystem::path gameJsonPath;

		const std::size_t count = args.size();
		std::vector<const char*> gameArgs;

		if (count <= 1 || std::string(args.at(1)).starts_with("-"))
		{
			gamePath = "game.json";
			gameArgs = std::vector<const char*>(args.begin() + 1, args.end());
		}
		else
		{
			gamePath = args.at(1);
			gameArgs = std::vector<const char*>(args.begin() + 2, args.end());
		}

		if (gamePath.is_relative())
		{
			std::vector<std::filesystem::path> checkPaths = {
				std::filesystem::current_path(),
				Utils::getExecutableDir()
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

		std::filesystem::path p = (gameJsonPath / "..").lexically_normal();

		Engine* engine = new Engine(std::move(p), std::move(gameArgs));

		if (!instance_.has_value())
			instance_.emplace(engine);

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

		return true;
	}

	Engine::Engine(std::filesystem::path&& gamePath, std::vector<const char*>&& gameArgs):
		logger(Logger::get()),
		config_(),
		gamePath(gamePath),
		mainThreadID(std::this_thread::get_id()),
		eventManager(),
		gameArgs(gameArgs),
		subSystems_(),
		initializationOrder_()
	{
		instance_.emplace(this);

		const Logger& logger = Logger::get();
		logger.info("Initializing engine...");

		initializeSubSystem<ResourceManager>();
		ScriptManager& sm = initializeSubSystem<ScriptManager>();
		initializeSubSystem<ArchManager>();
		initializeSubSystem<GameObjectManager>();
		initializeSubSystem<WindowManager>();
		initializeSubSystem<RenderSystem>();
		initializeSubSystem<SceneManager>();

		sm.addEventHandler(ScriptManager::Events::ENV_CREATED, EVENT_HANDLER()
		{
			const JS::Env& env = *static_cast<const JS::Env*>(event.data);
			env.registerClass<JS::GameClass>();
		});

		for (const auto& [_, system] : subSystems_)
			system->allInitialized();

		sm.mainEnv().run([ & ](const JS::Env& env)
		{
			auto jsonPath = gamePath / "game.json";
			auto json = env.readJsonFile(jsonPath).ToLocalChecked();

			auto read = [ & ](const char* key, v8::Local<v8::Value> obj = v8::Local<v8::Value>())
			{
				if (obj.IsEmpty())
					obj = json;
				return JS::getFromObject(env, obj, key);
			};

			Logger::get().debug("Game json: ", JS::Format::parse(env, json));

			config_.name = JS::parseString(env, read("name").ToLocalChecked());
			config_.entry = JS::parseString(env, read("entry").ToLocalChecked());
		});

		sm.initializeGame();

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

	void Engine::initializeGame(const JS::Env& env, v8::Local<v8::Object> gameObject)
	{
		assert(!game_.has_value());
		JS::Game& game = game_.emplace(env);
		game.reset(gameObject);
	}

	void Engine::run()
	{
		if (game_.has_value())
		{
			getSubSystem<ScriptManager>()->mainEnv().run([&](const JS::Env& env){
				game_.value().onLoad();
			});
			getSubSystem<WindowManager>()->enterEventLoop();
		}
	}
}