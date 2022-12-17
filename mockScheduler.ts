// Round-robin, FIFO, SJF
import Linked

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
    #executions: OngoingExecution[];

    constructor(initialQueue: Execution[]) {
        this.#executions = [];
        for (let execution of executions) {

        }
    }

    scheduleExecution(newExecution: Execution) {
        const ongoingExecution: OngoingExecution = {
            ...newExecution,
            remainingTime: newExecution.runTime,
        }
    }

    runExecutions() {
        let currentTime = 0;
    }
}

const programs = createPrograms(20);
const executions = createRandomExecutions(programs);

console.log(executions)