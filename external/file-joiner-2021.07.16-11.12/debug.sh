clear matrac
clear
clang++ -w src/debug-main.cpp -lsqlite3 -o matrac
if [ $? -eq 0 ]; then
    ./matrac
fi