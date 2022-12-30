declare module "engine"
{
	export abstract class Game<Args extends any[] = []>
	{
		public abstract onInitialize(...args: Args): any;
		public abstract onLoad(): any;
		public abstract onTerminate(): void;

		public start(): void;
	}

	type InitializeGameMethod<Args extends any[]> = (...args: Args) => any;

	type InferInitializeGameArgs<T extends Game<any>> = T extends Game<infer Args> ? Args : [];
}