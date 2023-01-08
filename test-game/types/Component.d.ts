declare module "engine"
{
	export abstract class Component
	{
		public constructor(gameObject: GameObject);


	}

	type ComponentType<T extends Component> = new (gameObject: GameObject) => T;
}