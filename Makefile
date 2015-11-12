threads: threads_test.cpp worker_thread.hpp
	g++ -lboost_thread -o threads threads_test.cpp 

postgres_test:	
	g++ libpq_test.cpp -o client -lpqxx -lpq

clean:
	rm -fr client threads