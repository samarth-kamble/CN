#include<stdio.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<math.h>
int min(int a,int b)
{
    return (a<b?a:b);
}
int main()
{
    int tt,tp,n,time,get,sendack=0;
    char buffer1[50];
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
    int totalpacketssendin1RTT=(1+(2*(tp/tt)));
    int noofbits=log2(totalpacketssendin1RTT);
    int senderswindowsize=min(pow(2,noofbits),(1+2*(tp/tt)));

    int count=0,j=0,ack=tt+tp+tt+tp,time2;
    
    while(j<senderswindowsize)
    {
        recv(valuegivenbysocket,&get,sizeof(get),0);
        recv(valuegivenbysocket,&time2,sizeof(time2),0);
        printf("\nPacket no. %d received at %d ms",get,time2);
        j++;
    }
    while(j<n)//j=11
    {
        send(valuegivenbysocket,&ack,sizeof(ack),0);//14 ms last gela
        ack=ack+tt;
        //15
        sendack++;
        //3
        recv(valuegivenbysocket,&get,sizeof(get),0);
        recv(valuegivenbysocket,&time2,sizeof(time2),0);
        printf("\nPacket no. %d received at %d ms",get,time2);
        j++;
    }
    while(sendack<n)//7
    {
        send(valuegivenbysocket,&ack,sizeof(ack),0);
        sendack++;

        if(sendack==senderswindowsize)
        ack=ack+tp;
        else
        ack=ack+tt;//18
    }


    close(valuegivenbysocket);
    return 0;
}
