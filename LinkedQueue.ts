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
    pushAlgorithm?: (newItem: T, lastItem: T) => boolean;

    get length(): number {
        return this.#length;
    }
    constructor(data?: T[]) {
        if (data)
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
        // If there's already a node, and there is no algorithm.
        else if (!this.pushAlgorithm) {
            if (!this.#lastNode) {
                throw new Error("Internal error.");
            }
            const lastNode = this.#lastNode;
            lastNode.next = newNode;
            newNode.prev = lastNode;
            this.#lastNode = newNode;
        }
        // There is an algorithm that tells where to push.
        else {
            let curNode = this.#firstNode;
            if (!curNode) {
                throw new Error("Internal error.");
            }
            while (curNode) {
                const curNodeData = curNode.data;
                if (!this.pushAlgorithm(curNodeData, item)) {
                    break;
                }
                curNode = curNode.next;
            }
            if (!curNode) {
                if (!this.#lastNode) {
                    throw new Error("Internal error.");
                }
                const lastNode = this.#lastNode;
                lastNode.next = newNode;
                newNode.prev = lastNode;
                this.#lastNode = newNode;
            } else {
                const prevNode = curNode.prev;
                if (!prevNode) {
                    this.#firstNode = newNode;
                    curNode.prev = newNode;
                    newNode.next = curNode;
                } else {
                    prevNode.next = newNode;
                    curNode.prev = newNode;
                    newNode.prev = prevNode;
                    newNode.next = curNode;
                }
            }
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