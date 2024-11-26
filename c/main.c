#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define TAPE_SIZE 30000
#define UBF_FILE_EXTENSION ".ultimate_brainfuck"
#define UBF_VALID_CHARS "><+-.,[]"

bool has_ubf_extension(const char* filename) {
    size_t len = strlen(filename);
    size_t ext_len = strlen(UBF_FILE_EXTENSION);
    return len >= ext_len && strcmp(filename + len - ext_len, UBF_FILE_EXTENSION) == 0;
}

bool has_bf_extension(const char* filename) {
    size_t len = strlen(filename);
    return (len >= 3 && strcmp(filename + len - 3, ".bf") == 0) ||
           (len >= 10 && strcmp(filename + len - 10, ".brainfuck") == 0);
}

char* read_bf_file(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, "Error: Could not open file %s\n", filename);
        return NULL;
    }

    fseek(file, 0, SEEK_END);
    size_t length = ftell(file);
    fseek(file, 0, SEEK_SET);

    char* data = (char*)malloc(length + 1);
    if (!data) {
        fprintf(stderr, "Error: Memory allocation failed\n");
        fclose(file);
        return NULL;
    }

    fread(data, 1, length, file);
    data[length] = '\0';
    fclose(file);

    char* filtered = (char*)malloc(length + 1);
    if (!filtered) {
        fprintf(stderr, "Error: Memory allocation failed\n");
        free(data);
        return NULL;
    }

    size_t j = 0;
    for (size_t i = 0; i < length; i++) {
        if (strchr(UBF_VALID_CHARS, data[i])) {
            filtered[j++] = data[i];
        }
    }
    filtered[j] = '\0';
    free(data);
    return filtered;
}

char increment(char bfc) {
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

char decrement(char bfc) {
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

char* convert_to_ultimate_brainfuck(const char* parsed) {
    size_t length = strlen(parsed);
    char* ultimate = (char*)malloc(length + 1);
    if (!ultimate) {
        fprintf(stderr, "Error: Memory allocation failed\n");
        return NULL;
    }

    size_t counter = 0;
    for (size_t i = 0; i < length; i++) {
        char c = parsed[i];
        for (size_t j = 0; j < counter; j++) {
            c = increment(c);
        }
        ultimate[i] = c;
        counter++;
    }
    ultimate[length] = '\0';
    return ultimate;
}

char* convert_to_brainfuck(const char* parsed) {
    size_t length = strlen(parsed);
    char* brainfuck = (char*)malloc(length + 1);
    if (!brainfuck) {
        fprintf(stderr, "Error: Memory allocation failed\n");
        return NULL;
    }

    size_t counter = 0;
    for (size_t i = 0; i < length; i++) {
        char c = parsed[i];
        for (size_t j = 0; j < counter; j++) {
            c = decrement(c);
        }
        brainfuck[i] = c;
        counter++;
    }
    brainfuck[length] = '\0';
    return brainfuck;
}

void interpret(const char* input) {
    unsigned char tape[TAPE_SIZE] = {0};
    unsigned char* ptr = tape;
    size_t length = strlen(input);

    for (size_t i = 0; i < length; i++) {
        switch (input[i]) {
            case '>': ptr++; break;
            case '<': ptr--; break;
            case '+': (*ptr)++; break;
            case '-': (*ptr)--; break;
            case '.': putchar(*ptr); break;
            case ',': *ptr = getchar(); break;
            case '[':
                if (!*ptr) {
                    int loop = 1;
                    while (loop > 0) {
                        i++;
                        if (input[i] == '[') loop++;
                        else if (input[i] == ']') loop--;
                    }
                }
                break;
            case ']':
                if (*ptr) {
                    int loop = 1;
                    while (loop > 0) {
                        i--;
                        if (input[i] == '[') loop--;
                        else if (input[i] == ']') loop++;
                    }
                }
                break;
        }
    }
}

int main(int argc, char** argv)
{
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <file.bf or file.ultimate_brainfuck>\n", argv[0]);
        return 1;
    }

    const char* input_file = argv[1];

    if (has_bf_extension(input_file)) {
        char* bf = read_bf_file(input_file);
        if (!bf) return 1;

        char* ultimate_brainfuck = convert_to_ultimate_brainfuck(bf);
        free(bf);

        if (ultimate_brainfuck) {
            printf("Brainfuck transpiled to ultimate brainfuck:\n%s\n", ultimate_brainfuck);
            free(ultimate_brainfuck);
        }
    } else if (has_ubf_extension(input_file)) {
        char* ubf = read_bf_file(input_file);
        if (!ubf) return 1;

        char* brainfuck = convert_to_brainfuck(ubf);
        free(ubf);

        if (brainfuck) {
            printf("Ultimate brainfuck transpiled to brainfuck:\n%s\n", brainfuck);
            printf("Interpreting transpiled brainfuck:\n");
            interpret(brainfuck);
            free(brainfuck);
        }
    } else {
        fprintf(stderr, "Error: Unsupported file extension.\n");
        return 1;
    }

    return 0;
}
