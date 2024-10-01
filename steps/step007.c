#include <arpa/inet.h>
#include <errno.h>
#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int main() {
  char buffer[BUFFER_SIZE];
  char resp[] = "HTTP/1.0 200 OK\r\n"
                "Server: webserver-c\r\n"
                "Content-type: text/html\r\n\r\n"
                "<html>hello, from c!</html>\r\n";

  // create socket
  int sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd == -1)
  {
    perror("webserver (socket)");
    return 1;
  }
  printf("socket created successfully\n");

  // create address to bind the socket to
  struct sockaddr_in host_addr;
  int host_addrlen = sizeof(host_addr);

  host_addr.sin_family=AF_INET;
  host_addr.sin_port=htons(PORT);
  host_addr.sin_addr.s_addr=htonl(INADDR_ANY);
  
  // create client address
  struct sockaddr_in client_addr;
  int client_addrlen = sizeof(client_addr);

  // bind the socket to the address
  if (bind(sockfd, (struct sockaddr *)&host_addr, host_addrlen) != 0)
  {
    perror("webserver (bind)");
    return 1;
  }

  printf("socket successfully bound to address\n");
  
  if(listen(sockfd, SOMAXCONN) != 0)
  {
    perror("webserver (listen)");
    return 1;
  }
  printf("server listening for connection\n");
  
  for (;;) {
    // asep incoming connections
    int newsockfd=accept(sockfd, (struct sockaddr *)&host_addr, (socklen_t *)&host_addrlen);
    if (newsockfd < 0)
    {
      perror("webserver (accept)");
      continue;
    }
    printf("connection accepted\n");
    
    //get client address
    int sockn = getsockname(newsockfd, (struct sockaddr*)&client_addr, (socklen_t *)&client_addrlen);

    //iqra value from soket 
    int valread=read(newsockfd, buffer, BUFFER_SIZE);
    if(valread < 0)
    {
      perror("webserver (read)");
      continue;
    }
    
    //print client ipaddr
    printf("[%s:%u]\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

    //write to socket
    int valwrite=write(newsockfd, resp, strlen(resp));
    if (valwrite < 0)
    {
      perror("webserver (write)");
      continue;
    }

    close(newsockfd);
  }
  
  return 0;
}
