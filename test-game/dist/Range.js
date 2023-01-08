export const range = (from, to, step = 1) => {
    const data = [];
    for (let i = from; i < to; i += step)
        data.push(i);
    const maxIndex = data.length;
    return {
        [Symbol.iterator]() {
            let index = -1;
            return {
                next: () => ({ value: data[++index], done: index >= maxIndex })
            };
        }
    };
};
