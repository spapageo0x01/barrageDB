all:	
	g++ libpq_test.cpp -o client -lpqxx -lpq

clean:
	rm client