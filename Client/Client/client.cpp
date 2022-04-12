//Disable inet_addr warnings/error
#pragma warning(disable:4996) 

#pragma comment(lib,"ws2_32.lib") //Required for WinSock
#include <WinSock2.h> //For win sockets
#include <string> 
#include <iostream> 

SOCKET Connection;//This client's connection to the server


void ClientThread()
{
	char buffer[256]; //Create buffer to hold messages up to 256 characters
	while (true)
	{
		recv(Connection, buffer, sizeof(buffer), NULL); //receive buffer
		std::cout << buffer << std::endl;
	}
}

int main()
{
	//Winsock Startup
	WSAData wsaData;
	WORD DllVersion = MAKEWORD(2, 1);
	if (WSAStartup(DllVersion, &wsaData) != 0)
	{
		MessageBoxA(NULL, "Winsock startup failed", "Error", MB_OK | MB_ICONERROR);
		return 0;
	}

	SOCKADDR_IN addr; //Address to be binded to our Connection socket
	int sizeofaddr = sizeof(addr); //Need sizeofaddr for the connect function
	addr.sin_addr.s_addr = inet_addr("127.0.0.1"); //Address = localhost (this pc)
	addr.sin_port = htons(1111); //Port = 1111
	addr.sin_family = AF_INET; //IPv4 Socket

	Connection = socket(AF_INET, SOCK_STREAM, NULL); //Set Connection socket
	if (connect(Connection, (SOCKADDR*)&addr, sizeofaddr) != 0) //If we are unable to connect...
	{
		MessageBoxA(NULL, "Failed to Connect", "Error", MB_OK | MB_ICONERROR);
		return 0; //Failed to Connect
	}

	std::cout << "Connected to server: 127.0.0.1" << std::endl;
	CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)ClientThread, NULL, NULL, NULL); //Create the client thread that will receive any data that the server sends.

	char buffer[256] = "test"; //256 char buffer to send message
	while (true)
	{
		std::cin.getline(buffer, sizeof(buffer)); //Get line if user presses enter and fill the buffer
		send(Connection, buffer, sizeof(buffer), NULL); //Send buffer
		Sleep(10);
	}
	return 0;
}