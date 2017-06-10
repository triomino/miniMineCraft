main : src\main.cc
	g++ src\main.cc
.PHONY : clean
clean:
	rm -r *.exe *.o
