import { Game, SceneManager, Window } from "engine";
import { Maybe } from "./Maybe";
import { TestScene } from "./scenes/TestScene";

export class TestGame extends Game<[readonly string[]]>
{
	private readonly window_: Maybe<Window> = new Maybe();

	public onInitialize(args: readonly string[])
	{
		console.log("Initializing game with args", args);
	}

	public onLoad()
	{
		console.log("Game loading...");

		// Register all scenes
		SceneManager.registerScene<TestScene>("test", TestScene);
		// ...
		// ...


		// load the start scene 
		SceneManager.loadScene("test");
		
		// setup a window and show it
		const window = this.window_.reset(new Window());
		window.show();
	}

	public onTerminate(): void
	{

	}
}