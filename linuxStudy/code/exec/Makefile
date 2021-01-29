src = $(wildcard ./*.c)
obj = $(patsubst ./*.c,./*.o,$(src))
target = app
CC = gcc
CFLAGS = -g -Wall

$(target):$(obj)
	$(CC) $(obj) -o $@ $(CFLAGS)

%.o:%.c
	$(CC) -c $< -o $@ $(CFLAGS)
.PHONY:clean
clean:
	-rm -f $(target)
