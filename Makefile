Target = main

LinkLibs = -lSOIL -lglew32 -lglfw3 -lopengl32 -lwinmm -lgdi32 -lglu32 -Wl,--subsystem,windows

OBJS = bin/main.o bin/Model.o bin/Control.o bin/Chunk.o

Target : ${OBJS}
	g++ -o ${Target} ${OBJS} ${LinkLibs} 

bin/main.o : src/main.cc
	g++ -o $@ -c $<

bin/Control.o : src/Control.cc src/Control.h src/Model.h
	g++ -o $@ -c $<

bin/Model.o : src/Model.cc src/Model.h
	g++ -o $@ -c $<

bin/Chunk.o : src/Chunk.cc src/Chunk.h
	g++ -o $@ -c $<

.PHONY : clean
clean:
	rm -r bin/*.exe bin/*.o 