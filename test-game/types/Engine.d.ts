declare module "engine"
{
	namespace Engine
	{
		type GameType<T extends Game> = new (...args: any[]) => T;

		type InitializeArgs<T extends Game<any>, Args = InferInitializeGameArgs<T>> = Args extends any[] ? [GameType<T>, ...Args] : [GameType<T>];

		export const initialize: <T extends Game<any>>(...args: InitializeArgs<T>) => T;
	}

	export default Engine;
}