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
        int listenfd, connfd, bytes_wrt, bytes_rcv, total_bytes_wrt, sign;
        int nums[2] = {0 ,0};
        struct sockaddr_in serv_addr, cli_addr;
        socklen_t len = sizeof(struct sockaddr_in);
        char ip_str[INET_ADDRSTRLEN], wrt_buff[MAXLINE], rcv_buff[MAXLINE];

        /* initialize server socket */
        listenfd = socket(AF_INET, SOCK_STREAM, 0); /* SOCK_STREAM : TCP */
        if (listenfd < 0) {
                printf("Error: init socket\n");
                return 0;
        }

        /* initialize server address (IP:port) */
        memset(&serv_addr, 0, sizeof(serv_addr));
        serv_addr.sin_family = AF_INET;
        serv_addr.sin_addr.s_addr = INADDR_ANY; /* IP address: 0.0.0.0 */
        serv_addr.sin_port = htons(SERVER_PORT); /* port number */

        /* bind the socket to the server address */
        if (bind(listenfd, (struct sockaddr *)&serv_addr, sizeof(struct sockaddr)) < 0) {
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
                connfd = accept(listenfd, (struct sockaddr *)&cli_addr, &len);
                if (connfd < 0) {
                        printf("Error: accept\n");
                        return 0;
                }
                memset(&ip_str, 0, sizeof(ip_str));
                memset(&wrt_buff, 0, sizeof(wrt_buff));
                memset(&rcv_buff, 0, sizeof(rcv_buff));

                /* print client (remote side) address (IP : port) */
                inet_ntop(AF_INET, &(cli_addr.sin_addr), ip_str, INET_ADDRSTRLEN);
                printf("Incoming connection from %s : %hu\n", ip_str, ntohs(cli_addr.sin_port));

                /* read the response */
                bytes_rcv = 0;
                while (1) {
                        bytes_rcv += recv(connfd, rcv_buff + bytes_rcv, sizeof(rcv_buff) - bytes_rcv - 1, 0);
                        /* terminate until read '\n' */
                        if (bytes_rcv && rcv_buff[bytes_rcv - 1] == '\n')
                                break;
                }

                /* parse request */
                sscanf(rcv_buff, "%d %d\n", &nums[0], &nums[1]);

                /* generate response */
                snprintf(wrt_buff, sizeof(wrt_buff) - 1, "%d", nums[0] + nums[1]);
                printf("%d + %d = %d\n", nums[0], nums[1], nums[0] + nums[1]);

                /* send response */
                bytes_wrt = 0;
                total_bytes_wrt = strlen(wrt_buff);
                while (bytes_wrt < total_bytes_wrt) {
                        bytes_wrt += write(connfd, wrt_buff + bytes_wrt, total_bytes_wrt - bytes_wrt);
                }

                /* close the connection */
                close(connfd);
        }

        return 0;
}
