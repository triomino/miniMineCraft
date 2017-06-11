
SingleFile = src\main.cc

LinkLibs = -lSOIL -lglew32 -lglfw3 -lopengl32 -lwinmm -lgdi32 -lglu32 -Wl,--subsystem,windows

IncludeFiles = src\Camera.h src\Shader.h src\Chunk.h src\Random.h src\Player.h src\vs.vs src\fs.frag

main : ${SingleFile} ${IncludeFiles}
	g++ -o main ${SingleFile} ${LinkLibs}
.PHONY : clean
clean:
	rm -r *.exe *.o 