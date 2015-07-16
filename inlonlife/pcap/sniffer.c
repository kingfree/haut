#include <pcap.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <netinet/if_ether.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>
#include <netinet/tcp.h>
#include <netinet/udp.h>
#include <net/if_arp.h>
#include <arpa/inet.h>

#define MAXBYTE2CAPTURE 2048

void process_packet(const struct pcap_pkthdr *pkthdr, const u_char *packet)
{
    int i;
    if (packet == NULL) {
        return;
    }
    printf("\n大小: %d\n", pkthdr->len);

    struct ether_header *ethheader = (struct ether_header *)packet;
    printf("目的地址: ");
    for (i = 0; i < 6; i++) printf("%02X:", ethheader->ether_dhost[i]);
    printf("\n来源地址: ");
    for (i = 0; i < 6; i++) printf("%02X:", ethheader->ether_shost[i]);

    u_short type = ntohs(ethheader->ether_type);
    printf("\n类型: %04x\n", type);

    if (type == ETHERTYPE_IP) {
        printf("=== %s ===\n", "IPv4");
        struct ip *ip = (struct ip *)(packet + sizeof(struct ether_header));
        printf("版本: %d\n", ip->ip_v);
        printf("首部长度: %d\n", ip->ip_hl);
        printf("服务类型(TOS): 0x%02x", ip->ip_tos);
        if (ip->ip_tos & IPTOS_LOWDELAY) {
            printf(" 最小时延");
        }
        if (ip->ip_tos & IPTOS_THROUGHPUT) {
            printf(" 最大吞吐量");
        }
        if (ip->ip_tos & IPTOS_RELIABILITY) {
            printf(" 最高可靠性");
        }
        if (ip->ip_tos & IPTOS_MINCOST) {
            printf(" 最小费用");
        }
        printf("\n");
        printf("总长度: %d\n", ntohs(ip->ip_len));
        printf("标识: %d\n", ntohs(ip->ip_id));
        printf("偏移: %d\n", ntohs(ip->ip_off) & IP_OFFMASK);
        printf("生存时间(TTL): %d\n", ip->ip_ttl);
        printf("协议: %d\n", ip->ip_p);
        printf("标识: %d\n", ntohs(ip->ip_id));
        printf("校验和: %x\n", ip->ip_sum);
        printf("来源IP地址: %s\n", inet_ntoa(ip->ip_src));
        printf("目的IP地址: %s\n", inet_ntoa(ip->ip_dst));

        int proto = ip->ip_p;
        u_char *hdr = (u_char *)ip + sizeof(struct ip);
        if (proto == IPPROTO_ICMP) {
            printf("--- %s ---\n", "ICMP");
            struct icmp *ic = (struct icmp *)hdr;
            printf("类型: %d\n", ic->icmp_type);
            printf("代码: %d\n", ic->icmp_code);
            printf("校验和: %x\n", ic->icmp_cksum);
        } else if (proto == IPPROTO_TCP) {
            printf("--- %s ---\n", "TCP");
            struct tcphdr *tcp = (struct tcphdr *)hdr;
            printf("源端口号: %d\n", ntohs(tcp->th_sport));
            printf("目的端口号: %d\n", ntohs(tcp->th_dport));
            printf("序号: %u\n", ntohl(tcp->th_seq));
            printf("确认序号: %u\n", ntohl(tcp->th_ack));
            printf("首部长度: %d\n", tcp->th_off);
            printf("标志:");
            if (tcp->th_flags & TH_FIN ) printf(" [FIN] 完成");
            if (tcp->th_flags & TH_SYN ) printf(" [SYN] 同步"); 
            if (tcp->th_flags & TH_RST ) printf(" [RST] 重连"); 
            if (tcp->th_flags & TH_PUSH) printf(" [PSH] 接收方尽快转交"); 
            if (tcp->th_flags & TH_ACK ) printf(" [ACK] 确认"); 
            if (tcp->th_flags & TH_URG ) printf(" [URG] 紧急指针"); 
            if (tcp->th_flags & TH_ECE ) printf(" [ECE] "); 
            if (tcp->th_flags & TH_CWR ) printf(" [CWR] "); 
            printf("\n窗口大小: %d\n", ntohs(tcp->th_win));
            printf("校验和: %x\n", ntohs(tcp->th_sum));
            printf("紧急指针: %d\n", ntohs(tcp->th_urp));
        } else if (proto == IPPROTO_UDP) {
            printf("--- %s ---\n", "UDP");
            struct udphdr *udp = (struct udphdr *)hdr;
            printf("源端口号: %d\n", ntohs(udp->uh_sport));
            printf("目的端口号: %d\n", ntohs(udp->uh_dport));
            printf("长度: %d\n", ntohs(udp->uh_ulen));
            printf("校验和: %x\n", ntohs(udp->uh_sum));
        }
    } else if (type == ETHERTYPE_ARP) {
        printf("=== %s ===\n", "ARP");
        struct arphdr *arp = (struct arphdr *)(packet + sizeof(struct ether_header));
        printf("硬件类型: %04x ", ntohs(arp->ar_hrd));
        switch (ntohs(arp->ar_hrd)) {
            case ARPHRD_ETHER:
                printf("以太网");
                break;
            case ARPHRD_IEEE802:
                printf("ioken-ring hardware format");
                break;
            case ARPHRD_FRELAY:
                printf("frame relay hardware format");
                break;
            case ARPHRD_IEEE1394:
                printf("IEEE1394 hardware address");
                break;
        }
        printf("\n协议类型: %04x\n", ntohs(arp->ar_pro));
        printf("硬件地址长度: %d\n", arp->ar_hln);
        printf("协议地址长度: %d\n", arp->ar_pln);
        printf("操作: %04x ", ntohs(arp->ar_op));
        switch (ntohs(arp->ar_op)) {
            case ARPOP_REQUEST:
                printf("ARP请求");
                break;
            case ARPOP_REPLY:
                printf("ARP应答");
                break;
            case ARPOP_REVREQUEST:
                printf("RARP请求");
                break;
            case ARPOP_REVREPLY:
                printf("RARP应答");
                break;
            case ARPOP_INVREQUEST:
                printf("identify peer请求");
                break;
            case ARPOP_INVREPLY:
                printf("identify peer应答");
                break;
        }
        u_char *add = (u_char *)&arp->ar_op + sizeof(arp->ar_op);
        printf("\n发送端 MAC 地址: ");
        for (i = 0; i < arp->ar_hln; i++) printf("%02X:", add[i]);
        add += arp->ar_hln;
        printf("\n发送端 IP 地址: ");
        for (i = 0; i < arp->ar_pln; i++) printf("%0d.", add[i]);
        add += arp->ar_pln;
        printf("\n目的端 MAC 地址: ");
        for (i = 0; i < arp->ar_hln; i++) printf("%02X:", add[i]);
        add += arp->ar_hln;
        printf("\n目的端 IP 地址: ");
        for (i = 0; i < arp->ar_pln; i++) printf("%0d.", add[i]);
        add += arp->ar_pln;
        printf("\n");

    } else if (type == ETHERTYPE_REVARP) {
        printf("=== %s ===\n", "RARP");
    } else {
        printf("=== %s ===\n", "未知");
    }

    //  printf("内容: \n");
    //  for (size_t i = 0; i < pkthdr->len; i++) {
    //      printf("%02x ", packet[i]);
    //      if (((i + 1) % 16 == 0 && i != 0) || i == pkthdr->len - 1) {
    //          printf("\n");
    //      }
    //  }

    return;
}

int main(int argc, char *argv[])
{
    bpf_u_int32 netaddr = 0, mask = 0;
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t *descr = NULL;
    struct pcap_pkthdr pkthdr;
    const unsigned char *packet = NULL;
    memset(errbuf, 0, PCAP_ERRBUF_SIZE);

    if (argc < 2) {
        printf("用法: %s <接口>\n", argv[0]);
        exit(1);
    }

    descr = pcap_open_live(argv[1], MAXBYTE2CAPTURE, 0, 512, errbuf);
    pcap_lookupnet(argv[1], &netaddr, &mask, errbuf);

    // struct bpf_program filter;
    // pcap_compile(descr, &filter, "tcp", 1, mask);
    // pcap_setfilter(descr, &filter);

    for (;;) {
        packet = pcap_next(descr, &pkthdr);
        process_packet(&pkthdr, packet);
    }

    return 0;
}

