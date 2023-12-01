#include<stdio.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
int main()
{
    char msg1[40]="Hello client,we are connected now";
    int valuegivenbysocket=socket(AF_INET,SOCK_STREAM,0);

    struct sockaddr_in srv;
    srv.sin_family=AF_INET;
    srv.sin_port=htons(9000);
    srv.sin_addr.s_addr=INADDR_ANY;

    int valuegivenbybind=bind(valuegivenbysocket,((struct sockaddr_in *)&srv),sizeof(srv));

    listen(valuegivenbysocket,3);

    int valuegivenbyaccept=accept(valuegivenbysocket,NULL,NULL);

    send(valuegivenbyaccept,&msg1,sizeof(msg1),0);


    int tt,tp,n;
    printf("\nEnter transmission time(in ms):");
    scanf("%d",&tt);
    send(valuegivenbyaccept,&tt,sizeof(tt),0);

    printf("\nEnter propagation time(in ms):");
    scanf("%d",&tp);
    send(valuegivenbyaccept,&tp,sizeof(tp),0);

    printf("\nEnter total number of packets need to transfer:");
    scanf("%d",&n);
    send(valuegivenbyaccept,&n,sizeof(n),0);

    int i=1;
    int time,ack=0;//time at which packet will recieve at client
    while(i<=n)
    {
            printf("\n%d packet transfered at %d ms",i,tt+ack);
            send(valuegivenbyaccept,&i,sizeof(i),0);
            if(i==1)
            time=tt+tp+tt;
            else
            time=ack+tt+tp+tt;//time=7ms
            send(valuegivenbyaccept,&time,sizeof(time),0);
            recv(valuegivenbyaccept,&ack,sizeof(ack),0);
            printf("\nAcknowledge received at %d ms",ack);//ack=12
            i++;
    }

    close(valuegivenbysocket);


    return 0;
}
