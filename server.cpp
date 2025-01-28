#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>
#include <thread>

int main(){
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if(serverSocket == -1){
        perror("Error creating socket");
        return 1;
    }


    sockaddr_in serverAddress{};
    socklen_t serverAddressSize = sizeof(serverAddress);
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(8080);
    serverAddress.sin_addr.s_addr = INADDR_ANY;

    
    
    int bind_status = bind(serverSocket, (sockaddr*)&serverAddress, serverAddressSize);
    // bind(serverSocket, (sockaddr*)&serverAddress, serverAddressSize);
    if(bind_status == -1){
        perror("Error binding socket");
        return 1;
    }

    int listen_status = listen(serverSocket, 5);
    if(listen_status == -1){
        perror("Error listening on socket");
        return 1;
    }

    std::cout << "Server is listening on port 8080" << std::endl;

    while (true){
        sockaddr_in clientAddress{};
        socklen_t clientAddressSize = sizeof(clientAddress);
        int clientSocket = accept(serverSocket, (sockaddr*)&clientAddress, &clientAddressSize);
        if(clientSocket == -1){
            perror("Error accepting client connection");
            return 1;
        }
        char buffer[1024];
        memset(buffer, 0, sizeof(buffer));
        int bytesRead = recv(clientSocket, buffer, sizeof(buffer), 0);
        if(bytesRead == -1){
            perror("Error reading from client");
            return 1;
        }
        std::cout << "Client says: " << buffer << std::endl;
        send(clientSocket, buffer, bytesRead, 0);
        close(clientSocket);
        std::this_thread::sleep_for(std::chrono::seconds(5));
    }

    close(serverSocket);
    return 0;

}