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

int main() {
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
    int cSocket = socket(PF_INET, SOCK_STREAM, 0);
    if (cSocket < 0) {
        errorhandler("Socket error\n");
        clearwinsock();
        return 1;
    } else {
        printf("Socket created\n");
    }

    // Set up the server address structure
    struct sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    serverAddr.sin_port = htons(9999);

    // Connect to the server
    if (connect(cSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        errorhandler("Failed to connect\n");
        closesocket(cSocket);
        clearwinsock();
        return 1;
    }

    char input[500];
    char output[500];
    char operator;
    int num1;
    int num2;

    while (1) {
        printf("Insert operator, number one, and number two. [operator space number one space number two]\n");
        if (fgets(input, sizeof(input), stdin) == NULL) {
            break;
        }

        // Check if the operator is '=' and break the loop if true
        if (input[0] == '=' && input[1] == '\n') {
            printf("System close, you entered '='\n");
            break;
        }

        if (sscanf(input, "%c %d %d", &operator, &num1, &num2) != 3) {
            errorhandler("Invalid input\n");
            continue;
        }

        if (input[1] != ' ') {
            errorhandler("Put a space after the operator\n");
            continue;
        }

        if (operator != '+' && operator != '-' && operator != 'x' && operator != '/') {
            errorhandler("Invalid operator\n");
            continue;
        }

        // Send operator and numbers to the server
        send(cSocket, input, strlen(input), 0);

        // Receive the result from the server
        int resultrecv = recv(cSocket, output, sizeof(output), 0);

        if (resultrecv <= 0) {
            break;
        }

        printf("Result: %s\n", output);
    }

    // Close the client socket and clean up Winsock resources
    closesocket(cSocket);
    clearwinsock();
    return 0;
}
