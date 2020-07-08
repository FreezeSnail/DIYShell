all: first
first: shell.c
	gcc  -lreadline shell.c  -o shell && ./shell
clean:
	rm -rf first