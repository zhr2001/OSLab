src = exercise.c
opts = -l pthread
objs = exercise.o
all: main
main: ${objs}
	gcc ${objs} ${opts} -o main
exercise.o: ${src}
	gcc -g -c ${src}
clean:
	rm main *.o