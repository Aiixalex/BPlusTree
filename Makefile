TARGET = bptree
OBJS = BPlusTree.o BPTreeNode.o

CFLAGS = -std=c++11
CXX = g++

all: clean $(TARGET)

%.o : %.cpp
	$(CXX) -c $(CFLAGS) $<

$(TARGET): $(OBJS)
	$(CXX) $(CFLAGS) $(OBJS) -o $@

clean:
	rm -f $(TARGET)
	rm -f $(OBJS)