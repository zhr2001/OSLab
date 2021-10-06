head = execrise.h
srcs = execrise.c
objs = execrise.o
opts = -g -c
all: main
main: $(objs)
	gcc $(objs) -o main

execrise.o: $(srcs) $(head)
	gcc  $(opts) $(srcs)

clean:
	rm main *.o