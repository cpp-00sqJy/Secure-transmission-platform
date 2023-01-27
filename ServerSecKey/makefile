SRC=$(wildcard ./*.cpp ./log/*.cpp ./openssl/*.cpp ./protobuf/*.cpp  ./shm/*.cpp ./tcpserver/*.cpp ./daemon/*.cpp ./OCCI/*.cpp)

OBJS=$(patsubst %.cpp, %.o, $(SRC)) 
OBJS+=$(patsubst %.cc, %.o, $(wildcard ./protobuf/*.cc))

TARGET=server 

$(TARGET):$(OBJS)
		g++ $^ -o $@  -ljson -lcrypto -lprotobuf -pthread -locci -lclntsh

%.o:%.cpp
		
		@g++ -c $< -o $@

%.o:%.cc
		@g++ -c $< -o $@

.PHONY:clean
clean:

		-rm -rf $(OBJS) $(TARGET)
