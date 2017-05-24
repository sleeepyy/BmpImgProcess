SHELL = cmd.exe
cc = g++
sources = main.cpp BmpFile.cpp
objects = main.o BmpFile.o
target = main

$(target): $(objects)
	$(cc) -std=c++14 -Wall -o main main.o BmpFile.o
main.o: main.cpp BmpFile.h
	$(cc) -c main.cpp
BmpFile.o: BmpFile.cpp BmpFile.h
	$(cc) -c BmpFile.cpp

clean:
	del $(target) $(objects)

# main: $(sources)
# 	$(cc) -o main $(sources)