import { Scene, GameObject, Transform, SpriteRenderer, Renderer } from "engine";
import { range } from "../Range";
export class SplashScene extends Scene {
    onLoad() {
        console.log(`Scene ${this.name} loading`);
        const material = Renderer.createMaterial("test");
        Renderer.setDefaultMaterial(material);
        const objects = [];
        for (const _ of range(0, 10))
            objects.push(new GameObject());
        console.log(Transform);
        for (const o of objects) {
            o.addComponent(SpriteRenderer);
        }
    }
    onUnload() {
        console.log(`Scene ${this.name} unloading`);
    }
}
