#include <stdio.h>  
#include <netinet/in.h>  
#include <sys/socket.h>  
#include <netinet/ether.h>  
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <sys/socket.h>    
#include <netinet/ether.h>    
#include <stdio.h>
#include <net/if.h>// struct ifreq    
#include <sys/ioctl.h> // ioctl、SIOCGIFADDR    
#include <sys/socket.h> // socket    
#include <netinet/ether.h> // ETH_P_ALL    
#include <netpacket/packet.h> // struct sockaddr_ll 

int main(int argc,char *argv[])  
{  
    unsigned char buf[2048] = {0};  
    struct ifreq req;
    struct sockaddr_ll send_sll;
    struct sockaddr_ll recv_sll;
    socklen_t len = sizeof(recv_sll);

    int send_raw_fd = socket(PF_PACKET, SOCK_RAW, htons(ETH_P_ALL));  
    strncpy(req.ifr_name, "C", IFNAMSIZ);              
    if(-1 == ioctl(send_raw_fd, SIOCGIFINDEX, &req))    
    {  
        perror("ioctl");  
        close(send_raw_fd);  
        exit(-1);  
    }
    bzero(&send_sll, sizeof(send_sll));
    send_sll.sll_family = PF_PACKET;
    send_sll.sll_protocol = htons(ETH_P_ALL);
    send_sll.sll_ifindex = req.ifr_ifindex;
    bind(send_raw_fd, (struct sockaddr *)&send_sll, sizeof(send_sll));

    int sock_raw_fd = socket(PF_PACKET, SOCK_RAW, htons(ETH_P_ALL));  
    strncpy(req.ifr_name, "B", IFNAMSIZ);              
    if(-1 == ioctl(sock_raw_fd, SIOCGIFINDEX, &req))    
    {  
        perror("ioctl");  
        close(sock_raw_fd);  
        exit(-1);  
    }  
    bzero(&recv_sll, sizeof(recv_sll));
    recv_sll.sll_family = PF_PACKET;
    recv_sll.sll_protocol = htons(ETH_P_ALL);
    recv_sll.sll_ifindex = req.ifr_ifindex;
    bind(sock_raw_fd, (struct sockaddr *)&recv_sll, sizeof(recv_sll));

    while(1){
        int rcv_len = recvfrom(sock_raw_fd, buf, sizeof(buf), 0, (struct sockaddr *)&recv_sll, &len);  

        if(rcv_len <= 0) {
            perror("recvfrom\n");
            break;
        }

        int snd_len = sendto(send_raw_fd, buf, rcv_len, 0 , (struct sockaddr *)&send_sll, sizeof(send_sll));  
        if(snd_len <= 0)  
        {  
            perror("sendto\n");
            break;
        }  
    }

    return 0;  
}  
