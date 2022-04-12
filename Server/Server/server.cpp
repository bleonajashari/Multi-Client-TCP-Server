//Disable inet_addr warnings/error
#pragma warning(disable:4996) 

#pragma comment(lib,"ws2_32.lib")
#include <WinSock2.h>
#include <string>
#include <iostream>

SOCKET Connections[100];
int ConnectionCounter = 0;

void ClientHandlerThread(int index) //index = the index in the SOCKET Connections array
{
	char buffer[256]; //Buffer to receive and send out messages from/to the clients
	while (true)
	{
		recv(Connections[index], buffer, sizeof(buffer), NULL); //get message from client

		std::string ClientName = "Client > ";
		std::string Format = buffer;
		std::string Final = ClientName + Format;


		std::cout << Final.c_str() << std::endl;//Print what the client said

		for (int i = 0; i < ConnectionCounter; i++) //For each client connection
		{
			if (i == index) //Don't send the chat message to the same user who sent it
				continue; //Skip user
			send(Connections[i], buffer, sizeof(buffer), NULL);//send the chat message to this client
		}
	}
}

int main()
{
	//Winsock Startup
	WSAData wsaData;
	WORD DllVersion = MAKEWORD(2, 1);
	if (WSAStartup(DllVersion, &wsaData) != 0) //If WSAStartup returns anything other than 0, then that means an error has occured in the WinSock Startup.
	{
		MessageBoxA(NULL, "WinSock startup failed", "Error", MB_OK | MB_ICONERROR);
		return 0;
	}

	SOCKADDR_IN addr; //Address that we will bind our listening socket to
	int addrlen = sizeof(addr); //length of the address (required for accept call)
	addr.sin_addr.s_addr = inet_addr("127.0.0.1"); //Broadcast locally
	addr.sin_port = htons(1111); //Port
	addr.sin_family = AF_INET; //IPv4 Socket

	SOCKET sListen = socket(AF_INET, SOCK_STREAM, NULL); //Create socket to listen for new connections
	bind(sListen, (SOCKADDR*)&addr, sizeof(addr)); //Bind the address to the socket
	listen(sListen, SOMAXCONN); //Places sListen socket in a state in which it is listening for an incoming connection. Note:SOMAXCONN = Socket Oustanding Max Connections

	SOCKET newConnection; //Socket to hold the client's connection
	int ConnectionCounter = 0; //# of client connections
	for (int i = 0; i < 100; i++)
	{
		newConnection = accept(sListen, (SOCKADDR*)&addr, &addrlen); //Accept a new connection
		if (newConnection == 0) //If accepting the client connection failed
		{
			std::cout << "Failed to accept the client's connection." << std::endl;
		}
		else //If client connection properly accepted
		{
			std::cout << "Client Connected" << std::endl;
			char MOTD[256] = "This is a message sent from the server"; //Create buffer with message of the day
			send(newConnection, MOTD, sizeof(MOTD), NULL); //Send MOTD buffer
			Connections[i] = newConnection; //Set socket in array to be the newest connection before creating the thread to handle this client's socket.
			ConnectionCounter += 1; //Incremenent total # of clients that have connected
			CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)ClientHandlerThread, (LPVOID)(i), NULL, NULL); //Create Thread to handle this client. The index in the socket array for this thread is the value (i).
		}
	}

	system("pause");
	return 0;
}
