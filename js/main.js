const fs = require('fs');

const ubfFileExtension = '.ultimate_brainfuck';
const ubfValidChars = '><+-.,[]';

function hasUbfExtension(filename) {
    return filename.endsWith(ubfFileExtension);
}

function hasBfExtension(filename) {
    return filename.endsWith('.bf') || filename.endsWith('.brainfuck');
}

function readBfFile(filename) {
    try {
        const data = fs.readFileSync(filename, 'utf8');
        return Array.from(data).filter(ch => ubfValidChars.includes(ch));
    } catch (error) {
        console.error(`Error: Could not open file ${filename}`);
        return [];
    }
}

function increment(bfc) {
    switch (bfc) {
        case '>': return '<';
        case '<': return '+';
        case '+': return '-';
        case '-': return '.';
        case '.': return ',';
        case ',': return '[';
        case '[': return ']';
        case ']': return '>';
        default: return ' ';
    }
}

function decrement(bfc) {
    switch (bfc) {
        case ']': return '[';
        case '[': return ',';
        case ',': return '.';
        case '.': return '-';
        case '-': return '+';
        case '+': return '<';
        case '<': return '>';
        case '>': return ']';
        default: return ' ';
    }
}

function convertToUltimateBrainfuck(parsed) {
    let ultimate = '';
    let counter = 0;

    for (const c of parsed) {
        let currentChar = c;
        for (let j = 0; j < counter; j++) {
            currentChar = increment(currentChar);
        }
        ultimate += currentChar;
        counter++;
    }
    return ultimate;
}

function convertToBrainfuck(parsed) {
    let brainfuck = '';
    let counter = 0;

    for (const c of parsed) {
        let currentChar = c;
        for (let j = 0; j < counter; j++) {
            currentChar = decrement(currentChar);
        }
        brainfuck += currentChar;
        counter++;
    }
    return brainfuck;
}

function interpret(input) {
    const tape = new Uint8Array(30000).fill(0); // Initialize tape with 30,000 cells
    let ptr = 0; // Pointer to the current tape cell
    const stack = []; // Stack for loop tracking

    let i = 0;
    while (i < input.length) {
        const currentChar = input[i];

        switch (currentChar) {
            case '>':
                ptr = (ptr + 1) % tape.length;
                break;
            case '<':
                ptr = (ptr - 1 + tape.length) % tape.length;
                break;
            case '+':
                tape[ptr] = (tape[ptr] + 1) % 256;
                break;
            case '-':
                tape[ptr] = (tape[ptr] - 1 + 256) % 256;
                break;
            case '.':
                process.stdout.write(String.fromCharCode(tape[ptr]));
                break;
            case ',':
                const inputChar = fs.readSync(0, Buffer.alloc(1), 0, 1, null);
                tape[ptr] = inputChar[0] || 0;
                break;
            case '[':
                if (tape[ptr] === 0) {
                    let loop = 1;
                    while (loop > 0) {
                        i++;
                        if (input[i] === '[') loop++;
                        else if (input[i] === ']') loop--;
                    }
                } else {
                    stack.push(i);
                }
                break;
            case ']':
                if (tape[ptr] !== 0) {
                    i = stack[stack.length - 1];
                } else {
                    stack.pop();
                }
                break;
        }
        i++;
    }
}

function main() {
    const args = process.argv.slice(2);

    if (args.length < 1) {
        console.error('Usage: node script.js <file.bf or file.ultimate-brainfuck>');
        process.exit(1);
    }

    const input = args[0];

    if (hasBfExtension(input)) {
        const bf = readBfFile(input);
        const ultimateBrainfuck = convertToUltimateBrainfuck(bf);
        console.log('Brainfuck transpiled to ultimate brainfuck:');
        console.log(ultimateBrainfuck);
    } else if (hasUbfExtension(input)) {
        const ubf = readBfFile(input);
        const brainfuck = convertToBrainfuck(ubf);
        console.log('Ultimate brainfuck transpiled to brainfuck:');
        console.log(brainfuck);
        console.log('Interpreting transpiled brainfuck:');
        interpret(brainfuck);
    } else {
        console.error('Error: Unsupported file extension.');
    }
}

main();
