ENV := valgrind-debug
ENV := gdb-debug
ENV := no-debug

VALGRIND_TOOL=memcheck
VALGRIND_TOOL=helgrind

GPP := g++

ifeq ($(ENV),gdb-debug)
GPP += -Wall -g -O0 -lm
else
ifeq ($(ENV),valgrind-debug)
GPP += -Wall -g -O0 -lm
else
GPP += -Wall -lm
endif
endif

kaoshi: 
	rm -f ./*.out
	$(GPP) ./main.cpp -o ./main.out
ifeq ($(ENV),gdb-debug)
	gdb ./main.out
else
ifeq ($(ENV),valgrind-debug)
	valgrind --tool=$(VALGRIND_TOOL) ./main.out
else
	./main.out
endif
endif