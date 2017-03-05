#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>

#define MAXLINE 100

int main()
{
        int sockfd, n;
        struct sockaddr_in servaddr;
        char recvline[MAXLINE] = {0};

        /* init socket */
        sockfd = socket(AF_INET, SOCK_STREAM, 0);       /* TCP */
        if (sockfd < 0) {
                printf("Error: init socket\n");
                return 0;
        }

        /* init server address (IP : port) */
        memset(&servaddr, 0, sizeof(servaddr));
        servaddr.sin_family = AF_INET;
        servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
        servaddr.sin_port = htons(12345);

        /* connect to the server */
        if (connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
                printf("Error: connect\n");
                return 0;
        }

        /* read the response */
        while (1) {
                n = read(sockfd, recvline, sizeof(recvline) - 1);

                if (n <= 0) {
                        break;
                } else {
                        recvline[n] = 0;        /* 0 terminate */
                        printf("%s", recvline);
                }
        }

        /* close the connection */
        close(sockfd);
        return 0;
}
