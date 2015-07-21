#define __USE_BSD
#include <netinet/ip.h>
#define __FAVOR_BSD
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <pcap.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAXBYTE2CAPTURE 2048

int tcp_rst_send(tcp_seq seq, tcp_seq ack, unsigned long src_ip,
                 unsigned long dst_ip, u_short src_prt, u_short dst_prt,
                 u_short win)
{
    return 0;
}

int main(int argc, char *argv[])
{
    int count = 0;
    bpf_u_int32 netaddr, mask = 0;
    pcap_t *descr = NULL;
    struct bpf_program filter;
    struct ip *iphdr = NULL;
    struct tcphdr *tcphdr = NULL;
    struct pcap_pkthdr pkthdr;
    const unsigned char *packet = NULL;
    char errbuf[PCAP_ERRBUF_SIZE];

    memset(errbuf, 0, PCAP_ERRBUF_SIZE);

    if (argc < 2) {
        printf("用法: %s <接口>\n", argv[0]);
        exit(1);
    }

    descr = pcap_open_live(argv[1], MAXBYTE2CAPTURE, 1, 512, errbuf);
    pcap_lookupnet(argv[1], &netaddr, &mask, errbuf);
    pcap_compile(descr, &filter, "(tcp[13] == 0x10) or (tcp[13] == 0x18)", 1,
                 mask);
    pcap_setfilter(descr, &filter);

    for (;;) {
        packet = pcap_next(descr, &pkthdr);
        iphdr = (struct ip *)(packet + 14);
        tcphdr = (struct tcphdr *)(packet + 14 + 20);
        printf("\n\n序号: %d\n", ++count);
        printf("ACK : %u\n", ntohl(tcphdr->th_ack));
        printf("SEQ : %u\n", ntohl(tcphdr->th_seq));
        printf("来源: %s:%d\n", inet_ntoa(iphdr->ip_dst),
               ntohs(tcphdr->th_dport));
        printf("目的: %s:%d\n", inet_ntoa(iphdr->ip_src),
               ntohs(tcphdr->th_sport));
        tcp_rst_send(tcphdr->th_ack, 0, iphdr->ip_dst.s_addr,
                     iphdr->ip_src.s_addr, tcphdr->th_dport, tcphdr->th_sport,
                     0);
    }

    return 0;
}
