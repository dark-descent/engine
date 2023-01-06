declare module "engine"
{
	const enum WindowState
	{
		MINIMIZED = 0,
		MAXIMIZED = 1,
		UNMAXIMIZED = 2
	}

	interface IWindowConfig
	{
		title: string;
		width: number;
		height: number;
		minWidth: number;
		maxWidth: number;
		minHeight: number;
		maxHeight: number;
		resizable: boolean;
		fullscreenable: boolean;
		minimizable: boolean;
		initialState: WindowState;
		initialHidden: boolean;
	}

	interface Window
	{
		readonly show: () => void;
		readonly minimize: () => void;
		readonly maximize: () => void;
		readonly unmaximize: () => void;
	}
}