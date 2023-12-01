#include<stdio.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
int main()
{
    int tt,tp,n,i=1,get,time,ack;//get packet no.,time at which packet will recieve at client,ack to give ack at
    char buffer1[40];
    int valuegivenbysocket=socket(AF_INET,SOCK_STREAM,0);

    struct sockaddr_in srv;
    srv.sin_family=AF_INET;
    srv.sin_port=htons(9000);
    srv.sin_addr.s_addr=INADDR_ANY;

    int valuegivenbyconnect=connect(valuegivenbysocket,((struct sockaddr_in *)&srv),sizeof(srv));

    recv(valuegivenbysocket,&buffer1,sizeof(buffer1),0);
    printf("\nMsg from server:%s",buffer1);


    recv(valuegivenbysocket,&tt,sizeof(tt),0);
    recv(valuegivenbysocket,&tp,sizeof(tp),0);
    recv(valuegivenbysocket,&n,sizeof(n),0);
    while(i<=n)
    {
        recv(valuegivenbysocket,&get,sizeof(get),0);
        recv(valuegivenbysocket,&time,sizeof(time),0);
        printf("\n%d Packet recieved at %d",get,time);
        ack=time+tp;
        send(valuegivenbysocket,&ack,sizeof(ack),0);
        i++;
    }

    close(valuegivenbysocket);

    return 0;
}
