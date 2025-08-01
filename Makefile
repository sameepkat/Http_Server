CC = clang++
CFLAGS = -Wall -Werror -std=c++20
SOURCES = src/server.cpp src/http_utils.cpp src/request_handler.cpp
OBJECTS = $(SOURCES:.c=.o)
EXECUTABLE = server

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f $(OBJECTS) $(EXECUTABLE)


# Define dependencies
.PHONY = all clean
