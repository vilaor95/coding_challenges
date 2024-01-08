PROGRAM = ccjp

OBJECTS = $(PROGRAM).o

default: $(PROGRAM)

%.o: %.c
	g++ -g -c $< -o $@

$(PROGRAM): $(OBJECTS)
	g++ $(OBJECTS) -o $@

.PHONY: test
test: $(PROGRAM)
	./test.sh
	./test_extra.sh
