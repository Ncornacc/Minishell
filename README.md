# minishell
42/Codam minishell, a simple shell based on bash
## Description
Writing our own simple little shell in C.

The shell takes single-line inputs and replicates bash behaviour.
It supports:
* redirections - ```<```, ```>```, and ```>>```.
* pipes - ```|```
* separating semicolons - ```;```
* single and double quotes - ```'``` ```"```
* environment variables - ```$```
* ```ctrl-C```, ```ctrl-D```, and ```ctrl-\```

Implemented builtins:
* ```echo``` with option ’-n’
* ```cd``` with relative or absolute path, and '-'
* ```pwd``` without any options
* ```export``` without any options
* ```unset``` without any options
* ```env``` without any options and any arguments
* ```exit``` without any options

## Commands
```
# Build
make

# Run
./minishell
```

![minishell](/minishell.png)(https://github.com/MichelleJiam/minishell)
