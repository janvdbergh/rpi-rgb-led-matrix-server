CFLAGS=-Wall -O3 -g
CXXFLAGS=$(CFLAGS)
LDFLAGS+=-lrgbmatrix  -lrt -lm -lpthread

all : bin/displayserver

bin/displayserver : obj/Main.o obj/LedMatrixDisplay.o
	mkdir -p bin
	$(CXX) $^ -o $@ $(LDFLAGS)

obj/%.o : %.cc
	mkdir -p obj
	$(CXX) -I/usr/local/include/rpi-rgb-led-matrix $(CXXFLAGS) -c -o $@ $<

clean:
	rm -rf obj bin
