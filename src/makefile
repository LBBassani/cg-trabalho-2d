test:
	g++ -o ../target/test *.cpp -lGL -lGLU -lglut -DTEST

run_test: ../target/test
	../target/test

all:
	g++ -o ../target/trabalho *.cpp -lGL -lGLU -lglut

run: ../target/trabalho
	../target/trabalho $(ARGS)
