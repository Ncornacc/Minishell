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
* escape character - ```\```
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
## Demo
<img src="/minishell.png" width="650" title="minishell">

## Resources
[bash repo](https://github.com/bminor/bash)

[man pages for bash commands](https://man7.org/linux/man-pages/dir_all_by_section.html)

[shell operators](https://unix.stackexchange.com/questions/159513/what-are-the-shells-control-and-redirection-operators)

[environment variable naming conventions](https://pubs.opengroup.org/onlinepubs/9699919799/basedefs/V1_chap08.html)

[process statuses](https://www.gnu.org/software/libc/manual/html_node/Process-Completion-Status.html)

[special exit codes (version-dependant)](https://tldp.org/LDP/abs/html/exitcodes.html#EXITCODESREF)
