CC = gcc
TARGETS = hw1

$(TARGETS) :
	$(CC) hw1.c -o hw1

clean :
	rm hw1
