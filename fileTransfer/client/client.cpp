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
    
    const char *filename = "File.txt";
    FILE *fd = fopen(filename, "rb");
    size_t rret, wret;
    int bytes_read;
    while (!feof(fd)) {
        if ((bytes_read = fread(&buffer, 1, bufferSize, fd)) > 0)
            send(clientSocket, buffer, bytes_read, 0);
        else
            break;
    }
    fclose(fd);
}
