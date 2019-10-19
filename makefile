
CC = g++
SOURCES = lingeo3d.cpp
MAIN_EXEC = triangle.cpp
MAIN_EXECO = $(MAIN_EXEC: .cpp=.o)
OBJECTS = $(SOURCES:.cpp=.o)
EXECUTABLE = sq_inter

all: $(SOURCES) $(MAIN_EXEC) $(EXECUTABLE)



$(EXECUTABLE): $(OBJECTS)
	$(CC) $(OBJECTS) $(MAIN_EXECO) -o $@

.cpp.o:
	$(CC) $(DEBUG) $(FLAGS) -c -o $@ $<


clean:
	rm -rf *.o $(EXECUTABLE)
