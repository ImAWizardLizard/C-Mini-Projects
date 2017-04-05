CFLAGS=-Wall -g

all: clean logfind
	./logfind || true
	./logfind linux
	./logfind harry
	./logfind process linux
	./logfind -o harry process linux da d

clean:
	rm -f logfind
