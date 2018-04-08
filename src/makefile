CC = g++
OUTDIR = ../bin/
BULLETGL = ../lib/bulletgl/
INCDIRS = -I . -I $(BULLETGL) -I ../lib/
CFLAGS =  -ggdb -lGL -lGLU -lglut -lGLEW -lBulletDynamics -lBulletCollision -lBulletSoftBody -lLinearMath -Wall 
LDFLAGS = -I /usr/local/include/bullet/
# RUNTIMEDIRS = -Wl,-rpath,'$$ORIGIN/usr/local/lib'
RUNTIMEDIRS = -Wl,-rpath,'/usr/local/lib'
build:
	mkdir -p $(OUTDIR)
	$(CC) $(CFLAGS) $(INCDIRS) $(LDFLAGS) $(RUNTIMEDIRS) ./*.cpp $(BULLETGL)*.cpp ../lib/*.cpp  -o $(OUTDIR)a.out
	echo "## makefile: running. ##"
	$(OUTDIR)a.out
run:
	echo "## makefile: running. ##"
	$(OUTDIR)a.out
debug:
	gdb $(OUTDIR)a.out
clean:
	rm -r $(OUTDIR)
