P2.out : Project2.o Set_Limits.o Lexical_Analyzer.o Syntactical_Analyzer.o
	g++ -g -o P2.out Project2.o Set_Limits.o Lexical_Analyzer.o Syntactical_Analyzer.o

Project2.o : Project2.cpp Set_Limits.h Syntactical_Analyzer.h
	g++ -g -c Project2.cpp

Set_Limits.o : Set_Limits.cpp Set_Limits.h
	g++ -g -c Set_Limits.cpp

Lexical_Analyzer.o : Lexical_Analyzer.o.save
	cp Lexical_Analyzer.o.save Lexical_Analyzer.o

Syntactical_Analyzer.o : Syntactical_Analyzer.cpp Syntactical_Analyzer.h Lexical_Analyzer.h
	g++ -g -c Syntactical_Analyzer.cpp

clean : 
	rm *.o P2.out *.gch

submit : Project2.cpp Lexical_Analyzer.h Syntactical_Analyzer.h Syntactical_Analyzer.cpp makefile README.txt
	rm -rf lankfordP2
	mkdir lankfordP2
	cp Project2.cpp lankfordP2
	cp Syntactical_Analyzer.h lankfordP2
	cp Syntactical_Analyzer.cpp lankfordP2
	cp makefile lankfordP2
	cp README.txt lankfordP2
	tar cfvz lankfordP2.tgz lankfordP2
	cp lankfordP2.tgz ~tiawatts/cs460drop
