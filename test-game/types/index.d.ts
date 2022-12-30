/// <reference path="./Console.d.ts" />
/// <reference path="./Engine.d.ts" />
/// <reference path="./Window.d.ts" />
/// <reference path="./Game.d.ts" />
/// <reference path="./Scene.d.ts" />
/// <reference path="./SceneManager.d.ts" />

declare interface IEntry
{
	(args: ReadonlyArray<string>): any;
}

declare const __dirname: string;
declare const __filename: string;