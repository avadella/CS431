CPP = g++
CFLAGS = -Wall -O2
GDBFLAGS = -g
OBJFILES = main.o scheduler.o process.o shell.o
TARGET   = main

all: $(TARGET)

$(TARGET): $(OBJFILES)
	$(CPP) $(CPPFLAGS) -o $(TARGET) $(OBJFILES) $(LDFLAGS)

format:
	clang-format -i main.cpp
	clang-format -i process.h
	clang-format -i process.cpp
	clang-format -i scheduler.h
	clang-format -i scheduler.cpp
	clang-format -i shell.h
	clang-format -i shell.cpp

sanitize:
	# https://github.com/google/sanitizers/wiki/AddressSanitizer
	clang -fsanitize=address -O1 -fno-omit-frame-pointer -g

valgrind: main
	valgrind --tool=memcheck --leak-check=yes --show-reachable=yes --num-callers=20 --track-origins=yes --track-fds=yes ./$(TARGET)

clean:
	rm -f $(OBJFILES) $(TARGET) *~