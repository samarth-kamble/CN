#include<stdio.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<math.h>
int min(int a,int b)
{
    return(a<b?a:b);
}
int main()
{
    int tt,tp,n,totalpacketssendin1RTT,ack,noofbits,senderswindowsize;
    char msg1[50]="Hello Client!We are connected now";
    int valuegivenbysocket=socket(AF_INET,SOCK_STREAM,0);

    struct sockaddr_in srv;
    srv.sin_family=AF_INET;
    srv.sin_port=htons(9000);
    srv.sin_addr.s_addr=INADDR_ANY;

    int valuegivenbybind=bind(valuegivenbysocket,((struct sockaddr_in *)&srv),sizeof(srv));

    listen(valuegivenbysocket,3);

    int valuegivenbyaccept=accept(valuegivenbysocket,NULL,NULL);

    send(valuegivenbyaccept,&msg1,sizeof(msg1),0);

    printf("\nEnter transmission time (in ms):");
    scanf("%d",&tt);
    send(valuegivenbyaccept,&tt,sizeof(tt),0);

    printf("\nEnter propagation time (in ms):");
    scanf("%d",&tp);
    send(valuegivenbyaccept,&tp,sizeof(tp),0);

    printf("\nEnter total packets you want to transfer:");
    scanf("%d",&n);
    send(valuegivenbyaccept,&n,sizeof(n),0);

    totalpacketssendin1RTT=(1+(2*(tp/tt)));
    noofbits=log2(totalpacketssendin1RTT);
    senderswindowsize=min(pow(2,noofbits),(1+2*(tp/tt)));
    int slidingwindow[senderswindowsize];
    int a[senderswindowsize];
    
     
    int time1=tt,time2=tt+tp+tt,i,k=0;
  
    int j=0,recvack=0;
  
        while(j<senderswindowsize)
        {
        send(valuegivenbyaccept,&j,sizeof(j),0);
        send(valuegivenbyaccept,&time2,sizeof(time2),0);
        printf("\nPacket no. %d transferred at %d ms",j,time1);
        a[k]=j;
        k++;
        time1=time1+tt;
        time2=time2+tt;
        j++;
        }
        printf("\nSliding window:");
        for(int i=0;i<senderswindowsize;i++)
            printf("%d ",a[i]);
        while(j<n)
        {
        recv(valuegivenbyaccept,&ack,sizeof(ack),0);
        printf("\nAcknowledgement for packet no. %d received at %d ms",recvack,ack);
        for(i=0;i<senderswindowsize;i++)
        {
            a[i]=a[i+1];
        }
        a[i-1]=j;
        printf("\nSliding window:");
        for(int i=0;i<senderswindowsize;i++)
            printf("%d ",a[i]);
        recvack++;
        time1=ack+tt;
        time2=time1+tp+tt;
        send(valuegivenbyaccept,&j,sizeof(j),0);
        printf("\nPacket no. %d transferred at %d ms",j,time1);
        send(valuegivenbyaccept,&time2,sizeof(time2),0);
        j++;
        }
        while(recvack<n)
        {
            recv(valuegivenbyaccept,&ack,sizeof(ack),0);
            printf("\nAcknowledgement for packet no. %d received at %d ms",recvack,ack);
            recvack++;
        }
    
    close(valuegivenbysocket);

    return 0;
}
