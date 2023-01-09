import { Game, Renderer, SceneManager } from "engine";
import { TestScene } from "./scenes/TestScene";
import { SplashScene } from "./scenes/SplashScene";
export class TestGame extends Game {
    onInitialize(args) {
        console.log("Initializing game with args", args);
        return {
            window: {
                title: "test",
                initialState: 1 /* WindowState.MAXIMIZED */,
                initialHidden: true
            }
        };
    }
    onLoad() {
        console.log("Game loading...");
        Renderer.registerShader("test", "resources/shaders/default.vertex.glsl", "resources/shaders/default.fragment.glsl");
        // Register all scenes
        SceneManager.registerScene("splash_scene", SplashScene);
        SceneManager.registerScene("test", TestScene);
        // load the start scene 
        SceneManager.loadScene("splash_scene");
        this.window.show();
    }
    onTerminate() {
    }
}
