#if defined WIN32
#include <winsock2.h>
#else
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#endif

#include <stdio.h>
#include <ctype.h>
#include "math_functions.h"

// Function to clean up Winsock resources on Windows
void clearwinsock() {
#if defined WIN32
    WSACleanup();
#endif
}

// Function to handle error messages
void errorhandler(char *errorMessage) {
    printf("%s", errorMessage);
}

int main(int argc, char *argv[]) {
#if defined WIN32
    // Initialize Winsock for Win32 platform
    WSADATA wsa_data;
    int result = WSAStartup(MAKEWORD(2, 2), &wsa_data);
    if (result != NO_ERROR) {
        printf("Error at WSAStartup()\n");
        return 0;
    }
#endif

    // Create a socket
    int sSocket = socket(PF_INET, SOCK_STREAM, 0);
    if (sSocket < 0) {
        errorhandler("Socket error\n");
        clearwinsock();
        return 1;
    } else {
        printf("Socket created\n");
    }

    // Set up the server address structure
    struct sockaddr_in cod;
    cod.sin_family = AF_INET;
    cod.sin_addr.s_addr = inet_addr("127.0.0.1");
    cod.sin_port = htons(9999);

    // Bind the socket to the specified address and port
    if (bind(sSocket, (struct sockaddr *) &cod, sizeof(cod)) < 0) {
        errorhandler("bind() failed.\n");
        closesocket(sSocket);
        clearwinsock();
        return 1;
    }

    int qlen = 5;

    // Listen for incoming connections
    if (listen(sSocket, qlen) < 0) {
        errorhandler("listen() failed. \n");
        closesocket(sSocket);
        clearwinsock();
        return 1;
    }

    int clientServerSocket;
    struct sockaddr_in consocket;
    int address_len;

    char operat;
    int connum1;
    int connum2;

    while (1) {
        printf("\nWaiting for a client to connect...");
        address_len = sizeof(consocket);
        char input[500];
        char output[500] = "";

        // Accept a connection from a client
        if ((clientServerSocket = accept(sSocket, (struct sockaddr *) &consocket, &address_len)) < 0) {
            errorhandler("accept() failed. \n");
            closesocket(clientServerSocket);
            continue;
        }

        printf("\nHandling client %s\n", inet_ntoa(consocket.sin_addr));

        // Receive operator and numbers from the client
        int byterec;
        while ((byterec = recv(clientServerSocket, input, sizeof(input), 0)) > 0) {
            // Parse the received input to extract operator and numbers
            if (sscanf(input, "%c %d %d", &operat, &connum1, &connum2) != 3) {
                errorhandler("Invalid input from client\n");
                continue;
            }

            int risult;

            // Perform the requested mathematical operation
            switch (operat) {
                case '+':
                    risult = add(connum1, connum2);
                    break;
                case '-':
                    risult = sub(connum1, connum2);
                    break;
                case 'x':
                    risult = mult(connum1, connum2);
                    break;
                case '/':
                    risult = division(connum1, connum2);
                    break;
                default:
                    errorhandler("Invalid operator\n");
                    continue;
            }

            // Prepare the result message
            sprintf(output, "result: %d\n", risult);
            int len = strlen(output);

            // Send the result back to the client
            send(clientServerSocket, output, len, 0);
        }

        // Close the client socket
        closesocket(clientServerSocket);
    }

    // Clean up resources
    closesocket(sSocket);
    clearwinsock();

    return 0;
}
