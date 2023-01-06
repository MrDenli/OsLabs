compile:
	g++ -c -Wall -Werror -fpic lib1.cpp -ldl
	g++ -c -Wall -Werror -fpic lib2.cpp -ldl
	g++ -shared -o liblib1.so lib1.o -lm -ldl
	g++ -shared -o liblib2.so lib2.o -lm -ldl
	g++ -L. -Wall -o main1.out main1.cpp -llib1 -ldl
	g++ -L. -Wall -o main2.out main1.cpp -llib2 -ldl
	g++ -Wall -o main.out main2.cpp -ldl

clean:
	rm *.o
	rm *.so
	rm *.out
