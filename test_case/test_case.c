#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <getopt.h>

#define TEST_MAX_LENGTH     128

#define TIME_SUB(tv1, tv2) ((tv1.tv_sec - tv2.tv_sec)*1000 + (tv1.tv_usec - tv2.tv_usec)/1000)

int send_case(int fd, char* msg, int len)
{
    int ret = send(fd, msg, len, 0);
    if(ret < 0)
    {
        close(fd);
        exit(1);
    }

    return ret;
}

int recv_case(int fd, char* buf, int len)
{
    int ret = recv(fd, buf, len, 0);
    if(ret < 0)
    {
        close(fd);
        exit(1);
    }

    return ret;
}

void equals(char* expect, char* result, char* mod)
{
    if(strcmp(expect, result) == 0) return;
    else printf("case: %s failed------->expect:%s, result:%s\r\n", mod, expect, result);
}

void test_case(int fd, char* msg, char* expect, char* mod)
{
    if(!msg || !expect || !mod)
        return;

    send_case(fd, msg, strlen(msg));

    char buf[TEST_MAX_LENGTH] = {0};
    recv_case(fd, buf, TEST_MAX_LENGTH);
    equals(expect, buf, mod);
}

void array_test(int fd)
{
    test_case(fd, "SET NAME JIEJIE", "SUCCESS", "SET");
    test_case(fd, "GET NAME", "value is JIEJIE", "GET");
    test_case(fd, "MOD NAME JIEJIE MIEMIE", "SUCCESS", "MOD");
    test_case(fd, "GET NAME", "value is MIEMIE", "GET");
    test_case(fd, "DEL NAME", "SUCCESS", "DEL");
    test_case(fd, "GET NAME", "FAILED", "GET");
}

void array_test_10w(int fd)
{
    int i = 100000;
    while(i > 0)
    {
        array_test(fd);
        i--;
    }
}


int connect_tcpserv(const char* ip, unsigned short port)
{
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in local;
    memset(&local, 0, sizeof(local));
    local.sin_family = AF_INET;
    local.sin_port = htons(port);
    local.sin_addr.s_addr = inet_addr(ip);

    int ret = connect(sockfd, (struct sockaddr*)&local, sizeof(local));
    if(ret)
    {
        perror("connect failed");
        return -1;
    }

    return sockfd;
}

//-s -p -m

int main(int argc, char* argv[])
{
    char ip[16];
    int port, mode;
    int opt;
    while((opt = getopt(argc, argv, "s:p:m:?")) != -1)
    {
        switch(opt)
        {
            case 's':
                strcpy(ip, optarg);
                break;
            case 'p':
                port = atoi(optarg);
                break;
            case 'm':
                mode = atoi(optarg);
                break;
        }
    }

    int connfd = connect_tcpserv(ip, port);

    if(mode & 0x01)
    {
        struct timeval tv_begin;
        gettimeofday(&tv_begin, NULL);

        array_test_10w(connfd);

        struct timeval tv_end;
        gettimeofday(&tv_end, NULL);

        int t = TIME_SUB(tv_end, tv_begin);

        printf("qps is %d\r\n", 600000*1000/t);
    }

    return 0;
}