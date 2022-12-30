import { Game, SceneManager, Window } from "engine";
import { Maybe } from "./Maybe";
import { TestScene } from "./scenes/TestScene";
import { TestScene2 } from "./scenes/TestScene2";
import { TestScene3 } from "./scenes/TestScene3";
export class TestGame extends Game {
    window_ = new Maybe();
    onInitialize(args) {
        console.log("Initializing game with args", args);
    }
    onLoad() {
        console.log("Game loading...");
        // Register all scenes
        SceneManager.registerScene("test", TestScene);
        SceneManager.registerScene("test2", TestScene2);
        SceneManager.registerScene("test3", TestScene3);
        // ...
        // ...
        // load the start scene 
        SceneManager.loadScene("test");
        // setup a window and show it
        const window = this.window_.reset(new Window());
        window.show();
    }
    onTerminate() {
    }
}
