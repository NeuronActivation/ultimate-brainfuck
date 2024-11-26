# ultimate-brainfuck

Ultimate brainfuck is an esoteric programming language that adds an evil twist to brainfuck.

A counter is added to brainfuck programming. This counter starts from zero and increases by one for every character in the ultimate brainfuck program.

Every character is moved the by the value of the counter. So in a brainfuck program with fifty characters the last character would have to be moved fifty times.

The character moves by taking the next value in the order `><+-.,[]` and `]` will continue from the beginning.

