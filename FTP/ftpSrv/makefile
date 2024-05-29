GCC ?= g++
CCMODE = PROGRAM
INCLUDES =  -I/usr/local/include/
CFLAGS =  -Wall $(MACRO) 
CFLAGS = -Wall -g -DTEST #开启测试模式testout()
TARGET = ftpServer
SRCS := $(wildcard *.cpp)   
LIBS = -L /usr/local/lib/  -levent -lpthread



ifeq ($(CCMODE),PROGRAM)
$(TARGET): 
	$(GCC) $(CFLAGS) $(INCLUDES) -o $(TARGET)  $(SRCS) $(LIBS)
	@chmod +x $(TARGET)
	@echo make $(TARGET) ok.

.PHONY:clean
clean:
	rm -rf $(TARGET)
endif

.PHONY:install
