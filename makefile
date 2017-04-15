SHELL = cmd.exe
cc = g++
sources = main.cpp BmpFile.cpp
objects = main.o BmpFile.o

main: $(objects)
	$(cc) -std=c++14 -o main main.o BmpFile.o
main.o: main.cpp BmpFile.h
	$(cc) -c main.cpp
BmpFile.o: BmpFile.cpp BmpFile.h
	$(cc) -c BmpFile.cpp

clean:
	del $(objects)

# main: $(sources)
# 	$(cc) -o main $(sources)