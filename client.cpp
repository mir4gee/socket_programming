#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <cstring>

int main(){

    int clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if(clientSocket == -1){
        perror("Error creating socket");
        return 1;
    }

    sockaddr_in serverAddress{};
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(8080);
    inet_pton(AF_INET, "127.0.0.1", &serverAddress.sin_addr);

    if (connect(clientSocket, (sockaddr*)&serverAddress, sizeof(serverAddress)) == -1){
        perror("Error connecting to server");
        return 1;
    }

    char buffer[1024];
    memset(buffer, 0, sizeof(buffer));

    std::string message;

    while (true){
        std::cout << "Enter a message: ";
        std::getline(std::cin, message);
        send(clientSocket, message.c_str(), message.size(), 0);
        int bytesRead = recv(clientSocket, buffer, sizeof(buffer), 0);
        std::cout << "Server says: " << buffer << std::endl;
        memset(buffer, 0, sizeof(buffer));
    }

    close(clientSocket);



    return 0;
}