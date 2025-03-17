OUT = bin/tictac.a
CC = clang++
ODIR = obj
SDIR = src
INC = -I./include -I/usr/include
CFLAGS = -x c++ -Wall -O3 -DNDEBUG --std=c++17 -fdiagnostics-color=always
LINKFLAGS = -L /usr/lib -ltorch -ltorch_cpu -lc10 -Wl,-rpath,/usr/lib
SANITY = -fsanitize=address,undefined,leak,bounds,float-divide-by-zero,float-cast-overflow,nullability,alignment,integer -fno-sanitize=unsigned-shift-base -fno-omit-frame-pointer -fstack-protector-all

_OBJS = agents/Agent.o agents/RandomAgent.o agents/MCTSAgent.o agents/ManualAgent.o agents/AlphaBetaAgent.o agents/NNStaticEval.o agents/MCTSStaticEval.o agents/BasicStaticEval.o display.o BoardData.o Engine.o dataset.o main.o
OBJS = $(patsubst %,$(ODIR)/%,$(_OBJS))


$(ODIR)/%.o: $(SDIR)/%.cpp 
	$(CC) -c $(INC) $(CFLAGS) -o $@ $<

$(OUT): $(OBJS) 
	$(CC) $(OBJS) $(LINKFLAGS) -o $@

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o $(OUT)
	rm -f $(ODIR)/agents/*.o $(OUT)

