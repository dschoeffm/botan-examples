# clang aes.cpp -I /usr/local/include/botan-1.11 -std=c++14 -L /usr/local/lib/ -lbotan

CXX=clang
INCLUDE=-I/usr/local/include/botan-1.11
CXXFLAGS=-std=c++11
LIBPATH=-L/usr/local/lib/
LIBS=-lbotan-1.11 -lstdc++

PROGS=aes

SRCS = $(wildcard *.cpp)
OBJS = $(SRCS:.cpp=.o)

#all:
#	${CXX} ${LIBPATH} ${LIBS} ${CXXFLAGS} ${INCLUDE} aes.cpp -o aes

all: $(PROGS)

$(PROGS): $(OBJS)
	$(CXX) ${LIBPATH} ${LIBS} ${CXXFLAGS} ${INCLUDE} $^ -o $@

.cpp.o:
	${CXX} ${CXXFLAGS} ${INCLUDE} -c $<

clean:
	$(RM) $(PROGS) $(OBJS)
