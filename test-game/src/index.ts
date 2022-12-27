class Game extends Engine.Game
{
	protected initialize(config: Engine.IConfig)
	{

	}

	protected onLoad()
	{
	
	}
	
	protected onTerminate()
	{
	
	}
}

const entry: IEntry = async (args) =>
{
	const game = await Engine.initialize(Game);

	game.start();
};

export default entry;