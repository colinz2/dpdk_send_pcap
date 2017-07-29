CC = g++

LIBS = -lpthread -ldl -lm  -Wl,--whole-archive -ldpdk -Wl,--no-whole-archive

CC_OPTS =  -g -O3 -march=native -msse4.2 -msse4.1 -mssse3 -msse4 -msse2

CC_OPTS += -I$(DPDK_BUILD)/include -m64 --std=c++11 
LD_OPTS += -L$(DPDK_BUILD)/lib $(LIBS)

all: send_pack

send_pack: send_pack.o
	$(CC) $(LD_OPTS) -o $@ $<

send_pack.o: send_pack.cc
	$(CC) $< $(CC_OPTS) -o $@ -c

clean:
	rm -rf *.o send_pack
