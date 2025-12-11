# SM (stands for Stack and Memory), an esolang made by Infinity
SM is a programming language, and is a fun project that I'd like to work with. It is a simple project but yet can run programs with only the memory and stack.

# NOTE
This language uses a unique error code for each type of error
1. For Windows: open cmd, run the program and type `echo %ERRORLEVEL%`
After you retrieved the error code, map it to the following errors it might occur in this file

# Settings
Settings can be enabled using the `@` prefix. For example: `@sm` enables the stack and memory into the program, `@input` enables the input stream, and `@output` enables the output stream. The only error that it can occur in this section is error code 8, is when you used an unexpected setting, like `@codeimprover`. There can be more errors when diving into each specific setting.

# Instructions
Instructions are used after some certain settings is enabled. For example: instruction `push` is unlocked through the setting `@sm`. The only error that it can occur in this section is error code 9, is when you used an unexpected instruction, like `improvemycode`. There can be more errors when diving into each specific instruction.

# `@sm`
`@sm` is a setting that enables the stack and memory for the program. After `@sm`, you must have a number (`8`/`16`/`32`/`64`), which represents the bit size you are working with

There are some kinds of error code you can have while using `@sm`, here is a list of all of them just from the `@sm` setting:
1. Error code `1`: The program expects a number `8`/`16`/`32`/`64`, specifically when your line looks like `@sm`
2. Error code `2`: The line contains more than 2 tokens (aka chunk of string seperated by a space)
3. Error code `3`: Stack and memory is already enabled, specifically when your line looks like `@sm 8`
4. Error code `4`: Stack and memory is already enabled, specifically when your line looks like `@sm 16`
5. Error code `5`: Stack and memory is already enabled, specifically when your line looks like `@sm 32`
6. Error code `6`: Stack and memory is already enabled, specifically when your line looks like `@sm 64`
7. Error code `7`: Your program expects a number 8/16/32/64, but you didn`t put in a number or you put in a number but not 8/16/32/64

# `push` (unlocked through `@sm`)
`push` is an instruction which can be unlocked through setting `@sm`. This is because `push` expects an unsigned integer from 0 to 2^bit size - 1, and it pushes the number you put into the stack

There are some kinds of error code you can have while using `push`, here is a list of all of them just from the `push` instruction:
1. Error code `10`: The `@sm` setting is not enabled
2. Error code `11`: The program expects an unsigned integer, specifically when your line looks like `push`
3. Error code `12`: The line contains more than 2 tokens
4. Error code `13`: The program expects an unsigned integer, but you didn`t take in one
5. Error code `14`: The program expects an 8 bit integer, but you didn`t take in one
6. Error code `15`: The program expects an 16 bit integer, but you didn`t take in one
7. Error code `16`: The program expects an 32 bit integer, but you didn`t take in one
8. Error code `17`: The program expects an 64 bit integer, but you didn`t take in one

# `pop` (unlocked through `@sm`)
`pop` is an instruction which can be unlocked through setting `@sm`. This is because `pop` pops the top element of the stack

There are some kinds of error code you can have while using `pop`, here is a list of all of them just from the `pop` instruction:
1. Error code `18`: The `@sm` setting is not enabled
2. Error code `19`: The line contains more than 1 token
3. Error code `20`: The stack is empty, so popping is impossible

# `learn` (unlocked through `@sm`)
`learn` is an instruction which can be unlocked through setting `@sm`. This is because `learn` saves the top value as the value to be stored, pop it, and save the top value as the memory address to be stored. So basically, this instruction saves a value to a memory address

There are some kinds of error code you can have while using `learn`, here is a list of all of them just from the `learn` instruction:
1. Error code `21`: The `@sm` settings is not enabled
2. Error code `22`: The line contains more than 1 token
3. Error code `23`: Stack is empty
4. Error code `24`: Stack only contains one element

# `remember` (unlocked through `@sm`)
`remember` is an instruction which can be unlocked through setting `@sm`. This is becacuse `remember` saves the top value to be the address that it will retrieve, and pops the top. And then, it pushes the value of the address

There are some kinds of error code you can have while using `remember`, here is a list of all of them just from the `remember` instruction:
1. Error code `25`: The `@sm` settings is not enabled
2. Error code `26`: The line contains more than 1 token
3. Error code `27`: Stack is empty

# `@output`
`@output` is a setting that enables the output stream for the program. After `@output`, you must specify either mode: ascii or int. The int mode will let your output stream to output the values as an integer, while the ascii mode will let your output stream to output the values as an ASCII character.

There are some kinds of error code you can have while using `@output`, here is a list of all of them just from the `@output` setting:
1. Error code `28`: The program expects a mode `ascii` or `int`, specifically when your line looks like `@output`
2. Error code `29`: The line contains more than 2 tokens
3. Error code `30`: Output stream is already enabled, specifically when your line looks like `@output int`
4. Error code `31`: Output stream is already enabled, specifically when your line looks like `@output ascii`
5. Error code `32`: Your code expects mode `ascii`/`int`, but you didn`t put in a keyword ascii or int

# `log` (unlocked through `@output` and `@sm`)
`log` is an instruction which can be unlocked through setting `@output` **and** `@sm`. This is because `log` prints out the value of the top of the stack depending on the mode `@output` is enabled. After it prints, the top value of the stack is popped

There are some kinds of error code you can have while using `log`, here is a list o all of them just from the `log` instruction:
1. Error code `33`: The `@output` settings is not enabled
2. Error code `34`: The `@sm` settings is not enabled
2. Error code `35`: The line contains more than 1 token
3. Error code `36`: Stack is empty

4. Error code `37`: Top of the stack needs to be smaller than 256 in ASCII mode
