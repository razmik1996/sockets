#include <netinet/in.h>
#include <sys/socket.h>
#include <string.h>
#include <iostream>
#include <cstdlib>
#include <stdio.h>

#define PORT 2333


int main() {
    const int bufferSize = 1024;
    int serverSocket;
    int newSocket;
    struct sockaddr_in address;
    char buffer[bufferSize] = {" "};
    int opt = 1;

    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket < 0) {
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
    
    strcpy(buffer, "Server connected!\n");
    send(newSocket, buffer, bufferSize, 0);

    const char* filename = "File.txt";

    size_t dataSize;
    FILE* fd = fopen(filename, "wb");
    while(!received) {
        dataSize = recv(newSocket, buffer, bufferSize, 0);
        fwrite(&buffer, 1, dataSize, fd);
    }
    fclose(fd);
}
