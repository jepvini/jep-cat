CFLAGS += -Wall
# GCC warnings that Clang doesn't provide:
ifeq ($(CC),gcc)
    CFLAGS += -Wjump-misses-init -Wlogical-op
endif

CFLAGS += -O3
CFLAGS += -lpthread

cat:
	gcc ./cat.c ${CFLAGS} -o cat
