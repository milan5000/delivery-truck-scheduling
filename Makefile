# i know this file probably looks very amateurish
# but it works for me so I don't really mind
SRCS = src/addresses.cpp
INCL = include/addresses.hpp
M_SRC = main.cpp
T_SRC = tester.cpp
OBJS = addresses.o
M_OBJS = main.o
T_OBJS = tester.o
M_EXEC = main.out
T_EXEC = tester.out
VERSION = -std=c++11

$(M_EXEC): $(OBJS) $(M_OBJS)
	clang++ $(OBJS) $(M_OBJS) -o $(M_EXEC) $(VERSION)

$(T_EXEC): $(OBJS) $(T_OBJS)
	clang++ $(OBJS) $(T_OBJS) -o $(T_EXEC) $(VERSION)

main.o: main.cpp $(INCL)
	clang++ -c main.cpp $(VERSION)

tester.o: tester.cpp $(INCL)
	clang++ -c tester.cpp $(VERSION)

addresses.o: src/addresses.cpp include/addresses.hpp
	clang++ -c src/addresses.cpp $(VERSION)

run: main.out
	./main.out

test: tester.out
	./tester.out

clean:
	rm $(OBJS) $(T_OBJS) $(M_OBJS) $(T_EXEC) $(M_EXEC)
