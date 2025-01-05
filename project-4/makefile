CPP      = g++
CPPFLAGS = -Wall -g
LDFLAGS  = 
OBJFILES = main.o
TARGET   = dining-hall

all: $(TARGET)

$(TARGET): $(OBJFILES)
	$(CPP) $(CPPFLAGS) -o $(TARGET) $(OBJFILES) $(LDFLAGS)

format:
	clang-format -i *.cpp *.h

valgrind: $(TARGET)
	valgrind --tool=memcheck --leak-check=yes --show-reachable=yes --num-callers=20 --track-origins=yes --track-fds=yes ./$(TARGET)

clean:
	rm -f $(OBJFILES) $(TARGET) *~