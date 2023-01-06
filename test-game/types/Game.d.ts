declare module "engine"
{
	interface IGameConfig 
	{
		window?: Partial<IWindowConfig>;
	}

	export abstract class Game<Args extends any[] = []>
	{
		public get window(): Window;

		public abstract onInitialize(...args: Args): IGameConfig;
		public abstract onLoad(): any;
		public abstract onTerminate(): void;

		public start(): void;

		public setGameWindow<T extends Window>(window: T): T;
	}

	type InitializeGameMethod<Args extends any[]> = (...args: Args) => any;

	type InferInitializeGameArgs<T extends Game<any>> = T extends Game<infer Args> ? Args : [];
}