declare module "engine"
{
	export namespace Renderer
	{
		export const registerShader: (name: string, vertexPath: string, fragmentPath: string) => void;
		export const createMaterial: <T extends MaterialData>(shaderName: string, data?: Partial<T>) => Material<T>;
		export const setDefaultMaterial: (material: Material<any>) => void;
	}
}
