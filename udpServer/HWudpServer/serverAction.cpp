#include <iostream>
using namespace std;
#include "serverAction.h"
#include <time.h>

void ServerAction::takeAction(char * sendMsg,  int action, SOCKET s, sockaddr* client_addr)
{
	switch (action)
	{
	case GETTIME:
		{
			strcpy(sendMsg, (const char *)(getTime()));
			break;
		}

	case GETTIMEWITHOUTDATE:
		{
			strcpy(sendMsg, (const char *) getTimeWithoutDate());
			break;
		}

	case GETTIMESINCEEPOCH:
		{
			strcpy(sendMsg, (const char *)getTimeSinceEpoch());
			break;
		}

	case GETCLIENTTOSERVERDELAYESTIMATION:
		{
			getClientToServerDelayEstimation(s,*client_addr);
			break;
		}

	case MEASURERTT:
		{
			measureRtt(s,*client_addr);
			break;
		}

	case GETDAYANDMONTH:
		{
			strcpy(sendMsg, (const char *)getDayAndMonth());
			break;
		}

	case GETYEAR:
		{
			strcpy(sendMsg, (const char *) getYear());
			break;
		}

	case GETSECONDSSINCEBEGININGOFYEAR:
		{
			strcpy(sendMsg, (const char *) getSecondSinceBeginigOfTheYear());
			break;
		}

	default:
		break;
	}
}

//=========================================================================================

char* ServerAction::getTime()
{
	// Get the current time.
	time_t timer;
	time(&timer);

	// Parse the current time to printable string.
	char * msg;
	msg = _strdup((const char*) ctime(&timer));
	
	int i = 0;

	while (msg[i]!= '\n')
		i++;
	msg[i] = NULL;
	return msg;
}


char* ServerAction::getTimeWithoutDate()
{
	const time_t curr = time(NULL);
	const tm* local = localtime(&curr);

	char * msg = new char [255];
	char * msgExact;

	strftime(msg,255,"%X",local);
	msgExact = _strdup((const char*) msg);
	delete msg;
	return msgExact;
}


char* ServerAction::getTimeSinceEpoch()
{

	time_t seconds;
    seconds = time (NULL);

	int num = (int)seconds;
	char * msg = new char [sizeof(num)+1];

	itoa(num,msg,10);
	
	return msg;
}

void ServerAction::getClientToServerDelayEstimation(SOCKET server, sockaddr client_addr)
{
	int i;
	int * met = new int;
	int client_addr_len = sizeof(client_addr);
	int bytesRecv,bytesSent;

	for (i=0;i<100; i++) 
	{
		bytesRecv = recvfrom(server,(char*)met,255,0,&client_addr,&client_addr_len);
		
		if (SOCKET_ERROR ==bytesRecv)
		{
			cout<<"Smart Server: Error at recvfrom(): "<<WSAGetLastError()<<endl;
			closesocket(server);
			WSACleanup();
			exit(1);
		}
	}

	for (i=0;i<100;i++)
	{
		char* currentTime = getTime();
		bytesSent = sendto(server, (const char *) currentTime,(int)strlen(currentTime),0,(const sockaddr *)&client_addr, client_addr_len);

		if (SOCKET_ERROR ==bytesSent)
		{
			cout<<"Smart Server: Error at recvfrom(): "<<WSAGetLastError()<<endl;
			closesocket(server);
			WSACleanup();
			exit(1);
		}
	}
}

void ServerAction::measureRtt(SOCKET server, sockaddr client_addr)
{
	int i;
	int * met = new int;
	int client_addr_len = sizeof(client_addr);
	int bytesRecv,bytesSent;

	for (i=0;i<100;i++)
	{
		bytesRecv = recvfrom(server,(char*)met,255,0,&client_addr,&client_addr_len);
	
		if (SOCKET_ERROR ==bytesRecv)
		{
			cout<<"Smart Server: Error at recvfrom(): "<<WSAGetLastError()<<endl;
			closesocket(server);
			WSACleanup();
			exit(1);
		}

		char* currentTime = getTime();
		bytesSent = sendto(server, (const char *) currentTime,(int)strlen(currentTime),0,(const sockaddr *)&client_addr, client_addr_len);

		if (SOCKET_ERROR ==bytesSent)
		{
			cout<<"Smart Server: Error at recvfrom(): "<<WSAGetLastError()<<endl;
			closesocket(server);
			WSACleanup();
			exit(1);
		}
	}
}

char* ServerAction::getDayAndMonth()
{
	const time_t curr = time(NULL);
	const tm* local = localtime(&curr);

	char * month = new char [255];
	char * day = new char [255];

	char * monthExact;
	char * dayExact;

	char * msgExact;

	strftime(month,255,"%B",local);
	strftime(day ,255,"%A",local);

	monthExact = _strdup((const char*) month);
	dayExact = _strdup((const char*) day) ;
	delete month;
	delete day;

	int size = strlen(monthExact)+strlen(dayExact)+2;
	msgExact = new char [size];
	int i = 0;

	while (i<size)
	{
		for (i = 0; i<(int)strlen(monthExact);i++)
			msgExact[i] = monthExact[i];
		msgExact[i++] = ' ';

		for (int j =0; j<(int)strlen(dayExact); j++)
			msgExact[i++] = dayExact[j];
		msgExact[i++] = '\0';
	}

	return msgExact;
}

char* ServerAction::getYear()
{
	const time_t curr = time(NULL);
	const tm* local = localtime(&curr);

	char * msg = new char [255];
	char * msgExact;

	strftime(msg,255,"%Y",local);
	msgExact = _strdup((const char*) msg);
	delete msg;
	return msgExact;
}

char* ServerAction:: getSecondSinceBeginigOfTheYear()
{
	time_t now =time(NULL);
    tm* tmp = localtime(&now);
    tm t = {0};
    t.tm_year = tmp->tm_year;
	char* msg = new char [250];
    int num = int(difftime(now, mktime(&t)));
	itoa(num,msg,10);
	char* sec;
	sec = _strdup((const char*) msg) ;
	delete msg;
	return sec;
}