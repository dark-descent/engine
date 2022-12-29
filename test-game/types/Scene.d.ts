declare namespace Engine
{
	abstract class Scene
	{
		public readonly name: string;

		public constructor(name: string);

		public onLoad(): any;
		public onUnload(): any;
	}
}