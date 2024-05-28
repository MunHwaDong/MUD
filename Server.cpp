#include "Server.h"

Server::Server()
{
	//최대 10개의 클라이언트 수용
	//clientsSockets.reserve(maxClientNum);
	socketCount = 0;

	WSADATA wsadata;
	SOCKADDR_IN serverAddr;

	try
	{
		WSAStartup(MAKEWORD(2, 2), &wsadata);
	}
	catch (const std::exception& e)
	{
		std::cout << e.what() << std::endl;
		WSACleanup();
		return;
	}

	try
	{
		this->serverSocket.sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	}
	catch (const std::exception& e)
	{
		std::cout << e.what() << std::endl;
		WSACleanup();
		return;
	}

	memset(&serverAddr, 0, sizeof(serverAddr));

	serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(PORT_NUMBER);

	try
	{
		bind(this->serverSocket.sock, reinterpret_cast<sockaddr*>(&serverAddr), sizeof(serverAddr));
	}
	catch (const std::exception& e)
	{
		std::cout << e.what() << std::endl;
		WSACleanup();
		return;
	}

	try
	{
		listen(this->serverSocket.sock, SOMAXCONN);
	}
	catch (const std::exception& e)
	{
		std::cout << e.what() << std::endl;
		WSACleanup();
		return;
	}

}

void Server::CloseServer()
{
	for (auto& socket : clientsSockets)
	{
		closesocket(socket.sock);
		WSACloseEvent(socket.ev);
	}
}

int Server::InitServer()
{
	return 0;
}

int Server::AddClient(int idx)
{
	SOCKADDR_IN addr;
	int len = 0;
	SOCKET acceptSocket;

	if (socketCount == FD_SETSIZE)
	{
		std::cout << "Server is too BUSY!!! contact to Developer!!!" << std::endl;
		return -1;
	}
	else
	{
		len = sizeof(addr);
		memset(&addr, 0, sizeof(addr));
		acceptSocket = accept(serverSocket.sock, reinterpret_cast<SOCKADDR*>(&addr), &len);

		char buffer[INET_ADDRSTRLEN];
		inet_ntop(AF_INET, &(addr.sin_addr), buffer, INET_ADDRSTRLEN);
		string ipAddr = buffer;

		HANDLE eve = WSACreateEvent();

		SocketInfo newSocket{
								acceptSocket,
								eve,
								ipAddr };


		clientsSockets[socketCount] = newSocket;

		WSAEventSelect(acceptSocket, eve, FD_READ | FD_CLOSE);

		++socketCount;
		
		std::cout << "새로운 클라이언트 접속 (IP : " << ipAddr << ")" << std::endl;

	}

	return 0;
}

void Server::RemoveClient(int idx)
{

	std::cout << "[" + clientsSockets[idx].nickName + " 이 게임에서 이탈하셨습니다!]" << std::endl;

	closesocket(clientsSockets[idx].sock);
	WSACloseEvent(clientsSockets[idx].ev);

	--socketCount;
	clientsSockets[idx].sock = clientsSockets[socketCount].sock;
	clientsSockets[idx].ev = clientsSockets[socketCount].ev;
	clientsSockets[idx].ipAddr.assign(clientsSockets[socketCount].ipAddr);
	clientsSockets[idx].nickName.assign(clientsSockets[socketCount].nickName);

}

string Server::GetClientIP(int idx)
{
	sockaddr_in sock;	
	int addrLen = sizeof(sock);

	char buffer[INET_ADDRSTRLEN];
	inet_ntop(AF_INET, &(sock.sin_addr), buffer, INET_ADDRSTRLEN);
	string ipAddr = buffer;

	if (getpeername(clientsSockets[idx].sock, reinterpret_cast<sockaddr*>(&sock), &addrLen) < 0)
		return nullptr;

	return ipAddr;

}