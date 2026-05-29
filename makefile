build:
	g++ -std=c++17 *.cpp -o loxi.out

runprompt:
	make build
	./loxi.out

runscript:
	make build
	./loxi.out tmp.lox