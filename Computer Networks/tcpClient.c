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
#include<arpa/inet.h>
#include<time.h>

#define MAX_TEXT 1400

struct MSG{
    int m_txt[MAX_TEXT];
    int m_seq;
    int m_total_packet;
    int m_size;
};

int main(int argc, char *argv[])
{
    struct MSG m_data;
    int sock;
    struct sockaddr_in sa;
    int bytes_sent, buffer_length;
    char buffer[MAX_TEXT];
    FILE *in;
    int f,j=0;
    int total_packet_no = 0;
    int c;
    int size=0;
    int nread;

    if(argc != 3)
    {
        printf("Please Enter the following\n");
        printf("./msg filename IP-Address\n");
        exit(1);
    }

    in = fopen(argv[1], "r");
    while((c=fgetc(in))!=EOF)
        size++;

    fclose(in);
    total_packet_no = ceil(size/MAX_TEXT)+1;
    printf("Total Number of Packets:%d\n", total_packet_no);
    m_data.m_total_packet = total_packet_no;
    sleep(2);

    in = fopen(argv[1], "r");
    f = fileno(in);

    sock = socket(AF_INET,SOCK_STREAM,0);
    if(sock==-1){
        printf("Error Creating Socket\n");
        return 0;
    }

    sa.sin_family = AF_INET;
    sa.sin_addr.s_addr = inet_addr(argv[2]);
    sa.sin_port = htons(3456);
    connect(sock,(struct sockaddr*)&sa,sizeof(sa));
    
    while(j<total_packet_no){
        sleep(1);
        memset(buffer,0,sizeof(buffer));
        nread = read(f,m_data.m_txt,MAX_TEXT);
        m_data.m_size = nread;
        if(nread == -1)
        {
            printf("Read Error\n");
            exit(1);	
        }
        j++;
        m_data.m_seq = j;
        printf("Sequence Number at Tx=%d\n",m_data.m_seq);
        bytes_sent=sendto(sock,(void*)&m_data,sizeof(struct MSG),0,(struct sockaddr*)&sa,sizeof(struct sockaddr_in));
        if(bytes_sent<0){
            printf("Error Sending Packe:%s\n",strerror(errno));
            exit(1);
        }
    }
    close(sock);
    fclose(in);
    exit(0);
}