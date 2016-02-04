CC = clang
CFLAGS = -O3 -Wall -Werror -march=native
LDFLAGS = -Wl,-s -lsqlite3
TARGET_BINARY = xsqlite3
TARGET_SOURCES = example.c xsqlite3.c
TARGET_OBJECTS = $(TARGET_SOURCES:%.c=%.o)

all: $(TARGET_BINARY)

clean:
	rm -f $(TARGET_BINARY) $(TARGET_OBJECTS)

.c.o:
	$(CC) $(CFLAGS) -c $<

.PHONY: clean

$(TARGET_BINARY): $(TARGET_OBJECTS)
	$(CC) $(LDFLAGS) -o $@ $(TARGET_OBJECTS)
