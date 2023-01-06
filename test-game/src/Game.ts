import { Game, IGameConfig, SceneManager, WindowState } from "engine";
import { TestScene } from "./scenes/TestScene";
import { SplashScene } from "./scenes/SplashScene";

export class TestGame extends Game<[readonly string[]]>
{
	public onInitialize(args: readonly string[]): IGameConfig
	{
		console.log("Initializing game with args", args);

		return {
			window: {
				title: "test",
				initialState: WindowState.MAXIMIZED,
				initialHidden: true
			}
		};
	}

	public onLoad()
	{
		console.log("Game loading...");

		// Register all scenes
		SceneManager.registerScene("splash_scene", SplashScene);
		SceneManager.registerScene("test", TestScene);

		// load the start scene 
		SceneManager.loadScene("splash_scene");
		
		this.window.show();
	}

	public onTerminate(): void
	{

	}
}