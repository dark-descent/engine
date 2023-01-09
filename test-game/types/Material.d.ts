declare module "engine"
{
	type MaterialData = {};

	class Material<T extends MaterialData>
	{
		public static readonly clone: <T extends MaterialData>(material: Material<T>, config: Partial<T>) => Material<T>;

		private constructor();

		public readonly set: <K extends keyof T>(key: K, value: T[K]) => void;
		public configure: (config: Partial<T>) => void;
	}
}