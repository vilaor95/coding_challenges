OBJECTS = ccwc.o

default: ccwc

%.o: %.c
	g++ -g -c $< -o $@

ccwc: $(OBJECTS)
	g++ $(OBJECTS) -o $@

.PHONY: test
test: ccwc
	./test.sh
