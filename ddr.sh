printf 'building... '
gcc -g -O0 brtplan.c -lncursesw -ldl -lpthread -lm -o brtplan.exe
./brtplan.exe
rm *.exe
if [ $? -eq 0 ]; then
        printf 'succeeded!\n'
fi
