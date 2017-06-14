Target = main.exe

LinkLibs = -lSOIL -lglew32 -lglfw3 -lopengl32 -lwinmm -lgdi32 -lglu32 -Wl,--subsystem,windows

OBJS = bin/main.o bin/Model.o bin/Control.o bin/Chunk.o bin/Light.o bin/obj.o bin/Materia.o bin/Player.o

Target : ${OBJS}
	g++ -o ${Target} ${OBJS} ${LinkLibs} 

bin/main.o : src/main.cc
	g++ -o $@ -c $<

bin/Control.o : src/Control.cc src/Control.h src/Model.h
	g++ -o $@ -c $<

bin/Model.o : src/Model.cc src/Model.h src/Camera.h
	g++ -o $@ -c $<

bin/Chunk.o : src/Chunk.cc src/Chunk.h
	g++ -o $@ -c $<
	
bin/Light.o : src/Light.cc src/Light.h
	g++ -o $@ -c $<

bin/obj.o : src/obj.cpp src/obj.h
	g++ -o $@ -c $< -std=c++11
    
bin/Materia.o: src/Materia.cc src/Materia.h
	g++ -o $@ -c $<

bin/Player.o: src/Player.cc src/Player.h
	g++ -o $@ -c $<

.PHONY : clean
clean:
	rm -r bin/*.exe bin/*.o 