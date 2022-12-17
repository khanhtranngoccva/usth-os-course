// Round-robin, FIFO, SJF

// FIFO
interface Program {
    runtime: number;
    identifier: number;
}

function createProgram(): Program {
    return {
        runtime: 2.5 + Math.random() * 5,
        identifier: Math.floor(Math.random() * 10);
    }
}

function createPrograms(progCount: number): Program[] {
    return Array.from({length: progCount}, () => createProgram());
}