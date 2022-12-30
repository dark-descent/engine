import Engine from "engine";
import { TestGame } from "./Game";

const entry: IEntry = async (args) =>
{
	console.log("Game entry");

	Engine.initialize(TestGame, args);
};

export default entry;