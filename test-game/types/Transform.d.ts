declare module "engine"
{
	export class Transform extends Component
	{
		public get position(): Vector2;
		public set position(value: Vector2);

		public get rotation(): Vector2;
		public set rotation(value: Vector2);

		public get scale(): Vector2;
		public set scale(value: Vector2);
	}
}