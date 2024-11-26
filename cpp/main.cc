#include "bf_interpreter.hh"

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <cstring>

const std::string ubf_file_extension = ".ultimate_brainfuck";
const std::string ubf_valid_chars = "><+-.,[]";

bool hasUbfExtension(const std::string& filename)
{
    return filename.size() >= ubf_file_extension.size() && filename.substr(filename.size() - ubf_file_extension.size()) == ubf_file_extension;
}

bool hasBfExtension(const std::string& filename)
{
    return (filename.size() >= 3 && filename.substr(filename.size() - 3) == ".bf") ||
    (filename.size() >= 10 && filename.substr(filename.size() - 10) == ".brainfuck");
}

std::vector<char> readBfFile(const std::string& filename)
{
    std::vector<char> characters;
    std::ifstream file(filename);

    if (!file.is_open())
    {
        std::cerr << "Error: Could not open file " << filename << "\n";
        return characters;
    }

    std::ostringstream buffer;
    buffer << file.rdbuf();
    std::string data = buffer.str();

    for (char ch : data)
    {
        if (ubf_valid_chars.find(ch) != std::string::npos)
        {
            characters.emplace_back(ch);
        }
    }
    return characters;
}

char increment(char bfc)
{
    switch (bfc)
    {
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

char decrement(char bfc)
{
    switch (bfc)
    {
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

std::string convertToUltimateBrainfuck(const std::vector<char>& parsed)
{
    std::string ultimate;
    size_t counter = 0;

    for (size_t i = 0; i < parsed.size(); ++i)
    {
        char c = parsed[i];

        for (size_t j = 0; j < counter; ++j)
        {
            c = increment(c);
        }
        ultimate += c;
        counter++;
    }
    return ultimate;
}

std::string convertToBrainfuck(const std::vector<char>& parsed)
{
    std::string brainfuck;
    size_t counter = 0;

    for (size_t i = 0; i < parsed.size(); ++i)
    {
        char c = parsed[i];

        for (size_t j = 0; j < counter; ++j)
        {
            c = decrement(c);
        }
        brainfuck += c;
        counter++;
    }
    return brainfuck;
}

int main(int argc, char** argv)
{
    if (argc < 2)
    {
        std::cerr << "Usage: " << argv[0] << " <file.bf or file.ultimate-brainfuck> \n";
        return 1;
    }

    std::string input = argv[1];

    if (hasBfExtension(input))
    {
        auto bf = readBfFile(input);
        auto ultimatebrainfuck = convertToUltimateBrainfuck(bf);
        std::cout << "Brainfuck transpiled to ultimate brainfuck:" << std::endl;
        std::cout << ultimatebrainfuck;
    }
    else if (hasUbfExtension(input))
    {
        auto ubf = readBfFile(input);
        std::string bf = convertToBrainfuck(ubf);
        std::cout << "Ultimate brainfuck transpiled to brainfuck:" << std::endl;
        std::cout << bf << std::endl;
        std::cout << "Interpreting transpiled brainfuck:" << std::endl;
        std::cout << std::endl;

        char* cstr = new char[bf.length() + 1];
        std::strcpy(cstr, bf.c_str());
        interpret(cstr);
        delete[] cstr;
    }
    return 0;
}
