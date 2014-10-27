#include <iostream>
using namespace std;

#include <winsock2.h>
#include <string.h>
#include <time.h>

#define GETTIME 1
#define GETTIMEWITHOUTDATE 2
#define GETTIMESINCEEPOCH 3
#define GETCLIENTTOSERVERDELAYESTIMATION 4 
#define MEASURERTT 5
#define GETDAYANDMONTH 6
#define GETYEAR 7
#define GETSECONDSSINCEBEGININGOFYEAR 8

#define TIME_PORT	27015

void printManu();
void handlerInput(int input,sockaddr_in serverAddress,SOCKET clientSocket);

void main() 
{
	// Initialize Winsock (Windows Sockets).

	WSAData wsaData; 
	if (NO_ERROR != WSAStartup(MAKEWORD(2,2), &wsaData))
	{
		cout<<"Time Client: Error at WSAStartup()\n";
	}

	// Client side:
	// Create a socket and connect to an internet address.

    SOCKET connSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (INVALID_SOCKET == connSocket)
	{
        cout<<"Time Client: Error at socket(): "<<WSAGetLastError()<<endl;
        WSACleanup();
        return;
	}

	 // Create a sockaddr_in object called server. 
	sockaddr_in server;
    server.sin_family = AF_INET; 
	server.sin_addr.s_addr = inet_addr("80.230.70.213");
	server.sin_port = htons(27015);

	printManu();

	int input;
	cin>>input;

	handlerInput(input, server, connSocket);
	
	// Closing connections and Winsock.
	cout<<"Time client: Closing Connection.\n";
	closesocket(connSocket);
	WSACleanup();
}

void printManu()
{
	cout<<"Enter Option to accure:"<<endl;
	cout<<"Press 1 to get time in the format of - year/monthe/day/hour/miunute/seconde"<<endl;
	cout<<"Press 2 to get time in the format of - hour/minute/seconde"<<endl;
	cout<<"Press 3 to get time since Epoch in the format of seconde"<<endl;
	cout<<"Press 4 to get client to server delay estitmation"<<endl;
	cout<<"Press 5 to masure RTT"<<endl;
	cout<<"Press 6 to get day and monthe"<<endl;
	cout<<"Press 7 to get the year"<<endl;
	cout<<"Press 8 to get seconde since begining of this year"<<endl;
}

void handlerInput(int input,sockaddr_in serverAddress,SOCKET clientSocket)
{
	int sentByte, recvByte;

	char*buff = new char [1000];

	if ((input == GETTIME)||(input==GETTIMEWITHOUTDATE)||(input==GETTIMESINCEEPOCH)||(input==GETDAYANDMONTH)||(input==GETYEAR)||(input == GETSECONDSSINCEBEGININGOFYEAR))
	{
		sentByte = sendto(clientSocket,(const char*)&input,sizeof(int),0,(const sockaddr*) &serverAddress, sizeof( serverAddress));
			
		if (SOCKET_ERROR ==sentByte)
		{
			cout<<"Error at sendto(): "<<WSAGetLastError()<<endl;
			closesocket(clientSocket);
			WSACleanup();
			exit(1);
		}

		int bu = sizeof(serverAddress);
		recvByte = recvfrom(clientSocket,buff,(int)strlen(buff),0, (sockaddr*)&serverAddress, &bu);

		buff[recvByte] = '\0';

		if (SOCKET_ERROR ==recvByte)
		{
			cout<<"Error at sendto(): "<<WSAGetLastError()<<endl;
			closesocket(clientSocket);
			WSACleanup();
			exit(1);
		}

		cout<<buff<<endl;
		delete buff;
	}

	else if (input == GETCLIENTTOSERVERDELAYESTIMATION)
	{
		int i;
		double tTime;

		double start, end;

		start = (double)GetTickCount();

		//control flow for the first request
		sentByte = sendto(clientSocket,(const char*)&input,sizeof(int),0,(const sockaddr*) &serverAddress, sizeof( serverAddress));
			
		if (SOCKET_ERROR ==sentByte)
		{
			cout<<"Error at sendto(): "<<WSAGetLastError()<<endl;
			closesocket(clientSocket);
			WSACleanup();
			exit(1);
		}

		for (i=0; i<100; i++)
		{
			sentByte = sendto(clientSocket,(const char*)&input,sizeof(int),0,(const sockaddr*) &serverAddress, sizeof( serverAddress));
			
			if (SOCKET_ERROR ==sentByte)
			{
				cout<<"Error at sendto(): "<<WSAGetLastError()<<endl;
				closesocket(clientSocket);
				WSACleanup();
				exit(1);
			}
		}

		int bu = sizeof(serverAddress);

		for (i=0; i<100; i++)
		{
			recvByte = recvfrom(clientSocket, buff, (int)strlen(buff),0,(sockaddr*)&serverAddress, &bu);
			if (SOCKET_ERROR ==recvByte)
			{
				cout<<"Error at sendto(): "<<WSAGetLastError()<<endl;
				closesocket(clientSocket);
				WSACleanup();
				exit(1);
			}
		}

		end = (double)GetTickCount();

		tTime =(end-start)/100;

		cout<<"Delay Estimation : "<<tTime<<endl;
		delete buff;
	}

	else if (input ==MEASURERTT)
	{
		int i;
		double tTime;

		double * timer = new double [100];

		int bu = sizeof(serverAddress);

		//control flow
		sentByte = sendto(clientSocket,(const char*)&input,sizeof(int),0,(const sockaddr*) &serverAddress, sizeof( serverAddress));

		if (SOCKET_ERROR ==sentByte)
		{
			cout<<"Error at sendto(): "<<WSAGetLastError()<<endl;
			closesocket(clientSocket);
			WSACleanup();
			exit(1);
		}

		for (i=0;i<100;i++)
		{
			sentByte = sendto(clientSocket,(const char*)&input,sizeof(int),0,(const sockaddr*) &serverAddress, sizeof( serverAddress));

			if (SOCKET_ERROR ==sentByte)
			{
				cout<<"Error at sendto(): "<<WSAGetLastError()<<endl;
				closesocket(clientSocket);
				WSACleanup();
				exit(1);
			}

			recvByte = recvfrom(clientSocket,buff,(int)strlen(buff),0,(sockaddr*)&serverAddress, &bu);

			if (SOCKET_ERROR ==recvByte)
			{
				cout<<"Error at sendto(): "<<WSAGetLastError()<<endl;
				closesocket(clientSocket);
				WSACleanup();
				exit(1);
			}

			timer[i] =  (double)GetTickCount();
		}

		delete buff;

		double total =0;

		for(i=0; i<100;i++)
			total+= timer[i];

		delete timer;

		double rtt;

		rtt = total/100; 

		cout<<"RTT : "<<rtt<<endl;
	}

	else
		cout<<"invalid request"<<endl;
}