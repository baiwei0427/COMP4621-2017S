#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <time.h>

#define MAXLINE 100
#define SERVER_PORT 12345
#define LISTENNQ 5

int main(int argc, char **argv)
{
        int listenfd, connfd;
        struct sockaddr_in servaddr, cliaddr;
        socklen_t len = sizeof(struct sockaddr_in);
        char buff[MAXLINE] = {0};
        char ip_str[INET_ADDRSTRLEN] = {0};
        time_t ticks;

        /* initialize server socket */
        listenfd = socket(AF_INET, SOCK_STREAM, 0); /* SOCK_STREAM : TCP */
        if (listenfd < 0) {
                printf("Error: init socket\n");
                return 0;
        }

        /* initialize server address (IP:port) */
        memset(&servaddr, 0, sizeof(servaddr));
        servaddr.sin_family = AF_INET;
        servaddr.sin_addr.s_addr = INADDR_ANY; /* IP address: 0.0.0.0 */
        servaddr.sin_port = htons(SERVER_PORT); /* port number */

        /* bind the socket to the server address */
        if (bind(listenfd, (struct sockaddr *)&servaddr, sizeof(struct sockaddr)) < 0) {
                printf("Error: bind\n");
                return 0;
        }

        if (listen(listenfd, LISTENNQ) < 0) {
                printf("Error: listen\n");
                return 0;
        }

        /* keep processing incoming requests */
        while (1) {
                /* accept an incoming connection from the remote side */
                connfd = accept(listenfd, (struct sockaddr *)&cliaddr, &len);
                if (connfd < 0) {
                        printf("Error: accept\n");
                        return 0;
                }

                /* print client (remote side) address (IP : port) */
                inet_ntop(AF_INET, &(cliaddr.sin_addr), ip_str, INET_ADDRSTRLEN);
                printf("Incoming connection from %s : %hu\n", ip_str, ntohs(cliaddr.sin_port));

                /* get current time and construct the reponse */
                ticks = time(NULL);
                snprintf(buff, sizeof(buff) - 1, "%.24s\r\n", ctime(&ticks));

                /* send response */
                write(connfd, buff, strlen(buff));

                /* close the connection */
                close(connfd);
        }

        return 0;
}
