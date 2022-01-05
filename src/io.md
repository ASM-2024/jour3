---
title: "Low-level input and output"
author: victor-emmanuel.provost
logo: https://avatars.githubusercontent.com/u/96961500?s=200&v=4
---
[Today's slides](C_UNIX_Workshop_IO_Signals.pdf)

# Introduction

The first part of today's practical aims at seeing how input/output is managed
at low-level. To do so, you are going to be restricted in the headers you can
use, meaning, you **cannot** use `<stdio.h>`.

Here is the skeleton for this part:
[I/O Skeleton](io_skeleton.tar.gz)

# Low-level I/O
## my_puts.c
For this part, you will need the following `syscalls`:

- open(2)
- read(2)
- stat(2)
- write(2)

The first step you will be taken is to code a small version of `puts(3)`
You are expected to write a function that, when given a string, will print it
to `stdout`.

## file_type.c
This exercise aims at introducing `stat(2)`. Your objective is to write the
function `void file_type(char *filename)` that takes a filename and prints
whether this file is a regular file or a directory. (Hint: You might want
to take a look at the macros S_IS*-something* that come with `stat`).

Expected output:
```sh
asm$ ./file_type some_file
The given file is a regular file!
asm$ ./file_type some_dir
The given file is a directory!
```

## parrot.c
The purpose of this exercise is to write a program that takes a file as a
parameter, and asks the user to give a number which represents the amount of
time the content of the given file will be printed on the terminal.
Extra information are given in the skeleton of the `parrot.c` file.

Expected output:
```sh
asm$ cat some_file
Hello there!
asm$ ./parrot some_file
Squuuuaaaaaawk! Hello human, give me a number!
5
Hello there!
Hello there!
Hello there!
Hello there!
Hello there!
asm$ ./parrot some_file
a
parrot: Squuuuaaaaaawk! The given value is invalid. Try again!
Squuuuaaaaaawk! Hello human, give me a number!
```

# Correction

A correction will be given this weekend. It will be on the Github repository
and the link will be given here.
This will **not** be the **only** nor **the best**
solution to the problem but do take the time to read it as it uses advanced
concepts!

