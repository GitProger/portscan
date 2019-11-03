#ifndef __CL_H_INC
#define __CL_H_INC
#include "ports.h"
#include <winsock2.h>
#include <windows.h>
#include <ws2tcpip.h>

char *gets_s(char *s, int n) {
    fflush(stdin);
    int i, k = getchar();
    if (k == EOF)
         return NULL;
    for (i = 0; (k != EOF && k != '\n') && n--; i++) {
        s[i] = k;
        k = getchar();
        if (k == EOF && !feof(stdin))
            return NULL;
    }
    s[i] = '\0';
    return s;    
}

int scanPorts(const char *ip, int beg, int end) {
    WSADATA wsaData;
    SOCKET ConnectSocket = INVALID_SOCKET;
    struct addrinfo *result = NULL, *ptr = NULL, hints;
    char *sendbuf = "TESTING_PORT", port[7];
    int iResult, i, portint;

    iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult) {
        printf("WSAStartup failed with error %d\n", iResult);
        return 1;
    }
    
    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    
    for (portint = beg; portint <= end; portint++) {
        sprintf(port, "%d", portint);
        
        iResult = getaddrinfo(ip, port, &hints, &result);
        if (iResult) {
            printf("getaddrinfo failed with error: %d\n", iResult);
            WSACleanup();
            return 1;
        }
    
        for (ptr = result; ptr != NULL; ptr = ptr->ai_next) {
            ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype,
               ptr->ai_protocol);
            if (ConnectSocket == INVALID_SOCKET) {
                printf("socket failed with error: %d\n", WSAGetLastError());
                WSACleanup();
                return 1;
            }
            iResult = connect(ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
            if (iResult == SOCKET_ERROR) {
                closesocket(ConnectSocket);
                ConnectSocket = INVALID_SOCKET;
                continue;
            }
            break;
        }
        freeaddrinfo(result);
    
        printf("[%s]: port %s: ", ip, port);
        if (ConnectSocket == INVALID_SOCKET) {
            printf("closed.\n");
            closesocket(ConnectSocket);
            continue;
        }
        printf("opened.  =!\n");
        closesocket(ConnectSocket);
    }
    WSACleanup();
}

#endif
