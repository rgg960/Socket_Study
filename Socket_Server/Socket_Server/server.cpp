
#pragma comment(lib, "ws2_32")

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ws2tcpip.h>

#define MESSAGE_SIZE 1024

void ErrorHandling(char * message);

int main(int argc, char *argv[]) {

	WSADATA wsaData; // Windows 최적화
	SOCKET serv_sock, clnt_sock;
	SOCKADDR_IN serv_addr, clnt_addr;

	int SclntAddr;
	char message[MESSAGE_SIZE];
	int msl;

	if (argc != 2) {
		printf("Usage : %s <port>\n", argv[0]);
		exit(1);
	}

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) // MAKEWORD : SOCKET 버전확인
		ErrorHandling( (char*) "WSAStartup error!" );

	serv_sock = socket(PF_INET, SOCK_STREAM, 0);
	if ( serv_sock == INVALID_SOCKET )
		ErrorHandling((char*) "socket() error!");

	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY); // INADDR_ANY Cpu RAN addr
	serv_addr.sin_port = htons(atoi(argv[1]));

	if ( bind(serv_sock, (SOCKADDR*)&serv_addr, sizeof(serv_addr)) == SOCKET_ERROR ) 
		ErrorHandling((char*) "bind() error!");

	if ( listen(serv_sock,5) == SOCKET_ERROR )
		ErrorHandling((char*) "lisetn() error!");


	SclntAddr = sizeof(clnt_addr);
	clnt_sock = accept(serv_sock, (SOCKADDR*)&clnt_addr, &SclntAddr);
	if ( clnt_sock == INVALID_SOCKET )
		ErrorHandling((char*) "accept() error!");

	while (1) {
		memset(message, '\0', MESSAGE_SIZE);
		msl = recv(clnt_sock, message, sizeof(message) - 1, 0);

		if ( msl == -1 )
			ErrorHandling((char*) "recv() error!");
		printf("Message from client : %s \n", message);
		printf("메시지 입력 >> ");
		gets_s(message);
		send(clnt_sock, message, strlen(message) + 1, 0);

	}

	closesocket(serv_sock);
	closesocket(clnt_sock);
	WSACleanup();

	return 0;
}

void ErrorHandling(char * message) {
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}