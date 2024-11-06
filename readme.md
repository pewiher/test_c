# Compile library
gcc -shared -fPIC -o first_test.so first_test.c
gcc -shared -fPIC -o second_test.so second_test.c

# Compile the interprete
gcc -o libtest library.c -ldl


# run code
./libtest instruction.sc