use std::env;
use std::fs;
use std::io::{self, Read};

const UBF_FILE_EXTENSION: &str = ".ultimate_brainfuck";
const UBF_VALID_CHARS: &str = "><+-.,[]";

fn has_ubf_extension(filename: &str) -> bool {
    filename.ends_with(UBF_FILE_EXTENSION)
}

fn has_bf_extension(filename: &str) -> bool {
    filename.ends_with(".bf") || filename.ends_with(".brainfuck")
}

fn read_bf_file(filename: &str) -> Vec<char> {
    let data = match fs::read_to_string(filename) {
        Ok(content) => content,
        Err(_) => {
            eprintln!("Error: Could not open file {}", filename);
            return vec![];
        }
    };

    data.chars()
        .filter(|&ch| UBF_VALID_CHARS.contains(ch))
        .collect()
}

fn increment(bfc: char) -> char {
    match bfc {
        '>' => '<',
        '<' => '+',
        '+' => '-',
        '-' => '.',
        '.' => ',',
        ',' => '[',
        '[' => ']',
        ']' => '>',
        _ => ' ',
    }
}

fn decrement(bfc: char) -> char {
    match bfc {
        ']' => '[',
        '[' => ',',
        ',' => '.',
        '.' => '-',
        '-' => '+',
        '+' => '<',
        '<' => '>',
        '>' => ']',
        _ => ' ',
    }
}

// Convert a parsed Brainfuck program to Ultimate Brainfuck.
fn convert_to_ultimate_brainfuck(parsed: &[char]) -> String {
    let mut ultimate = String::new();
    let mut counter = 0;

    for &c in parsed {
        let mut temp = c;
        for _ in 0..counter {
            temp = increment(temp);
        }
        ultimate.push(temp);
        counter += 1;
    }
    ultimate
}

// Convert a parsed Ultimate Brainfuck program to Brainfuck.
fn convert_to_brainfuck(parsed: &[char]) -> String {
    let mut brainfuck = String::new();
    let mut counter = 0;

    for &c in parsed {
        let mut temp = c;
        for _ in 0..counter {
            temp = decrement(temp);
        }
        brainfuck.push(temp);
        counter += 1;
    }
    brainfuck
}

fn interpret(input: &str) {
    const TAPE_SIZE: usize = 30_000;
    let mut tape = [0u8; TAPE_SIZE];
    let mut ptr = 0;

    let input_bytes = input.as_bytes();
    let mut pc = 0;

    while pc < input_bytes.len() {
        match input_bytes[pc] as char {
            '>' => ptr = (ptr + 1) % TAPE_SIZE,
            '<' => ptr = (ptr + TAPE_SIZE - 1) % TAPE_SIZE,
            '+' => tape[ptr] = tape[ptr].wrapping_add(1),
            '-' => tape[ptr] = tape[ptr].wrapping_sub(1),
            '.' => print!("{}", tape[ptr] as char),
            ',' => {
                let mut buffer = [0];
                if io::stdin().read_exact(&mut buffer).is_ok() {
                    tape[ptr] = buffer[0];
                }
            }
            '[' => {
                if tape[ptr] == 0 {
                    let mut loop_level = 1;
                    while loop_level > 0 {
                        pc += 1;
                        if pc >= input_bytes.len() {
                            eprintln!("Error: Mismatched '[' in input");
                            return;
                        }
                        match input_bytes[pc] as char {
                            '[' => loop_level += 1,
                            ']' => loop_level -= 1,
                            _ => {}
                        }
                    }
                }
            }
            ']' => {
                if tape[ptr] != 0 {
                    let mut loop_level = 1;
                    while loop_level > 0 {
                        if pc == 0 {
                            eprintln!("Error: Mismatched ']' in input");
                            return;
                        }
                        pc -= 1;
                        match input_bytes[pc] as char {
                            ']' => loop_level += 1,
                            '[' => loop_level -= 1,
                            _ => {}
                        }
                    }
                }
            }
            _ => {}
        }
        pc += 1;
    }
}

fn main() {
    let args: Vec<String> = env::args().collect();

    if args.len() < 2 {
        eprintln!("Usage: {} <file.bf or file.ultimate_brainfuck>", args[0]);
        return;
    }

    let input = &args[1];

    if has_bf_extension(input) {
        let bf = read_bf_file(input);
        let ultimate_brainfuck = convert_to_ultimate_brainfuck(&bf);
        println!("Brainfuck transpiled to ultimate brainfuck:\n{}", ultimate_brainfuck);
    } else if has_ubf_extension(input) {
        let ubf = read_bf_file(input);
        let bf = convert_to_brainfuck(&ubf);
        println!("Ultimate brainfuck transpiled to brainfuck:\n{}", bf);
        println!("Interpreting transpiled brainfuck:");
        interpret(&bf);
    } else {
        eprintln!("Error: Unsupported file extension");
    }
}
