#include <stdio.h>
#include <string.h>
#include <process.h>
#include <winsock2.h>
#include <wincrypt.h>

#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "crypt32.lib")

#define PORT 1337
#define BUFFER_SIZE 1024

void get_md5(const char* data, char* ret_str) {
	HCRYPTPROV hProv;
	HCRYPTHASH hHash;
	DWORD cbHash = 16;
	BYTE hash[16];

	CryptAcquireContext(&hProv, NULL, NULL, PROV_RSA_FULL, 0);
	CryptCreateHash(hProv, CALG_MD5, 0, 0, &hHash);
	CryptHashData(hHash, (BYTE*)data, strlen(data), 0);
	CryptGetHashParam(hHash, HP_HASHVAL, hash, &cbHash, 0);

	CryptDestroyHash(hHash);
	CryptReleaseContext(hProv, 0);

	for (int i = 0; i < 16; ++i) {
		sprintf(ret_str + i * 2, "%02x", hash[i]);
	}
	ret_str[32] = '\0';
}

void get_sha1(const char* data, char* ret_str) {
	HCRYPTPROV hProv;
	HCRYPTHASH hHash;
	DWORD cbHash = 20;
	BYTE hash[20];

	CryptAcquireContext(&hProv, NULL, NULL, PROV_RSA_FULL, 0);
	CryptCreateHash(hProv, CALG_SHA1, 0, 0, &hHash);
	CryptHashData(hHash, (BYTE*)data, strlen(data), 0);
	CryptGetHashParam(hHash, HP_HASHVAL, hash, &cbHash, 0);

	CryptDestroyHash(hHash);
	CryptReleaseContext(hProv, 0);

	for (int i = 0; i < 20; ++i) {
		sprintf(ret_str + i * 2, "%02x", hash[i]);
	}
	ret_str[40] = '\0';
}

void get_sha256(const char* data, char* ret_str) {
	HCRYPTPROV hProv;
	HCRYPTHASH hHash;
	DWORD cbHash = 32;
	BYTE hash[32];

	CryptAcquireContext(&hProv, NULL, NULL, PROV_RSA_AES, CRYPT_VERIFYCONTEXT);
	CryptCreateHash(hProv, CALG_SHA_256, 0, 0, &hHash);
	CryptHashData(hHash, (BYTE*)data, strlen(data), 0);
	CryptGetHashParam(hHash, HP_HASHVAL, hash, &cbHash, 0);

	CryptDestroyHash(hHash);
	CryptReleaseContext(hProv, 0);

	for (int i = 0; i < 32; ++i) {
		sprintf(ret_str + i * 2, "%02x", hash[i]);
	}
	ret_str[64] = '\0';
}

int __stdcall HandleClient(SOCKET* clientSocket) {
	SOCKET clientConn = *clientSocket;
	char buffer[BUFFER_SIZE];
	int bytesReceived;

	for (int i = 0; i < 1024; i++) buffer[i] = '\0';

	if (recv(clientConn, buffer, BUFFER_SIZE, 0)) {
		printf("Client sent: %s\n", buffer);

		char command[10], arg[BUFFER_SIZE - 10];
		sscanf(buffer, "%s %[^\n]", command, arg);

		if (strcmp(command, "echo") == 0) {
			send(clientConn, arg, strlen(arg), 0);
		}
		else if (strncmp(command, "repeat", 6) == 0) {
			for (int i = 0; i < (command[6] - '0'); i++) {
				send(clientConn, arg, strlen(arg), 0);
			}
		}
		else if (strcmp(command, "length") == 0) {
			int len = strlen(arg);
			if (len < 2 || len > 9) {
				char msg[] = "Invalid Command";
				send(clientConn, msg, strlen(msg), 0);
				closesocket(clientSocket);
				_endthreadex(0);
				return 0;
			}
			char len_buf[3];
			sprintf(len_buf, "%d", len);
			send(clientConn, len_buf, strlen(len_buf), 0);
		}
		else if (strcmp(command, "md5") == 0) {
			char md5_str[33];
			get_md5(arg, &md5_str);
			send(clientConn, md5_str, strlen(md5_str), 0);

		}
		else if (strcmp(command, "sha1") == 0) {
			char sha1_str[41];
			get_sha1(arg, &sha1_str);
			send(clientConn, sha1_str, strlen(sha1_str), 0);
		}
		else if (strcmp(command, "sha256") == 0) {
			char sha256_str[65];
			get_sha256(arg, &sha256_str);
			send(clientConn, sha256_str, strlen(sha256_str), 0);
		}
		else {
			char msg[] = "Invalid Command";
			send(clientConn, msg, strlen(msg), 0);
		}
	}
	closesocket(clientSocket);
	_endthreadex(0);
	return 0;
}

int main(void) {
	WSADATA winSockData;
	SOCKET serverSocket, clientSocket;

	struct sockaddr_in serverAddress, clientAddress;
	int addrLen = sizeof(clientAddress);
	HANDLE thread;
	int result;

	int threadnum = 0;

	result = WSAStartup(MAKEWORD(2, 2), &winSockData);
	if (result != 0) {
		printf("Startup failed");
		exit(-1);
	}

	serverSocket = socket(AF_INET, SOCK_STREAM, 0); // AF_INET --> IPv4, SOCK_STREAM --> TCP
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_addr.s_addr = INADDR_ANY;
	serverAddress.sin_port = htons(PORT);

	bind(serverSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress));

	listen(serverSocket, 5);

	while (1) {
		clientSocket = accept(serverSocket, &clientAddress, &addrLen);

		threadnum++;
		thread = _beginthreadex(NULL, 0, &HandleClient, &clientSocket, 0, &threadnum);
	}

	closesocket(serverSocket);
	WSACleanup();

	return 0;
}