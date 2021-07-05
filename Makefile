build : schimbare.c reprezentanti.c drumuri.c labirint.c bonus.c
	gcc schimbare.c -o p1
	gcc reprezentanti.c -o p2
	gcc drumuri.c -o p3
	gcc labirint.c -o p4
	gcc bonus.c -o bonus
run-p1 : p1
	./p1
run-p2 : p2
	./p2
run-p3 : p3
	./p3
run-p4 : p4
	./p4
run-p5 : bonus
	./bonus
clean: p1 p2 p3 p4 bonus
	rm -f p1
	rm -f p2
	rm -f p3
	rm -f p4
	rm -f bonus
