#include "ChatServer.h"

void ChatServer::RecvAndBroadcast(int index)
{
	SOCKADDR_IN client_address;
	char message[MAXBYTE], broadcast_message[MAXBYTE];
	int recv_len = 0;
	int addr_len = 0;
	char* token = nullptr;
	char* next_token = nullptr;

	memset(&client_address, 0, sizeof(client_address));

	if (recv_len = recv(clientsSockets[index].sock, message, MAXBYTE, 0))
	{
		addr_len = sizeof(client_address);
		getpeername(clientsSockets[index].sock, (SOCKADDR*)&client_address, &addr_len);
		strcpy_s(broadcast_message, message);

		if (clientsSockets[index].nickName.length() <= 0)
		{
			token = strtok_s(message, "]", &next_token);
			clientsSockets[index].nickName = token + 1;
		}
		for (int i = 1; i < socketCount; i++)
			send(clientsSockets[i].sock, broadcast_message, MAXBYTE, 0);
	}
}

int ChatServer::ReadClient(int idx)
{
    thread readThread { &ChatServer::RecvAndBroadcast, this, idx };
    readThread.join();

	return 0;
}

void ChatServer::ChatService()
{
    //SOCKET  server_socket;
    WSANETWORKEVENTS ev;
    int index;
    int client_count = 10;
    WSAEVENT handle_array[11];

    //server_socket = server_init();

    if (serverSocket.sock < 0)
    {
        printf("초기화 에러\n");
        exit(0);
    }
    else
    {
        HANDLE event = WSACreateEvent();

        /*SocketInfo* sockInfo = new SocketInfo();
        clientsSockets.push_back(sockInfo);*/

        clientsSockets[socketCount].ev = event;
        clientsSockets[socketCount].sock = serverSocket.sock;
        clientsSockets[socketCount].nickName = "svr";
        clientsSockets[socketCount].ipAddr = "0.0.0.0";

        WSAEventSelect(serverSocket.sock, event, FD_ACCEPT);
        socketCount++;

        printf("\n >> 서버 초기화가 완료되었습니다.(포트번호:%d)\n", PORT_NUMBER);

        while (true)
        {
            memset(&handle_array, 0, sizeof(handle_array));

            for (int i = 0; i < socketCount; i++)
                handle_array[i] = clientsSockets[i].ev;

            index = WSAWaitForMultipleEvents(socketCount,
                handle_array, false, INFINITE, false);

            if ((index != WSA_WAIT_FAILED) && (index != WSA_WAIT_TIMEOUT))
            {
                WSAEnumNetworkEvents(clientsSockets[index].sock, clientsSockets[index].ev, &ev);
                if (ev.lNetworkEvents == FD_ACCEPT)
                {
                    this->AddClient(index);

                    char msg[256];
                    sprintf_s(msg, " >> 신규 클라이언트 접속(IP : %s)\n", clientsSockets[socketCount - 1].ipAddr.c_str());
                    NotifyClient(msg);
                }

                else if (ev.lNetworkEvents == FD_READ)
                    this->ReadClient(index);

                else if (ev.lNetworkEvents == FD_CLOSE)
                    this->RemoveClient(index);
            }
        }
        closesocket(serverSocket.sock);
    }

    WSACleanup();
}

int ChatServer::NotifyClient(char* message)
{
	for (int i = 1; i < socketCount; i++)
		send(clientsSockets[i].sock, message, MAXBYTE, 0);

	return 0;
}