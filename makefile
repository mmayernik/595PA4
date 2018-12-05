all: Compile

memcheck: Compile mem_test

small_test = tests/4.txt

big_test = tests/r4.sinks

wire_file = wire.param

inverter_file = inv0.param

sink_file = $(small_test)

text_output = output.txt

binary_output = output.b

Compile:
	@gcc -std=c99 -pedantic -Wvla -Wall -Wshadow -g pa4_main.c pa4.c merge_func.c spice.c  -o pa4 -lm

test:
	@./pa4 $(inverter_file) $(wire_file) $(sink_file) $(text_output) $(binary_output)

mem_test: 
	@valgrind --tool=memcheck --track-origins=yes --leak-check=full ./pa4 $(wire_file) $(inverter_file) $(sink_file) $(text_output) $(binary_output)


