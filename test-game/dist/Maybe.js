export class Maybe {
    static EmptyException = class extends Error {
    };
    static EMPTY = Symbol("EMPTY");
    value_;
    constructor(value = Maybe.EMPTY) {
        this.value_ = value;
    }
    get(defaultValue, onEmptyMsg) {
        if (this.isEmpty()) {
            if (defaultValue) {
                this.value_ = defaultValue;
            }
            else {
                throw new Maybe.EmptyException(onEmptyMsg || `${this.constructor.name}.value_ = null!`);
            }
        }
        return this.value_;
    }
    isEmpty() {
        return this.value_ == Maybe.EMPTY;
    }
    reset(value = Maybe.EMPTY) {
        this.value_ = value;
        if (!this.isEmpty())
            return this.value_;
    }
}
