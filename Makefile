CC = g++
CFLAGS = -pthread -lm -std=c++11 -Wall -O2 -pipe -g

TARGET1 = condvar
OBJS1 = condvar.o
#SRCS1 = condvar.cpp

TARGET2 = pcondvar
OBJS2 = pcondvar.o




all: $(TARGET1) $(TARGET2)

$(TARGET1): $(OBJS1)
	$(CC)  -o $@ $^ $(CFLAGS)

$(TARGET2): $(OBJS2)
	$(CC)  -o $@ $^ $(CFLAGS)

#%.o:%.c
#	$(CC) $(CFLAGS) -o $@ -c $<
%.o : %.cpp
	$(CC) $(CFLAGS) -o $@ -c $<


clean: 
	rm *.o $(TARGET1) $(TARGET2)
