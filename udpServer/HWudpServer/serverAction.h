#ifndef SERVERACTION
#define SERVERACTION

#include <winsock2.h>
#include <windows.h>

#define GETTIME 1
#define GETTIMEWITHOUTDATE 2
#define GETTIMESINCEEPOCH 3
#define GETCLIENTTOSERVERDELAYESTIMATION 4 
#define MEASURERTT 5
#define GETDAYANDMONTH 6
#define GETYEAR 7
#define GETSECONDSSINCEBEGININGOFYEAR 8

class ServerAction
{

public:

	//using defult constructor

	void takeAction(char * sendMsg,  int action, SOCKET s = NULL, sockaddr* client_addr = NULL);
	char* getTime();
	char* getTimeWithoutDate();
	char* getTimeSinceEpoch();
	void getClientToServerDelayEstimation(SOCKET server, sockaddr client_addr);
	void measureRtt(SOCKET server, sockaddr client_addr);
	char* getDayAndMonth();
	char* getYear();
	char* getSecondSinceBeginigOfTheYear();
};

#endif