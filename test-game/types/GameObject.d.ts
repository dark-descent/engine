declare module "engine"
{
	export class GameObject
	{
		public constructor(name?: string);

		public get transform(): Transform;

		public addComponent<T extends Component>(type: ComponentType<T>): T;
		public getComponent<T extends Component>(type: ComponentType<T>): T;
		public removeComponent<T extends Component>(type: ComponentType<T>): T;
	}
}