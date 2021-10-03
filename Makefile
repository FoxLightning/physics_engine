
build:
	g++ ./main.cpp -lsfml-graphics -lsfml-window -lsfml-system

run:
	./a.out

clear:
	ls | grep a.out | xargs rm