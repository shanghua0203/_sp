set -x
gcc -w c4.c -o c4
# ./c4 -s test/fib.c
# ./c4 test/fib.c
./c4 hello.c
./c4 -s hello.c
./c4 -d hello.c
# ./c4 c4.c hello.c
# ./c4 c4.c c4.c hello.c