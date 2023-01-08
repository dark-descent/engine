import { Scene } from "engine";
import { range } from "../Range";

export class SplashScene extends Scene
{
	public onLoad()
	{
		console.log(`Scene ${this.name} loading`);
		
		const objects = [];

		for(const i of range(0, 10))
			objects.push(this.spawn());
	
		console.log(objects);
	}

	public onUnload()
	{
		console.log(`Scene ${this.name} unloading`);
	}
}