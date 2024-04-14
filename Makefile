CXXFLAGS += --std=c++17
CXXFLAGS += -I/Users/pravarkataria/opt/openmpi/include
sequential:
	g++ $(CXXFLAGS) -o $(FILE).o $(FILE).cpp 
	./$(FILE).o
mpi : 
	mpic++ $(CXXFLAGS) -o $(FILE).o $(FILE).cpp 
	mpiexec -n $(PROCESSORS) ./$(FILE).o
clean : 
	rm -f *.o