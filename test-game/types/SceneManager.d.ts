declare namespace Engine
{
	namespace SceneManager
	{
		const registerScene: <T extends Scene>(name: string, type: T) => T;
		const loadScene: <T extends Scene>(name: string, type: T) => T;
		const activeScene: <T extends Scene = Scene>() => T | null;
	}
}