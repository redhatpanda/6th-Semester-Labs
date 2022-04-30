#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<errno.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/msg.h>
#include<sys/stat.h>
#include<math.h>
#include<strings.h>
#include<netinet/in.h>
#include<sys/socket.h>
#include<time.h>

#define MAX_TEXT 5600

struct MSG{
    int m_txt[1400];
    int m_seq;
    int m_total_packet;
    int m_size;
};

int main()
{
    struct MSG m_data;
    FILE *out;
    char buf[MAX_TEXT];
    int f2;
    int i = 0;
    int sockrx, sock1;
    int s_len,c_len,rec;
    struct sockaddr_in s_address;
    struct sockaddr_in c_address;
    char ff[60];
    int r;
    int k,j=0,l,s=0;

    if((sockrx = socket(AF_INET,SOCK_STREAM,0))<0)
    {
        printf("\nSOCKET ERROR\n");
        exit(1);
    }

    s_address.sin_family = AF_INET;
    s_address.sin_addr.s_addr = htonl(INADDR_ANY);
    s_address.sin_port = htons(3456);
    s_len = sizeof(s_address);

    bind(sockrx,(struct sockaddr*)&s_address,s_len);
    listen(sockrx,5);
    int clen = sizeof(c_address);
    sock1 = accept(sockrx,(struct sockaddr*)&c_address,&clen);
    printf("\nConnection Established\n");
    out = fopen("abc","w+a");
    f2 = fileno(out);

    do{
        sleep(1);
        c_len = sizeof(c_address);
        rec = recvfrom(sock1,(void*)&m_data,sizeof(struct MSG),0,(struct sockaddr*)&c_address,&c_len);
        bzero(buf,sizeof(buf));
        memcpy(buf, m_data.m_txt, sizeof(m_data.m_txt));
        i = m_data.m_seq;
        printf("Sequence Number at Receiver=%d\n",i);
        write(f2,buf,m_data.m_size);
    } while(i!=m_data.m_total_packet);

    fclose(out);
    close(sockrx);
    exit(0);
}