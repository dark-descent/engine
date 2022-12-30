import { Scene } from "engine";
export class TestScene2 extends Scene {
    onLoad() {
        console.log(`Scene ${this.name} loading`);
        this.spawn();
        this.spawn();
        this.spawn();
        this.spawn();
        this.spawn();
    }
    onUnload() {
        console.log(`Scene ${this.name} unloading`);
    }
}
