# List_Maker
A tool to make lists and then pull random entries from said list

## Purpose
I personaly use this to list names I like, so when a game asks for a name I can just randomly get a name from my list

## Install
```
chmod +x build.sh
./build.sh
```
or
```
g++ -c main.cpp FileManager.cpp
g++ main.o FileManager.o -o program
rm *.o
```

## Run
```
./program
```
Syntax
```
{command} {option} element
```
Commands
```
-h    help
-g    get
-a    add
-s    sub
-c    change
-l    list
-r    random
```
Options
```
-h    help
-e    element
-f    file
-d    directory
```
