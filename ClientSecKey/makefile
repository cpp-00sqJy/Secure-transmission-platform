SRC=$(wildcard ./*.cpp ./openssl/*.cpp ./protobuf/*.cpp  ./shm/*.cpp ./tcpclient/*.cpp)

OBJS=$(patsubst %.cpp, %.o, $(SRC)) 
OBJS+=$(patsubst %.cc, %.o, $(wildcard ./protobuf/*.cc))

TARGET=client

$(TARGET):$(OBJS)
		@g++ $^ -o $@  -ljson -lcrypto -lprotobuf 

#%.o:%.cpp
		
#		@g++ -c $< -o $@

#%.o:%.cc
#		@g++ -c $< -o $@

.PHONY:clean
clean:

		-rm -rf $(OBJS) $(TARGET)
