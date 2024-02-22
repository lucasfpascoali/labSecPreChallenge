
challenge1.o: challenge1.cpp
	g++ -o challenge1.o challenge1.cpp 

challenge2.o: challenge2.cpp
	g++ -o challenge2.o challenge2.cpp

challenge3.o: challenge3.cpp
	g++ -o challenge3.o challenge3.cpp

challenge4.o: challenge4.cpp
	g++ -o challenge4.o challenge4.cpp

challenge5.o: challenge5.cpp
	g++ -o challenge5.o challenge5.cpp -w

challenge6.o: challenge6.cpp
	g++ -o challenge6.o challenge6.cpp -w

challenge1: challenge1.o
	./challenge1.o 49276d206b696c6c696e6720796f757220627261696e206c696b65206120706f69736f6e6f7573206d757368726f6f6d

challenge2: challenge2.o
	./challenge2.o 1c0111001f010100061a024b53535009181c 686974207468652062756c6c277320657965

challenge3: challenge3.o
	./challenge3.o 1b37373331363f78151b7f2b783431333d78397828372d363c78373e783a393b3736

challenge4: challenge4.o
	./challenge4.o

challenge5: challenge5.o
	./challenge5.o

challenge6: challenge6.o
	./challenge6.o