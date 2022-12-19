// Round-robin, FIFO, SJF
import {LinkedQueue} from "./LinkedQueue";

interface Program {
    runTime: number;
    identifier: number;
}

interface Execution extends Program {
    startTime: number;
}

interface OngoingExecution extends Execution {
    endTime?: number;
    remainingTime: number;
}

function createProgram(): Program {
    return {
        runTime: 2.5 + Math.random() * 5,
        identifier: Math.floor(Math.random() * 10),
    }
}

function createPrograms(progCount: number): Program[] {
    return Array.from({length: progCount}, () => createProgram());
}

function createRandomExecutions(programs: Program[]): Execution[] {
    return programs.map(p => ({
        ...p,
        startTime: Math.random() * 10
    }))
}




class FIFOScheduler {
    #executions: LinkedQueue<OngoingExecution>;

    constructor(initialQueue: Execution[]) {
        this.#executions = new LinkedQueue();
        this.#executions.pushAlgorithm = (execution1, execution2) => {
            return execution1.startTime < execution2.startTime;
        };
        for (let execution of executions) {
            this.scheduleExecution(execution);
        }
    }

    scheduleExecution(newExecution: Execution) {
        const ongoingExecution: OngoingExecution = {
            ...newExecution,
            remainingTime: newExecution.runTime,
        }
        this.#executions.add(ongoingExecution);
    }

    display() {
        this.#executions.display();
    }

    runExecutions() {
        let currentTime = 0;
        this.#executions.display();
    }
}

const programs = createPrograms(20);
const executions = createRandomExecutions(programs);
const scheduler = new FIFOScheduler(executions);
scheduler.runExecutions();