#include "easywsclient.hpp"
//#include "easywsclient.cpp" // <-- include only if you don't want compile separately
#ifdef _WIN32
#pragma comment( lib, "ws2_32" )
#include <WinSock2.h>
#endif
#include <assert.h>
#include <stdio.h>
#include <string>

using easywsclient::WebSocket;
static WebSocket::pointer ws = NULL;

void handle_message(const std::string & message)
{
	printf(">>> %s\n", message.c_str());
	if (message == "world") { ws->close(); }
}

class hudGyro {
	public:
		#ifdef _WIN32
				INT rc;
				WSADATA wsaData;
		#endif

		// Constructor
		//To connect to the websocket
				hudGyro() {
					printf("\nInitialising websocket -> hudGyro\n");
					#ifdef _WIN32
						rc = WSAStartup(MAKEWORD(2, 2), &wsaData);
						if (rc) {
							printf("WSAStartup Failed.\n");
						}
					#endif

					ws = WebSocket::from_url("ws://192.168.0.102:81/");
					assert(ws);
				}

};

int main()
{
	hudGyro hgyro;

	ws->send("#");
	//ws->send("hello");
	while (ws->getReadyState() != WebSocket::CLOSED) {
		ws->poll();
		ws->dispatch(handle_message);
	}
	delete ws;
#ifdef _WIN32
	WSACleanup();
#endif
	return 0;
}
