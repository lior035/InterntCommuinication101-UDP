#include <iostream>
using namespace std;
// Don't forget to include "Ws2_32.lib" in the library list.
#include <winsock2.h>
#include <string.h>
#include <time.h>
#include "serverAction.h"


#define SMARTSERVER_PORT	27015

void main() 
{
	
	//open and check the workout inviroment for sockets
	WSAData wsaData; 
	
	if (NO_ERROR != WSAStartup(MAKEWORD(2,2), &wsaData))
	{
        cout<<"Smart Server: Error at WSAStartup()\n";
	}

	//create socket and check if succeded
    SOCKET m_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

	if (INVALID_SOCKET == m_socket)
	{
        cout<<"Time Server: Error at socket(): "<<WSAGetLastError()<<endl;
        WSACleanup();
        return;
	}


	//bind procecc - create address for server (port and ip) and bind it to he socket created 
	sockaddr_in serverService;
    serverService.sin_family = AF_INET; 
	serverService.sin_addr.s_addr = INADDR_ANY;	//inet_addr("127.0.0.1");
	serverService.sin_port = htons(SMARTSERVER_PORT);


	//binding and checking for errors
	if (SOCKET_ERROR == bind(m_socket, (SOCKADDR *) &serverService, sizeof(serverService))) 
	{
		cout<<"Time Server: Error at bind(): "<<WSAGetLastError()<<endl;
        closesocket(m_socket);
		WSACleanup();
        exit(1);
    }

	// Send and receive data.
	sockaddr client_addr;
	int client_addr_len = sizeof(client_addr);
	int bytesSent = 0;
    int bytesRecv = 0;
	char * answar = new char [1000];
	int * recvBuff;

	ServerAction sa;

	cout<<"Smart Server: Wait for clients' requests.\n";	
	
	while(true)
	{
		recvBuff = new int;
		bytesRecv = recvfrom(m_socket, (char*)recvBuff, 255, 0, &client_addr, &client_addr_len);
		cout<<"Received from"<<inet_addr(client_addr.sa_data)<<endl;
		if (SOCKET_ERROR ==bytesRecv)
		{
			cout<<"Smart Server: Error at recvfrom(): "<<WSAGetLastError()<<endl;
			closesocket(m_socket);
			WSACleanup();
			exit(1);
		}

		int num = *recvBuff;

		if ((num == GETTIME)||(num==GETTIMEWITHOUTDATE)||(num==GETTIMESINCEEPOCH)||(num==GETDAYANDMONTH)||(num==GETYEAR)||(num == GETSECONDSSINCEBEGININGOFYEAR))
		{
			sa.takeAction(answar,num);	
			bytesSent = sendto(m_socket,(const char*)answar,strlen(answar),0,(const sockaddr *)&client_addr,client_addr_len);

			if (SOCKET_ERROR == bytesSent)
			{
				cout<<"Smart Server: Error at sendto(): "<<WSAGetLastError()<<endl;
				closesocket(m_socket);
				WSACleanup();
				exit(1);
			}
		}

		if ((num == GETCLIENTTOSERVERDELAYESTIMATION)||(num==MEASURERTT))
			sa.takeAction(answar,num,m_socket,&client_addr);
	}

	// Closing connections and Winsock.
	cout<<"Time Server: Closing Connection.\n";
	closesocket(m_socket);
	WSACleanup();
}
