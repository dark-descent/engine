export const range = (from: number, to: number, step: number = 1) =>
{
	const data: number[] = [];

	for (let i = from; i < to; i += step)
		data.push(i);

	const maxIndex = data.length;

	return {
		[Symbol.iterator]()
		{
			let index = -1;

			return {
				next: () => ({ value: data[++index], done: index >= maxIndex })
			};
		}
	};
}