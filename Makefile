all:
	g++ -pthread -std=c++11 -o sendServer serverFolder/sendServer.cpp
	g++ -std=c++11 -o sendClient clientFolder/sendClient.cpp
	g++ -std=c++11 -o recvServer serverFolder/recvServer.cpp
	g++ -std=c++11 -o recvClient clientFolder/recvClient.cpp
clean:
	rm sendServer sendClient recvClient recvServer
