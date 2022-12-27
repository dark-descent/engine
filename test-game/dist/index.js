class Game extends Engine.Game {
    initialize(config) {
    }
    onLoad() {
    }
    onTerminate() {
    }
}
const entry = async (args) => {
    const game = await Engine.initialize(Game);
    game.start();
};
export default entry;
