#include "Engine.hpp"
#include "TraceException.hpp"
#include "Logger.hpp"
#include "js/Helpers.hpp"
#include "Utils.hpp"
#include "js/Format.hpp"
#include "Arch.hpp"
#include "GameObject.hpp"
#include "TransformComponent.hpp"

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
		componentManager_(),
		archManager_(),
		gameObjectManager_(componentManager_, archManager_),
		scriptManager_(*this),
		windowManager_(*this),
		initializedSystems_()
	{
		const Logger& logger = Logger::get();
		logger.info("Initializing engine...");

		initializeSubSystem(scriptManager_);
		initializeSubSystem(windowManager_);

		logger.info("Engine initialized!");
	}

	Engine::~Engine()
	{
		const Logger& logger = Logger::get();
		logger.info("Terminating engine...");

		for (int i = static_cast<int>(initializedSystems_.size()) - 1; i >= 0; i--)
		{
			initializedSystems_[i]->terminate();
		}

		initializedSystems_.clear();

		logger.info("Engine terminated!");
	}

	void Engine::run()
	{
		windowManager_.createWindow(config.name);

		struct TestA
		{
			float x;
			float y;
			float z;
		};

		struct TestB
		{
			char str[10];
		};

		const ComponentInfo& transform = componentManager_.registerComponent<Transform>();
		const ComponentInfo& testA = componentManager_.registerComponent<TestA>();
		const ComponentInfo& testB = componentManager_.registerComponent<TestB>();

		auto& a = gameObjectManager_.create();
		auto& b = gameObjectManager_.create();
		auto& c = gameObjectManager_.create();

		auto& t = a.addComponent<Transform>(transform);
		t.x = 1.0f;
		t.y = 2.0f;

		TestA& ta = a.addComponent<TestA>(testA);
		ta.x = 44.0f;
		ta.y = 45.0f;
		ta.z = 46.0f;

		auto& tb = a.addComponent<TestB>(testB);

		t = a.getComponent<Transform>(transform);
		ta = a.getComponent<TestA>(testA);

		

		windowManager_.enterEventLoop();
	}
}