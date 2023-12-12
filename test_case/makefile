CC = gcc
TARGET = test_case
SRCS = test_case.c
OBJS = $(SRCS:.c=.o)

$(TARGET):$(OBJS)
	$(CC) -o $(TARGET) $(SRCS)

%.o:%.c
	$(CC) -c $^ -o $@

clean:
	rm -rf $(OBJS) $(TARGET)