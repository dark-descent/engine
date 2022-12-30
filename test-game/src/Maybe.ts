export class Maybe<T>
{
	public static readonly EmptyException = class extends Error {};

	private static readonly EMPTY = Symbol("EMPTY");
	
	private value_: T | typeof Maybe.EMPTY;

	public constructor();
	public constructor(value: T);
	public constructor(value: T | typeof Maybe.EMPTY = Maybe.EMPTY)
	{
		this.value_ = value;
	}

	public get(defaultValue?: T, onEmptyMsg?: string): T
	{
		if(this.isEmpty())
		{
			if(defaultValue)
			{
				this.value_ = defaultValue;
			}
			else
			{
				throw new Maybe.EmptyException(onEmptyMsg || `${this.constructor.name}.value_ = null!`);
			}
		}
		return this.value_ as T;
	}

	public isEmpty(): boolean
	{
		return this.value_ == Maybe.EMPTY;
	}

	public reset(): void;
	public reset(value: T): T;
	public reset(value: T | typeof Maybe.EMPTY = Maybe.EMPTY): T | void
	{
		this.value_ = value;
		if(!this.isEmpty())
			return this.value_ as T;
	}
}