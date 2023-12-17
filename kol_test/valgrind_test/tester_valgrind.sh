#!/bin/bash

if ! g++ @opcjeCpp btester.cpp ../../kol.cpp -o bkol; then
    echo "Compilation error!"
    exit
fi

wrong_test=-1

for((i = 0;i < 30; i++)) {
    echo "-------------------Test number $i-------------------"
    
    
    if ! valgrind --track-origins=yes --tool=memcheck --leak-check=yes ./bkol <<< $i; then
        wrong_test=$i
        break
    else
        echo "-------------------All correct!----------------------"
    fi
    
    echo ""
}

if [ $wrong_test -ne -1 ]; then
    echo "Wrong answer or runtime error on test $wrong_test"
else
    echo "Congrats! All tests passed :)"
fi
