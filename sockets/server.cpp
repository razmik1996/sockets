#include <sys/socket.h>
#include <stdio.h>
#include <string.h>
#include <netinet/in.h>
#include <iostream>
#include <cstdlib>
#define PORT 2352
#define CLOSE_CONNECTION_SYMBOL "EXIT"

bool isLost(const char*);

int main() {
    const int bufferSize = 1024;
    int serverSocket;
    int newSocket;
    struct sockaddr_in address;
    char buffer[bufferSize] = {0};
    int opt = 1;

    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if(serverSocket < 0) {
        std::cout << "Socked failed" << std::endl;
        std::exit(1);
    }
    
    if(setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        std::cout << "Setsockopt failed" << std::endl;
        std::exit(2);
    }

    address.sin_family = AF_INET;
    address.sin_port = htons(PORT);
    address.sin_addr.s_addr = htonl(INADDR_ANY);

    if(bind(serverSocket, reinterpret_cast<struct sockaddr *>(&address), sizeof(address)) < 0) {
        std::cout << "Bind failed" << std::endl;
        std::exit(3);
    }

    if(listen(serverSocket, 3) < 0) {
        std::cout << "Listen failed" << std::endl;
        std::exit(4);
    }

    socklen_t size = sizeof(address);

    newSocket = accept(serverSocket, reinterpret_cast<struct sockaddr *>(&address), &size);
    if(newSocket < 0) {
        std::cout << "Accept failed" << std::endl;
        std::exit(5);
    }

    bool isExit = false;
    while(newSocket > 0) {
        strcpy(buffer, "Server connected!\n");
        send(newSocket, buffer, bufferSize, 0);
        std::cout << "Enter " << CLOSE_CONNECTION_SYMBOL << " for exit" << std::endl;
        std::cout << "Client: ";
        recv(newSocket, buffer, bufferSize, 0);
        std::cout << buffer << std::endl;
        if(isLost(buffer)) {
            isExit = true;
        }
        while(!isExit) {
            std::cout << "Server: ";
            std::cin.getline(buffer, bufferSize);
            send(newSocket, buffer, bufferSize, 0);
            if(isLost(buffer)) {
                break;
            }

            std::cout << "Client: ";
            recv(newSocket, buffer, bufferSize, 0);
            std::cout << buffer << std::endl;
            if(isLost(buffer)) {
                break;
            }           
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
