/// <reference path="./Console.d.ts" />
/// <reference path="./Engine.d.ts" />

declare interface IEntry
{
	(args: ReadonlyArray<string>): any;
}

declare const __dirname: string;
declare const __filename: string;