/// <reference path="./Console.d.ts" />
/// <reference path="./Engine.d.ts" />
/// <reference path="./Window.d.ts" />
/// <reference path="./Game.d.ts" />
/// <reference path="./Scene.d.ts" />
/// <reference path="./SceneManager.d.ts" />
/// <reference path="./GameObject.d.ts" />
/// <reference path="./Component.d.ts" />
/// <reference path="./Transform.d.ts" />
/// <reference path="./Vector.d.ts" />
/// <reference path="./SpriteRenderer.d.ts" />
/// <reference path="./Renderer.d.ts" />
/// <reference path="./Material.d.ts" />

declare interface IEntry
{
	(args: ReadonlyArray<string>): any;
}

declare const __dirname: string;
declare const __filename: string;