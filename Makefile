# These are used for the library 
VERSION = 1.10
LIBNAME = libtadahdtentity.so.$(VERSION)
LIBALIAS = libtadahdtentity.so
HDIR = /usr/local/include/tada_hdt_entity
LIBDIR = /usr/local/lib

OUT_DIRS = build bin
NOMOBJS = entity.o graph.o tnode.o
OBJS = $(NOMOBJS) main.o 
TOBJS = $(NOMOBJS) tests.o

NOMHEADERS = entity.h graph.h tnode.h

NOMSOURCES = entity.cpp graph.cpp tnode.cpp  
SOURCES = $(NOMSOURCES) main.cpp
TSOURCES = $(NOMSOURCES) tests.cpp

LIBS = -lhdt -pthread -leasylogger -ltabularparser
TLIBS = $(LIBS) -lgtest


INC = -I./include

CC = g++
CXXFLAGS = -std=c++11 $(INC)  

TESTAPP = bin/testapp
COVAPP = bin/covapp

OBJS_ABS = $(patsubst %,build/%,$(OBJS))
TOBJS_ABS = $(patsubst %,build/%,$(TOBJS))
SOURCES_ABS = $(patsubst %,src/%,$(SOURCES))
TSOURCES_ABS = $(patsubst %,src/%,$(TSOURCES))
NOMSOURCES_ABS = $(patsubst %,src/%,$(NOMSOURCES))
HEADERS_ABS = $(patsubst %,include/%,$(NOMHEADERS))


COVCLEANFILES = gcov.css snow.png ruby.png *.gcov  *.gcda *.gcno index-sort-f.html index-sort-l.html index.html \
				amber.png glass.png updown.png coverage.info emerald.png Users usr v1\

UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Linux)
	LIBCMAKETARGET := liblinux
	DETECTEDOS := Linux
endif

ifeq  ($(UNAME_S),Darwin)
	DETECTEDOS := MacOS
	LIBCMAKETARGET := libmac
endif

.PHONY : clean run test cov codecov install lib liblinux libmac


bin/tadaentity: $(OBJS_ABS)
	$(CC) -o $@ $? $(LIBS)


$(OBJS_ABS): $(SOURCES_ABS) $(OUT_DIRS)
	$(CC) $(CXXFLAGS) -c $(SOURCES_ABS)
	mv *.o build/




cov:	
	$(CC) $(CXXFLAGS) -c -fprofile-arcs -ftest-coverage -fPIC  $(TSOURCES_ABS)
	mv *.o build/
	$(CC) $(CXXFLAGS) -o $(COVAPP) -fprofile-arcs -ftest-coverage $(TOBJS_ABS) $(TLIBS) 
	$(COVAPP)
	lcov --directory . --capture --output-file coverage.info
	lcov --remove coverage.info '/usr/*' --output-file coverage.info
	# This is to ignore the xcode directory (will have no effect on linux)
	lcov --remove coverage.info '/Applications/Xcode.app/*' --output-file coverage.info
	lcov --list coverage.info


codecov:
	$(MAKE) cov
	curl -s https://codecov.io/bash > codecovpush.sh
	chmod +x codecovpush.sh
	./codecovpush.sh
	rm codecovpush.sh
	$(MAKE) clean	

lib:
	echo  "Detected OS: " $(DETECTEDOS)
	$(MAKE) $(LIBCMAKETARGET)

libmac:
	$(CC) $(CXXFLAGS)  -dynamiclib -flat_namespace  $(NOMSOURCES_ABS)  -o $(LIBNAME) $(LIBS)

liblinux:
	$(CC) $(CXXFLAGS) -fPIC -shared $(NOMSOURCES_ABS) -o  $(LIBNAME) $(LIBS)

install:
	mkdir -p  $(HDIR)
	cp  $(HEADERS_ABS) $(HDIR)
	$(MAKE) lib
	mv  $(LIBNAME)  $(LIBDIR)
	ln -fs $(LIBDIR)/$(LIBNAME)  $(LIBDIR)/$(LIBALIAS)
	echo -e "tada_hdt_entity lib is installed"
	$(MAKE) clean


run: 
	bin/tadaentity
	

test:
	$(CC)  ${CXXFLAGS} -c $(TSOURCES_ABS)
	mv *.o build/
	$(CC) -o $(TESTAPP) $(TOBJS_ABS) $(TLIBS)
	$(TESTAPP)

bin:	 
	mkdir -p $@

build:
	mkdir -p $@

clean:
	-rm -Rf build
	-rm -Rf bin
	-rm -Rf $(COVCLEANFILES)
	-rm -Rf test.hdt test.hdt.index.v1-1 test.log
