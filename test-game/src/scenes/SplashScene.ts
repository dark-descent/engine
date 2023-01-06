import { Scene } from "engine";

export class SplashScene extends Scene
{
	public onLoad()
	{
		console.log(`Scene ${this.name} loading`);

		this.spawn();
		this.spawn();
		this.spawn();
		this.spawn();
		this.spawn();
	}

	public onUnload()
	{
		console.log(`Scene ${this.name} unloading`);
	}
}