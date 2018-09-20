all:
	g++ -pthread -o sendServer serverFolder/sendServer.cpp
	g++ -o sendClient clientFolder/sendClient.cpp

clean:
	rm sendServer sendClient
