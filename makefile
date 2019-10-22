
CC = g++

SOURCES_inter = triangle.cpp
OBJECTS_inter = $(SOURCES_inter:.cpp=.o)
EXECUTABLE_inter = inter_sorted

SOURCES_gen = gen_triangles.cpp
OBJECTS_gen = $(SOURCES_gen:.cpp=.o)
EXECUTABLE_gen = gen_triangles

do_inter_sorted:  $(SOURCES_inter) $(EXECUTABLE_inter)

do_gen_triangles: $(SOURCES_gen) $(EXECUTABLE_gen) 

gen_bunch_of_tests: do_gen_triangles
	mkdir tests
	./gen_triangles 10 100 60 >./tests/1.dat
	./gen_triangles 100 100 30 >./tests/2.dat
	./gen_triangles 1000 100 10 >./tests/3.dat
	./gen_triangles 10000 100 2 >./tests/4.dat
	./gen_triangles 100000 100 1 >./tests/5.dat
	./gen_triangles 100000 200 1 >./tests/6.dat
	./gen_triangles 1000000 1000 1 >./tests/7.dat

run_tests: do_inter_sorted
	./inter_sorted <./tests/1.dat >./tests/1.ans
	./inter_sorted <./tests/2.dat >./tests/2.ans
	./inter_sorted <./tests/3.dat >./tests/3.ans
	./inter_sorted <./tests/4.dat >./tests/4.ans
	./inter_sorted <./tests/5.dat >./tests/5.ans
	./inter_sorted <./tests/6.dat >./tests/6.ans
	./inter_sorted <./tests/7.dat >./tests/7.ans

all: do_inter_sorted do_gen_triangles

$(EXECUTABLE_inter): $(OBJECTS_inter)
	$(CC) $(OBJECTS_inter)  -o $@

$(EXECUTABLE_gen): $(OBJECTS_gen)
	$(CC) $(OBJECTS_gen)  -o $@

.cpp.o:
	$(CC) $(DEBUG) $(FLAGS) -c -o $@ $<


clean:
	rm -rf *.o $(EXECUTABLE_inter) $(EXECUTABLE_gen) tests
