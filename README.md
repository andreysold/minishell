# minishell
Целью проекта является создание простой оболочки shell

**Разрешенные функции:**
_readline, rl_clear_history, rl_on_new_line,rl_replace_line, rl_redisplay, add_history, printf, malloc, free, write, access, open, read,close, fork, wait, waitpid, wait3, wait4, signal, sigaction, sigemptyset, sigaddset, kill, exit, getcwd, chdir, stat, lstat, fstat, unlink, execve, dup, dup2, pipe, opendir, readdir, closedir, strerror, perror, isatty, ttyname, ttyslot, ioctl, getenv, tcsetattr, tcgetattr, tgetent, tgetflag, tgetnum, tgetstr, tgoto, tputs_

## Особенности

* История ранее введенных команд
* Нахождение и запуск правильного исполняемого файла (на основе переменной PATH, используя относительный или абсолютный путь)
* Переменные среды ($, за которым следует последовательность символов) расширяются до своих значений)
* Подстановочные знаки в текущем рабочем каталоге
ctrl-C, ctrl-D и ctrl-\ ведут себя как в bash
’ (одинарные кавычки — предотвращают интерпретацию метасимволов в последовательности в кавычках)
" (двойные кавычки - предотвращают интерпретацию метасимволов в последовательности в кавычках, кроме $)
* $? расширяется до последнего статуса выхода
* | соединять cmds или группы трубами; вывод cmd подключается к вводу следующего cmd через конвейер

## Встроенные функции(модули)
* echo with option -n
* cd (relative or absolute path, - for OLDPWD, without arg for HOME)
* pwd without options
* export without options
* unset without options
* env without options
* exit [exit_status] without options

## Redirections:
* [n] (необязательный) указывает дескриптор файла, если не указан, это стандартный вывод/стандартный ввод

* [n]< файл перенаправление ввода
* << хердок
* [n]> файл, перенаправляющий вывод
* [n]>> файл добавление перенаправленного вывода

## Запуск програмы

* git clone <repo-url> minishell
* cd minishell
* git submodule init
* git submodule update
* make/make run
* ./minishell
