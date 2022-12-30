declare module "engine"
{
	namespace SceneManager
	{
		const registerScene: <T extends Scene>(name: string, type: SceneClass<T>) => T;
		const loadScene: <T extends Scene>(name: string) => T;
		const activeScene: <T extends Scene = Scene>() => T | null;
	}
}