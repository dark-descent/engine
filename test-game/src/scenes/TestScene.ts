import { Scene, SceneManager } from "engine";

export class TestScene extends Scene
{
	public onLoad()
	{
		console.log(`Scene ${this.name} loading`);
		this.spawn();
		this.spawn();
		this.spawn();
		this.spawn();
		this.spawn();

		try
		{
			SceneManager.loadScene("test2");
		}
		catch(e)
		{
			console.warn(e);
		}
	}

	public onUnload()
	{
		console.log(`Scene ${this.name} unloading`);
	}
}