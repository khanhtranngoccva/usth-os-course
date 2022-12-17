// We implement a doubly linked queue to remove nodes more easily.
class LinkedNode<T> {
    data: T;
    next?: LinkedNode<T>;
    prev?: LinkedNode<T>;

    constructor(data: T) {
        this.data = data;
    }
}

export class LinkedQueue<T> {
    #length = 0;
    #firstNode?: LinkedNode<T>;
    #lastNode?: LinkedNode<T>;

    get length(): number {
        return this.#length;
    }

    constructor(data: T[]) {
        for (let item of data) {
            this.add(item);
        }
    }

    add(item: T): number {
        const newNode = new LinkedNode(item);
        // No nodes?
        if (this.#length === 0) {
            this.#firstNode = newNode;
            this.#lastNode = newNode;
        }
        // If there's already a node.
        else {
            if (!this.#lastNode) {
                throw new Error("Internal error.");
            }
            const lastNode = this.#lastNode;
            lastNode.next = newNode;
            newNode.prev = lastNode;
            this.#lastNode = newNode;
        }
        this.#length++;
        return this.#length;
    }
    pop(): T|undefined {
        if (this.#length === 0) {
            return undefined;
        }
        this.#length--;
        const firstNode = this.#firstNode;
        if (!firstNode) {
            throw new Error("Internal error.");
        }
        this.#firstNode = firstNode.next;
        if (this.#firstNode) {
            this.#firstNode.prev = undefined;
        }
        if (this.#length === 0) {
            this.#lastNode = undefined;
        }
        return firstNode.data;
    }
    display(reverse = false): void {
        const values: T[] = [];
        if (reverse && this.#lastNode) {
            let curNode: LinkedNode<T>|undefined = this.#lastNode;
            while (curNode) {
                values.push(curNode.data);
                curNode = curNode.prev;
            }
        } else if (!reverse && this.#firstNode) {
            let curNode: LinkedNode<T>|undefined = this.#firstNode;
            while (curNode) {
                values.push(curNode.data);
                curNode = curNode.next;
            }
        }
        console.log(values);

    }
}