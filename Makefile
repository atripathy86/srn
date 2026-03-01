CC=g++34
VPATH = src     # find .cpp and .hpp files in src/
INCLUDES = -Isrc # so compiler finds headers after the move
OUTDIR = output
$(shell mkdir -p $(OUTDIR))
#CFLAGS=  -c -w -O2  -DDUPPACK_SET    
#CFLAGS=  -c -w -O2  -DDEBUG  -DSET -g
#CFLAGS=  -c -w -O2 -DDEBUG  -DDEBUG_L1 -DSET -g 
#CFLAGS=  -c -w -O2 -DDEBUG  -DDEBUG_L1 -DDEBUG_L2 -DSET  -g 
#CFLAGS=  -c -w -O2 -DDEBUG  -DDEBUG_L1 -DDEBUG_L2 -DDUPPACK_SET -DSELECTIVE_PACKET_FILTER -g 
#CFLAGS=  -c -w -O2 -DDEBUG  -DDEBUG_L1 -DDEBUG_L2 -DDUPPACK_CACHE -DSELECTIVE_PACKET_FILTER -g 
#CFLAGS=  -c -w -O2 -DDEBUG  -DDEBUG_L1 -DDEBUG_L2 -DDETECT_LOOP -DSELECTIVE_PACKET_FILTER -g 
#CFLAGS=  -c -w -O2 -DDEBUG  -DDEBUG_L1 -DDEBUG_L2 -DSELECTIVE_PACKET_FILTER -g 
#CFLAGS=  -c -w -O2 -DDEBUG  -DDEBUG_L1 -DDEBUG_L2 -DDUPPACK_CACHE -DDETECT_LOOP -DSELECTIVE_PACKET_FILTER -g  # debug build: verbose per-tick/event output, very slow
CFLAGS=  -c -w -O2 -DDUPPACK_CACHE -DDETECT_LOOP -DSELECTIVE_PACKET_FILTER  # release build: no debug output
#CFLAGS=  -c -w -O2 -DDEBUG  -DDEBUG_L1 -DDEBUG_L2 -DDUPPACK_SET -DSELECTIVE_PACKET_FILTER  -DDEBUG_FAULT2 -g 



#OBJS = $(SRCS:.cpp=.o)  # old: objects in root dir
OBJS = $(addprefix $(OUTDIR)/, $(SRCS:.cpp=.o))
#SRCS =  OntologyRepository.cpp Database.cpp Event.cpp  Infrastructure.cpp Analyst.cpp Node.cpp  Listener.cpp  IO_Reporter.cpp Input.cpp Scheduler.cpp simulation.cpp

SRCS =  OntologyRepository.cpp Database.cpp Event.cpp  Infrastructure.cpp Analyst.cpp Node.cpp  Listener.cpp  IO_Reporter.cpp Input.cpp Scheduler.cpp simulation.cpp
EXECUTABLE = simulation

all:  $(OUTDIR)/$(EXECUTABLE)
#	$(CC) ${OBJS} -o $(EXECUTABLE)

#$(EXECUTABLE) : $(OBJS)  # old: binary in root dir
$(OUTDIR)/$(EXECUTABLE) : $(OBJS)
	$(CC) ${OBJS} -o $@

#.cpp.o:  # old suffix rule: objects in root dir
#	$(CC) $(CFLAGS) $(INCLUDES) $< -o $@
$(OUTDIR)/%.o: src/%.cpp
	$(CC) $(CFLAGS) $(INCLUDES) $< -o $@

#${OBJS}



#simulation_test.o: simulation_test.cpp
#	$(CC) $(CFLAGS) simulation.cpp > error1.txt


clean:
	rm -rf $(OUTDIR)

