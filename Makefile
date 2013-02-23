CC=g++34
#CFLAGS=  -c -w -O2  -DDUPPACK_SET    
#CFLAGS=  -c -w -O2  -DDEBUG  -DSET -g
#CFLAGS=  -c -w -O2 -DDEBUG  -DDEBUG_L1 -DSET -g 
#CFLAGS=  -c -w -O2 -DDEBUG  -DDEBUG_L1 -DDEBUG_L2 -DSET  -g 
#CFLAGS=  -c -w -O2 -DDEBUG  -DDEBUG_L1 -DDEBUG_L2 -DDUPPACK_SET -DSELECTIVE_PACKET_FILTER -g 
#CFLAGS=  -c -w -O2 -DDEBUG  -DDEBUG_L1 -DDEBUG_L2 -DDUPPACK_CACHE -DSELECTIVE_PACKET_FILTER -g 
#CFLAGS=  -c -w -O2 -DDEBUG  -DDEBUG_L1 -DDEBUG_L2 -DDETECT_LOOP -DSELECTIVE_PACKET_FILTER -g 
#CFLAGS=  -c -w -O2 -DDEBUG  -DDEBUG_L1 -DDEBUG_L2 -DSELECTIVE_PACKET_FILTER -g 
CFLAGS=  -c -w -O2 -DDEBUG  -DDEBUG_L1 -DDEBUG_L2 -DDUPPACK_CACHE -DDETECT_LOOP -DSELECTIVE_PACKET_FILTER -g 
#CFLAGS=  -c -w -O2  -DDUPPACK_CACHE -DDETECT_LOOP -g    
#CFLAGS=  -c -w -O2 -DDEBUG  -DDEBUG_L1 -DDEBUG_L2 -DDUPPACK_SET -DSELECTIVE_PACKET_FILTER  -DDEBUG_FAULT2 -g 



OBJS = $(SRCS:.cpp=.o)
#SRCS =  OntologyRepository.cpp Database.cpp Event.cpp  Infrastructure.cpp Analyst.cpp Node.cpp  Listener.cpp  IO_Reporter.cpp Input.cpp Scheduler.cpp simulation.cpp 

SRCS =  OntologyRepository.cpp Database.cpp Event.cpp  Infrastructure.cpp Analyst.cpp Node.cpp  Listener.cpp  IO_Reporter.cpp Input.cpp Scheduler.cpp simulation.cpp 
EXECUTABLE = simulation

all:  $(SRCS) $(EXECUTABLE)
#	$(CC) ${OBJS} -o $(EXECUTABLE) 


$(EXECUTABLE) : $(OBJS)
	$(CC) ${OBJS} -o $@  


.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

#${OBJS}



#simulation_test.o: simulation_test.cpp
#	$(CC) $(CFLAGS) simulation.cpp > error1.txt


clean:
	rm -rf *.o $(EXECUTABLE)

