import { Scene } from "engine";
export class TestScene extends Scene {
    onLoad() {
        console.log(this);
        this.spawn();
        this.spawn();
        this.spawn();
        this.spawn();
        this.spawn();
    }
    onUnload() {
    }
}
