import { Scene } from "engine";

export class TestScene extends Scene
{
	public onLoad()
	{
		console.log(this);
		this.spawn();
		this.spawn();
		this.spawn();
		this.spawn();
		this.spawn();
	}

	public onUnload()
	{
		
	}
}