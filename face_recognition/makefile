EXENAME = hachathon
OBJS = main.o

CXX = gcc
CXXFLAGS = -lopencv_highgui -lopencv_imgproc -lopencv_core -lopencv_objdetect -Wall  -lwiringPi -I/usr/include/python2.7/ -lpython2.7 -w -pthread -std=c++11
LD = gcc
LDFLAGS = -pthread -std=c++11


$(EXENAME) : $(OBJS)
	$(LD) $(OBJS) $(LDFLAGS) -o $(EXENAME)

main.o: main.cpp 
	$(CXX) $(CXXFLAGS) main.cpp

clean:
	-rm -f *.o $(EXENAME)