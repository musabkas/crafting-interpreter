build:
	g++ -std=c++17 *.cpp -g -o loxi.out

runprompt:
	make build
	./loxi.out

runscript:
	make build
	./loxi.out script.lox