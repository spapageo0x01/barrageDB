barrage: barrage_db.cpp db_metadata.cpp db_metadata.hpp db_ops.cpp db_ops.hpp
	g++ -std=c++11 -lpqxx -lpq -lboost_thread -o barrage db_metadata.cpp worker_thread.cpp db_ops.cpp barrage_db.cpp

threads: threads_test.cpp worker_thread.hpp worker_thread.cpp
	g++ -lboost_thread -o threads worker_thread.cpp threads_test.cpp 

postgres_test:	
	g++ libpq_test.cpp -o client -lpqxx -lpq

clean:
	rm -fr client threads barrage
