
OUT_DIRS = build bin
OBJS = main.o
SOURCES = main.cpp
CC = g++
CXXFLAGS = -std=c++11  

OBJS_ABS = $(patsubst %,build/%,$(OBJS))
SOURCES_ABS = $(patsubst %,src/%,$(SOURCES))


.PHONY : clean run


bin/tadaentity: $(OBJS_ABS)
	$(CC) -o $@ $? $(LIBS)

$(OBJS_ABS): $(SOURCES_ABS) $(OUT_DIRS)
	$(CC) $(CXXFLAGS) -c $(SOURCES_ABS)
	mv *.o build/


bin:	 
	mkdir -p $@

build:
	mkdir -p $@


run: 
	bin/tadaentity
	


clean:
	-rm -Rf build
	-rm -Rf bin

