assembler: try.o node.o macro.o label.o valid_operand.o main.o line.o add_data.o kidud.o second_pass.o 
	gcc -Wall -ansi -pedantic -o assembler try.o node.o macro.o label.o valid_operand.o main.o line.o add_data.o kidud.o second_pass.o -fsanitize=address -fno-omit-frame-pointer

try.o: try.c mac.h
	gcc -Wall -ansi -pedantic -c try.c -fsanitize=address -fno-omit-frame-pointer

node.o: node.c mac.h
	gcc -Wall -ansi -pedantic -c node.c -fsanitize=address -fno-omit-frame-pointer

macro.o: macro.c mac.h
	gcc -Wall -ansi -pedantic -c macro.c -fsanitize=address -fno-omit-frame-pointer         

label.o: label.c mac.h
	gcc -Wall -ansi -pedantic -c label.c -fsanitize=address -fno-omit-frame-pointer

valid_operand.o: valid_operand.c mac.h
	gcc -Wall -ansi -pedantic -c valid_operand.c -fsanitize=address -fno-omit-frame-pointer

main.o: main.c mac.h
	gcc -Wall -ansi -pedantic -c main.c -fsanitize=address -fno-omit-frame-pointer

line.o: line.c mac.h
	gcc -Wall -ansi -pedantic -c line.c -fsanitize=address -fno-omit-frame-pointer

add_data.o: add_data.c mac.h
	gcc -Wall -ansi -pedantic -c add_data.c -fsanitize=address -fno-omit-frame-pointer

kidud.o: kidud.c mac.h
	gcc -Wall -ansi -pedantic -c kidud.c -fsanitize=address -fno-omit-frame-pointer

second_pass.o: second_pass.c mac.h
	gcc -Wall -ansi -pedantic -c second_pass.c -fsanitize=address -fno-omit-frame-pointer



