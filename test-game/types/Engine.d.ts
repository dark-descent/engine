declare namespace Engine
{
	interface IConfig
	{

	}

	abstract class Game
	{
		protected abstract initialize(config: IConfig);
		protected abstract onLoad();
		protected abstract onTerminate();

		public start(): void;
	}

	const initialize: <T extends Game>(type: new (...args: any[]) => T) => Promise<T>;
}