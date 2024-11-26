import sys

UBF_FILE_EXTENSION = ".ultimate_brainfuck"
UBF_VALID_CHARS = "><+-.,[]"

def has_ubf_extension(filename):
    return filename.endswith(UBF_FILE_EXTENSION)

def has_bf_extension(filename):
    return filename.endswith(".bf") or filename.endswith(".brainfuck")

def read_bf_file(filename):
    try:
        with open(filename, "r") as file:
            data = file.read()
        return [ch for ch in data if ch in UBF_VALID_CHARS]
    except FileNotFoundError:
        print(f"Error: Could not open file {filename}")
        return []

def increment(bfc):
    return {
        '>': '<',
        '<': '+',
        '+': '-',
        '-': '.',
        '.': ',',
        ',': '[',
        '[': ']',
        ']': '>'
    }.get(bfc, ' ')

def decrement(bfc):
    return {
        ']': '[',
        '[': ',',
        ',': '.',
        '.': '-',
        '-': '+',
        '+': '<',
        '<': '>',
        '>': ']'
    }.get(bfc, ' ')

def convert_to_ultimate_brainfuck(parsed):
    ultimate = ""
    counter = 0

    for c in parsed:
        current_char = c
        for _ in range(counter):
            current_char = increment(current_char)
        ultimate += current_char
        counter += 1
    return ultimate

def convert_to_brainfuck(parsed):
    brainfuck = ""
    counter = 0

    for c in parsed:
        current_char = c
        for _ in range(counter):
            current_char = decrement(current_char)
        brainfuck += current_char
        counter += 1
    return brainfuck

def interpret(input_code):
    tape = [0] * 30000
    ptr = 0
    stack = []
    i = 0

    while i < len(input_code):
        current_char = input_code[i]

        if current_char == '>':
            ptr = (ptr + 1) % len(tape)
        elif current_char == '<':
            ptr = (ptr - 1 + len(tape)) % len(tape)
        elif current_char == '+':
            tape[ptr] = (tape[ptr] + 1) % 256
        elif current_char == '-':
            tape[ptr] = (tape[ptr] - 1 + 256) % 256
        elif current_char == '.':
            sys.stdout.write(chr(tape[ptr]))
        elif current_char == ',':
            tape[ptr] = ord(sys.stdin.read(1) or '\x00')
        elif current_char == '[':
            if tape[ptr] == 0:
                loop = 1
                while loop > 0:
                    i += 1
                    if input_code[i] == '[':
                        loop += 1
                    elif input_code[i] == ']':
                        loop -= 1
            else:
                stack.append(i)
        elif current_char == ']':
            if tape[ptr] != 0:
                i = stack[-1]
            else:
                stack.pop()

        i += 1

def main():
    if len(sys.argv) < 2:
        print("Usage: python script.py <file.bf or file.ultimate_brainfuck>")
        sys.exit(1)

    input_file = sys.argv[1]

    if has_bf_extension(input_file):
        bf = read_bf_file(input_file)
        ultimate_brainfuck = convert_to_ultimate_brainfuck(bf)
        print("Brainfuck transpiled to ultimate brainfuck:")
        print(ultimate_brainfuck)
    elif has_ubf_extension(input_file):
        ubf = read_bf_file(input_file)
        brainfuck = convert_to_brainfuck(ubf)
        print("Ultimate brainfuck transpiled to brainfuck:")
        print(brainfuck)
        print("Interpreting transpiled brainfuck:")
        interpret(brainfuck)
    else:
        print("Error: Unsupported file extension.")

if __name__ == "__main__":
    main()
