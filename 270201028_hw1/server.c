#include <stdio.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h> // read(), write(), close()
#define MAX 80
#define PORT 8080
#define SA struct sockaddr

// Generate the famous turkish saying randomly
char* generate_turkish_saying_randomly(int random_number){
    char *sayings[] = {
        "Merhaba", "Eyvallah", "Guzel", "Lutfen", "Nasilsin", "Biliyorum", "Dogru", "Iyiyim", "Efendim", "Olur"
    }; 

    return sayings[random_number];
}

// Generate the random number in range [lower, upper] 
int generate_random_number(int lower, int upper)
{
    return (rand() % (upper - lower + 1)) + lower;
}

void func(int connfd)
{
    char buff[MAX];
    int n;

    // infinite loop for chat
    for (;;) {
        int lower = 0, upper = 9;
 
        // Seed for random generator
        srand(time(NULL));

        bzero(buff, MAX);
        // Get the random number
        int random_number = generate_random_number(lower, upper);

        // Get the famous turkish saying randomly
        char* saying = generate_turkish_saying_randomly(random_number);

        // Read the message from client and copy it in buffer
        read(connfd, buff, sizeof(buff));

        // Print buffer which contains the client contents
        printf("From client: %s\t To client : %s", buff,saying);
        bzero(buff, MAX);
        n = 0;
        
        // Copy server message in the buffer
        while ((buff[n] = *(saying+n)) != '\n'){
            n++;
        }

        // Send that buffer to client
        write(connfd, buff, sizeof(buff));
   
        // If msg contains "Exit" then server exit and chat ended.
        if (strncmp("exit", buff, 4) == 0) {
            printf("Server Exit...\n");
            break;
        }
    }
}

// Driver function
int main()
{
    int sockfd, connfd, len;
    struct sockaddr_in servaddr, cli;
   
    // Socket create and verification
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        printf("socket creation failed...\n");
        exit(0);
    }
    else
        printf("Socket successfully created..\n");
    bzero(&servaddr, sizeof(servaddr));
   
    // Assign IP, PORT
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(PORT);
   
    // Binding newly created socket to given IP and verification
    if ((bind(sockfd, (SA*)&servaddr, sizeof(servaddr))) != 0) {
        printf("socket bind failed...\n");
        exit(0);
    }
    else
        printf("Socket successfully binded..\n");
   
    // Server is ready to listen and verification
    if ((listen(sockfd, 5)) != 0) {
        printf("Listen failed...\n");
        exit(0);
    }
    else
        printf("Server listening..\n");
    len = sizeof(cli);
   
    // Accept the data packet from client and verification
    connfd = accept(sockfd, (SA*)&cli, &len);
    if (connfd < 0) {
        printf("server accept failed...\n");
        exit(0);
    }
    else
        printf("server accept the client...\n");
   
    // Function for chatting between client and server
    func(connfd);
   
    // After chatting close the socket
    close(sockfd);
}