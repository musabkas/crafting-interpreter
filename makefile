build:
	g++ loxi.cpp -o loxi.out

runprompt:
	make build
	./loxi.out

runscript:
	make build
	./loxi.out tmp.lox