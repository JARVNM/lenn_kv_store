
CC = gcc
FLAGS = -I ./Ntyco/core -L./Ntyco/ -lntyco
SRCS = lenn_kv_rbtree.c lenn_kv_store.c lenn_kv_array.c lenn_epoll_entry.c lenn_coroutine_entry.c
TARGET = lenn_kv_stroe
SUBDIR = ./Ntyco/

OBJS = $(SRCS:.c=.o)

all: $(SUBDIR) $(TARGET)

$(SUBDIR): ECHO
	make -C $@

ECHO:
	@echo $(SUBDIR)

$(TARGET):$(OBJS)
	$(CC) -o $@ $^ $(FLAGS)

%o:%.c
	$(CC) $(FLAGS) -c $^ -o $@

clean:
	rm -rf $(OBJS) $(TARGET)