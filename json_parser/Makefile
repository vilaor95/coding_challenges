COMPILER_FLAGS = -Wall -g

PROGRAM = ccjp

OBJECTS = $(PROGRAM).o

default: $(PROGRAM)

%.o: %.cpp
	g++ $(COMPILER_FLAGS) -c $< -o $@

$(PROGRAM): $(OBJECTS)
	g++ $(OBJECTS) -o $@

.PHONY: test
test: $(PROGRAM)
	./test.sh
	./test_extra.sh

.PHONY: clean
clean:
	- rm $(PROGRAM) $(OBJECTS)
