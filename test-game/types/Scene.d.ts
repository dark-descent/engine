declare module "engine"
{
	export abstract class Scene
	{
		public readonly name: string;

		public constructor(name: string);

		public abstract onLoad(): any;
		public abstract onUnload(): any;

		public readonly spawn: () => any;
	}

	export type SceneClass<T extends Scene> = new (name: string) => T;
}