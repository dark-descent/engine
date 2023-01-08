import { Scene, GameObject, Transform, SpriteRenderer } from "engine";
import { range } from "../Range";

export class SplashScene extends Scene
{
	public onLoad()
	{
		console.log(`Scene ${this.name} loading`);
		
		const objects = [];

		for(const _ of range(0, 10))
			objects.push(new GameObject());

		console.log(Transform);

		for(const o of objects)
		{
			o.addComponent(SpriteRenderer);
		}
	}

	public onUnload()
	{
		console.log(`Scene ${this.name} unloading`);
	}
}