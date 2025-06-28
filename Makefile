SRCS = addresses.cpp
M_SRC = main.cpp
T_SRC = tester.cpp
OBJS = addresses.o
M_OBJS = main.o
T_OBJS = tester.o
M_EXEC = main.out
T_EXEC = tester.out

$(M_EXEC): $(OBJS) $(M_OBJS)
	clang++ $(OBJS) $(M_OBJS) -o $(M_EXEC)

$(T_EXEC): $(OBJS) $(T_OBJS)
	clang++ $(OBJS) $(T_OBJS) -o $(T_EXEC)

main.o: main.cpp addresses.hpp
	clang++ -c main.cpp

tester.o: tester.cpp addresses.hpp
	clang++ -c tester.cpp

addresses.o: addresses.cpp addresses.hpp
	clang++ -c addresses.cpp

run: main.out
	./main.out

test: tester.out
	./tester.out

clean:
	rm $(OBJS) $(T_OBJS) $(M_OBJS) $(T_EXEC) $(M_EXEC)
