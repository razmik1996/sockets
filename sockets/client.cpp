#include <sys/socket.h>
#include <stdio.h>
#include <string.h>
#include <netinet/in.h>
#include <iostream>
#include <cstdlib>
#include <arpa/inet.h>
#include <netdb.h>

#define SERVER_IP "127.0.0.1"
#define PORT 2352
#define CLOSE_CONNECTION_SYMBOL "EXIT"

bool isLost(const char*);

int main() {
    const int bufferSize = 1024;
    int clientSocket;
    struct sockaddr_in serv_addr;
    char buffer[bufferSize] = {0};
    
    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if(clientSocket < 0) {
        std::cout << "Socked failed" << std::endl;
        std::exit(1);
    }

    serv_addr.sin_port = htons(PORT);
    serv_addr.sin_family = AF_INET;
    inet_pton(AF_INET, SERVER_IP, &serv_addr.sin_addr);

    int ret = connect(clientSocket, reinterpret_cast<const struct sockaddr*>(&serv_addr), sizeof(serv_addr));
    if(ret == 0) {
        std::cout << "Connection to server...";
    }
    
    std::cout << "Waiting for confirmation...\n";

    recv(clientSocket, buffer, bufferSize, 0);
    std::cout << "Connection established.\n";
    std::cout << buffer;
    std::cout << "Enter " << CLOSE_CONNECTION_SYMBOL << " for exit" << std::endl;
    

    while(true) {
        std::cout << "Client: ";
        std::cin.getline(buffer, bufferSize);
        send(clientSocket, buffer, bufferSize, 0);
        if(isLost(buffer)) {
            break;
        }
    
        std::cout << "Server: ";
        recv(clientSocket, buffer, bufferSize, 0);
        std::cout << buffer << std::endl;
        if(isLost(buffer)) {
            break;
        }
    }

}

bool isLost(const char* msg) {
    for(int i = 0; i < strlen(msg); ++i) {
        const char* exit = CLOSE_CONNECTION_SYMBOL;
        if(msg[i] == exit[i]) {
            continue;
        } else {
            return false;
        }
    }
    return true;
}

