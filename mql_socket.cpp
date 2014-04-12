#define WIN32_LEAN_AND_MEAN  // Exclude rarely-used stuff from Windows headers
#include <winsock2.h>
#include <Ws2tcpip.h>
#include <stdio.h>

// Link with ws2_32.lib
#pragma comment(lib, "Ws2_32.lib")

//#include <windows.h>
//#include <winsock2.h>
#include <stdlib.h>
#include <stdio.h>

#define CONECTADO		1
#define DESCONECTADO	2
#define SOCKET_CONNECT_STATUS_ERROR			1000
#define HTONS(a) (((0xFF&a)<<8) + ((0xFF00&a)>>8))
#define CELL_SIZE  255

typedef struct _SOCKET_CLIENT{
	BYTE status;
	USHORT sequence;
	ULONG sock;
} SOCKET_CLIENT, *PSOCKET_CLIENT;

typedef struct _SOCKET_DATA{
	char symbol[8];
	double datetime;
	double bid;
	double ask;
} SOCKET_DATA, PSOCKET_DATA;

_SOCKET_CLIENT *client;
USHORT idx = 0;

ULONG __stdcall Host2Ip( char * host ){
	struct hostent * p;
	ULONG ret;
	p = gethostbyname(host);
	if (p){
		ret = *(ULONG*)(p->h_addr);
	}else{
		ret = INADDR_NONE;
	}
	return ret;
}


__declspec(dllexport) int __stdcall socket_connect( char *host, int port ){
	WORD wVersionRequested; 
	WSADATA wsaData; 
	int err; 
	wVersionRequested = MAKEWORD(1, 1); 
	 
	err = WSAStartup(wVersionRequested, &wsaData); 
	 
	if (err != 0)  return err; 
	 
	if ( LOBYTE( wsaData.wVersion ) != 1 || 
		 HIBYTE( wsaData.wVersion ) != 1 ) { 
		 //WSACleanup(); 
		 return __LINE__; 
	}

	client = new _SOCKET_CLIENT;

	client->status = DESCONECTADO;
	client->sequence = idx++;

	struct sockaddr_in addr;
	BOOL bOptVal = TRUE;
	int bOptLen = sizeof(BOOL);

	ULONG ip;
	ULONG sock = INVALID_SOCKET;
	
	ip = Host2Ip(host);

	if (ip != INADDR_NONE){
			addr.sin_addr.S_un.S_addr = ip;
			addr.sin_port = HTONS(port);
		if (addr.sin_addr.S_un.S_addr != INADDR_NONE){
			addr.sin_family = AF_INET;
			sock = (ULONG)socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
			if ( sock != INVALID_SOCKET ){
				int conn_status = connect(sock, (struct sockaddr *)&addr, sizeof(addr));
				if ( conn_status == SOCKET_ERROR ){
					printf("\tdll> err. @%s: status=%d wsa_err=%d\n",host,conn_status,WSAGetLastError());
					printf("\tdll> SOCKET_ERROR=%d\n",SOCKET_ERROR);
					closesocket(sock);
					sock = INVALID_SOCKET;
				}
			}
		}
	}

	client->sock = sock;

	if ( client->sock == INVALID_SOCKET ){
		closesocket( client->sock );
	}else{
		client->status = CONECTADO;
	}
	
	return( WSAGetLastError() );

}

__declspec(dllexport) int __stdcall socket_disconnect(){
	if (client->status == CONECTADO){
		closesocket(client->sock);
		client->status = DESCONECTADO;
	}
	//WSACleanup();
	return( WSAGetLastError() );
}

__declspec(dllexport) char* __stdcall socket_send_receive(char* buffer){
	if ( client->status == CONECTADO ){
		int t = strlen(buffer);
		if ( send( client->sock , buffer , t , 0) != t ){
			client->status = DESCONECTADO;
			closesocket(client->sock);
			return "erro, tamanho do bloco invalido.";
		}else{
			memset(buffer, 0, t);
			recv(client->sock, buffer, t, 0);
			return buffer;
		}
	}else{
		return "erro, desconectado.";
	}
}
