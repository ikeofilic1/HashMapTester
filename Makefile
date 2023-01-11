INC_PATH = /home/ikeofili/Desktop/Code/GitHub/Projects/utils
SRC := hash_tests.c $(INC_PATH)/hash_table/hashtable.c
LIB_SRC := $(INC_PATH)/timer/timer.c

LIB_OBJ := timer.o
LIB_A = $(LIB_OBJ:%.o=lib%.a)
LIB_SO = $(LIB_OBJ:%.o=lib%.so)
OBJ = hash_tests.o hashtable.o

CC = gcc
CFLAGS = -I $(INC_PATH) -g -O3

.PHONY: static-build
static-build: $(LIB_A) $(OBJ)
	$(shell [ -f $(LIB_SO) ] && rm $(LIB_SO))
	$(CC) $(OBJ) -L. -ltimer -o $(OUTF)

%.o: %.c
	$(CC) -c $< -o $@ $(CFLAGS)

OUTF = hash_tests

$(LIB_OBJ): $(LIB_SRC)
	$(CC) $(CFLAGS) -c -fPIC $< -o $@

lib%.so: $(LIB_OBJ) #better to make the file a variable. same with .a version
	$(CC) -shared $^ -o $@

lib%.a: $(LIB_OBJ)
	ar rcs $@ $^

.PHONY: clean
clean:
	rm $(OBJ) $(OUTF) *.so *.a $(LIB_OBJ)

$(OBJ): $(SRC)
	$(CC) -c $^ $(CFLAGS)

.PHONY: dynamic-build
dynamic-build: $(LIB_SO) $(OBJ)
	$(CC) $(OBJ) -L. -ltimer -o $(OUTF)
