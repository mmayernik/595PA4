all: Compile 

memcheck: Compile mem_test

Compile:
	@gcc -std=c99 -pedantic -Wvla -Wall -Wshadow -g pa4_main.c pa4.c merge_func.c spice.c  -o pa4 -lm

test:
	@./pa4 

Compile_zst:
	@gcc -std=c99 -pedantic -Wvla -Wall -Wshadow -g git_pulls/595PA4/zst.c git_pulls/595PA4/zst_main.c git_pulls/595PA4/merge_func.c -o merge -lm

Build_binary:
	@./merge git_pulls/595PA4/4.txt output.txt output.b

loc1:
	@./pa3 git_pulls/595PA4/4.txt Output_File1 output.b

valgrind: 
	@valgrind --tool=memcheck
mem_test1: 
	@valgrind --tool=memcheck --track-origins=yes --leak-check=full ./pa3 4.txt Output_File1 Output_File2
diff2:
	@diff Output_File2 16b.len

