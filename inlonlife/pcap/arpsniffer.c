#include <pcap.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define ARP_REQUEST 1
#define ARP_REPLY 2
typedef struct arphdr {
    u_int16_t htype;
    u_int16_t ptype;
    u_char hlen;
    u_char plen;
    u_int16_t oper;
    u_char sha[6];
    u_char spa[4];
    u_char tha[6];
    u_char tpa[4];
} arphdr_t;

#define MAXBYTE2CAPTURE 2048

int main(int argc, char *argv[])
{
    int i = 0;
    bpf_u_int32 netaddr = 0, mask = 0;
    struct bpf_program filter;
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t *descr = NULL;
    struct pcap_pkthdr pkthdr;
    const unsigned char *packet = NULL;
    arphdr_t *arpheader = NULL;
    memset(errbuf, 0, PCAP_ERRBUF_SIZE);

    if (argc < 2) {
        printf("用法: %s <接口>\n", argv[0]);
        exit(1);
    }

    descr = pcap_open_live(argv[1], MAXBYTE2CAPTURE, 0, 512, errbuf);
    pcap_lookupnet(argv[1], &netaddr, &mask, errbuf);
    pcap_compile(descr, &filter, "arp", 1, mask);
    pcap_setfilter(descr, &filter);

    for (;;) {
        packet = pcap_next(descr, &pkthdr);
        arpheader = (struct arphdr *)(packet + 14);
        printf("\n\n大小: %d 字节\n", pkthdr.len);
        printf("硬件: %s\n", (ntohs(arpheader->htype) == 1) ? "以太网" : "未知");
        printf("协议: %s\n", (ntohs(arpheader->ptype) == 0x800) ? "IPv4" : "未知");
        printf("动作: %s\n", (ntohs(arpheader->oper) == ARP_REQUEST) ? "ARP请求" : "ARP应答");

        if (ntohs(arpheader->htype) == 1 && ntohs(arpheader->ptype) == 0x800) {
            printf("发送端 MAC 地址: ");
            for (i = 0; i < 6; i++) printf("%02X:", arpheader->sha[i]);
            printf("\n发送端 IP 地址: ");
            for (i = 0; i < 4; i++) printf("%0d.", arpheader->spa[i]);
            printf("\n目的端 MAC 地址: ");
            for (i = 0; i < 6; i++) printf("%02X:", arpheader->tha[i]);
            printf("\n目的端 IP 地址: ");
            for (i = 0; i < 4; i++) printf("%0d.", arpheader->tpa[i]);
            printf("\n");
        }
    }

    return 0;
}

