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

struct dns_header {
    unsigned id : 16;

#if BYTE_ORDER == BIG_ENDIAN
    unsigned qr : 1;
    unsigned opcode : 4;
    unsigned aa : 1;
    unsigned tc : 1;
    unsigned rd : 1;

    unsigned ra : 1;
    unsigned zero : 3;
    unsigned rcode : 4;
#else
    unsigned rd : 1;
    unsigned tc : 1;
    unsigned aa : 1;
    unsigned opcode : 4;
    unsigned qr : 1;

    unsigned rcode : 4;
    unsigned zero : 3;
    unsigned ra : 1;
#endif

    unsigned qdcount : 16;
    unsigned ancount : 16;
    unsigned nscount : 16;
    unsigned arcount : 16;
};

struct dns_question {
    // u_char qname[32]; 不定长
    unsigned qtype : 16;
#define QTYPE_A      1
#define QTYPE_NS     2
#define QTYPE_CNAME  5
#define QTYPE_PTR   12
#define QTYPE_HINFO 13
#define QTYPE_MX    15
#define QTYPE_AXFR 252
#define QTYPE_ANY  255
    unsigned qclass : 16;
};

struct dns_resource_record {
    // unsigned rr_name : 32; 不定长
    // unsigned rr_type : 16;
    // unsigned rr_class : 16;
    // 同查询报文 不再使用
    unsigned rr_ttl : 32;
    unsigned rr_rdlength : 16;
    u_char rr_data[0];
};

#define MAXBYTE2CAPTURE 2048

void print_mem(void *mem, size_t len)
{
    u_char *ch = (u_char *)mem;
    for (size_t i = 0; i < len; i++) {
        printf("%02x ", ch[i]);
        if (((i + 1) % 16 == 0 && i != 0) || i == len - 1) {
            printf("\n");
        }
    }
}

struct compression_name {
#if BYTE_ORDER == BIG_ENDIAN
    unsigned mark : 2;
    unsigned off : 6;
    unsigned set : 8;
#else
    unsigned off : 6;
    unsigned mark : 2;
    unsigned set : 8;
#endif
};

size_t get_domain_name(void *head, void *qname, char *dst)
{
    int i = 0, j = 0, l = 0;
    struct compression_name* cn;
    u_char* p = (u_char *)qname;
    while (p[i]) {
        cn = (struct compression_name *)(p + i);
        if (cn->mark == 3) { // 11
            printf("压缩: %x %x %x\n", cn->mark, cn->off, cn->set);
            l = get_domain_name(head, (char *)head + ((cn->off << 8) + cn->set), dst + j);
            j += l;
            i += 2;
        } else if (p[i] == 0) {
            break;
        } else {
            printf("正常: %x\n", p[i]);
            for (int k = 1; k <= p[i]; k++) {
                dst[j++] = p[k + i];
            }
            dst[j++] = '.';
            l = p[i];
            j += l;
            i += l + 1;
        }
    }
    dst[j - 1] = '\0';
    return i;
}

void* process_dns(struct dns_header *dns, void* tail) {
    static char qname[64];
    int qlen = get_domain_name(dns, tail, qname);
    struct dns_question *dq = (struct dns_question*)((char *)tail + qlen);
    printf("域名: %s\n", qname);
    short qtype = ntohs(dq->qtype); 
    printf("类型: (%d) ", qtype);
    switch (qtype) {
        case QTYPE_A:
            printf("A IP地址");
            break;
        case QTYPE_NS:
            printf("NS 名字服务器");
            break;
        case QTYPE_CNAME:
            printf("CNAME 规范名称");
            break;
        case QTYPE_PTR:
            printf("PTR 指针记录");
            break;
        case QTYPE_HINFO:
            printf("HINFO 主机信息");
            break;
        case QTYPE_MX:
            printf("MX 邮件交换记录");
            break;
        case QTYPE_AXFR:
            printf("AXFR 对区域转换的请求");
            break;
        case QTYPE_ANY:
            printf("ANY 对所有记录的请求");
            break;
    }
    short qclass = ntohs(dq->qclass);
    printf("\n类: (%d) %s\n", qclass, qclass == 1 ? "IPv4" : "其他");
    return dq + 1;
}

void process_packet(const struct pcap_pkthdr* pkthdr, const u_char* packet)
{
    int i;
    if (packet == NULL) {
        return;
    }
    printf("\n大小: %d\n", pkthdr->len);

    struct ether_header* ethheader = (struct ether_header*)packet;
    printf("目的地址: ");
    for (i = 0; i < 6; i++)
        printf("%02X:", ethheader->ether_dhost[i]);
    printf("\n来源地址: ");
    for (i = 0; i < 6; i++)
        printf("%02X:", ethheader->ether_shost[i]);

    u_short type = ntohs(ethheader->ether_type);
    printf("\n类型: %04x\n", type);

    if (type == ETHERTYPE_IP) {
        printf("=== %s ===\n", "IPv4");
        struct ip* ip = (struct ip*)(packet + sizeof(struct ether_header));
        printf("版本: %d\n", ip->ip_v);
        printf("首部长度: %d\n", ip->ip_hl);
        printf("服务类型(TOS): 0x%02x", ip->ip_tos);
        if (ip->ip_tos & IPTOS_LOWDELAY)
            printf(" 最小时延");
        if (ip->ip_tos & IPTOS_THROUGHPUT)
            printf(" 最大吞吐量");
        if (ip->ip_tos & IPTOS_RELIABILITY)
            printf(" 最高可靠性");
        if (ip->ip_tos & IPTOS_MINCOST)
            printf(" 最小费用");
        printf("\n");
        printf("总长度: %d\n", ntohs(ip->ip_len) * 4);
        printf("标识: %d\n", ntohs(ip->ip_id));
        printf("偏移: %d\n", ntohs(ip->ip_off) & IP_OFFMASK);
        printf("生存时间(TTL): %d\n", ip->ip_ttl);
        printf("协议: %d\n", ip->ip_p);
        printf("标识: %d\n", ntohs(ip->ip_id));
        printf("校验和: %x\n", ip->ip_sum);
        printf("来源IP地址: %s\n", inet_ntoa(ip->ip_src));
        printf("目的IP地址: %s\n", inet_ntoa(ip->ip_dst));

        int proto = ip->ip_p;
        u_char* hdr = (u_char*)ip + sizeof(struct ip);
        if (proto == IPPROTO_ICMP) {
            printf("--- %s ---\n", "ICMP");
            struct icmp* ic = (struct icmp*)hdr;
            printf("类型: %d\n", ic->icmp_type);
            printf("代码: %d\n", ic->icmp_code);
            printf("校验和: 0x%x\n", ic->icmp_cksum);
        } else if (proto == IPPROTO_TCP) {
            printf("--- %s ---\n", "TCP");
            struct tcphdr* tcp = (struct tcphdr*)hdr;
            printf("源端口号: %d\n", ntohs(tcp->th_sport));
            printf("目的端口号: %d\n", ntohs(tcp->th_dport));
            printf("序号: %u\n", ntohl(tcp->th_seq));
            printf("确认序号: %u\n", ntohl(tcp->th_ack));
            printf("首部长度: %d\n", tcp->th_off);
            printf("标志:");
            if (tcp->th_flags & TH_FIN)
                printf(" [FIN] 完成");
            if (tcp->th_flags & TH_SYN)
                printf(" [SYN] 同步");
            if (tcp->th_flags & TH_RST)
                printf(" [RST] 重连");
            if (tcp->th_flags & TH_PUSH)
                printf(" [PSH] 接收方尽快转交");
            if (tcp->th_flags & TH_ACK)
                printf(" [ACK] 确认");
            if (tcp->th_flags & TH_URG)
                printf(" [URG] 紧急指针");
            if (tcp->th_flags & TH_ECE)
                printf(" [ECE] ");
            if (tcp->th_flags & TH_CWR)
                printf(" [CWR] ");
            printf("\n窗口大小: %d\n", ntohs(tcp->th_win));
            printf("校验和: 0x%x\n", ntohs(tcp->th_sum));
            printf("紧急指针: %d\n", ntohs(tcp->th_urp));
        } else if (proto == IPPROTO_UDP) {
            printf("--- %s ---\n", "UDP");
            struct udphdr* udp = (struct udphdr*)hdr;
            unsigned short sport, dport;
            printf("源端口号: %d\n", sport = ntohs(udp->uh_sport));
            printf("目的端口号: %d\n", dport = ntohs(udp->uh_dport));
            printf("长度: %d\n", ntohs(udp->uh_ulen));
            printf("校验和: 0x%x\n", ntohs(udp->uh_sum));
            if (sport == 53 || dport == 53) {
                printf("+++ %s +++\n", "DNS");
                struct dns_header* dns = (struct dns_header*)(udp + 1);
                printf("标识: 0x%04x\n", ntohs(dns->id));
                printf("报文类型: (%d) %s\n", dns->qr, dns->qr ? "响应" : "查询");
                static char* opstring[] = {"标准查询", "反向查询", "服务器状态请求"};
                unsigned short opcode = dns->opcode;
                printf("查询类型: (%d) %s\n", opcode, opcode > 2 ? "其他" : opstring[opcode]);
                printf("授权回答: (%d) %s\n", dns->aa, dns->aa ? "是": "否");
                printf("可截断的: (%d) %s\n", dns->tc, dns->tc ? "是": "否");
                printf("期望递归: (%d) %s\n", dns->rd, dns->rd ? "是": "否");
                printf("可用递归: (%d) %s\n", dns->ra, dns->ra ? "是": "否");
                printf("返回码: %d\n", dns->rcode);
                unsigned short qdcount = ntohs(dns->qdcount);
                unsigned short ancount = ntohs(dns->ancount);
                unsigned short nscount = ntohs(dns->nscount);
                unsigned short arcount = ntohs(dns->arcount);
                printf("问题数: %d\n", qdcount);
                printf("资源记录数: %d\n", ancount);
                printf("授权资源记录数: %d\n", nscount);
                printf("额外资源记录数: %d\n", arcount);
                void *tail = dns + 1;
                for (i = 0; i < qdcount; i++) {
                    printf("[问题%2d]\n", i + 1);
                    tail = process_dns(dns, tail);
                }
                for (i = 0; i < ancount; i++) {
                    printf("[资源%2d]\n", i + 1);
                    tail = process_dns(dns, tail);
                    struct dns_resource_record *rr = (struct dns_resource_record *)tail;
                    printf("生存时间: %d s\n", ntohl(rr->rr_ttl));
                    unsigned short rlen = ntohs(rr->rr_rdlength);
                    if (rlen == 4) {
                        struct in_addr *ip = (struct in_addr *)rr->rr_data;
                        printf("资源地址: %s\n", inet_ntoa(*ip));
                    }
                    tail = rr->rr_data + rlen;
                }
            }
        }
    }
    else if (type == ETHERTYPE_ARP) {
        printf("=== %s ===\n", "ARP");
        struct arphdr* arp = (struct arphdr*)(packet + sizeof(struct ether_header));
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
        u_char* add = (u_char*)&arp->ar_op + sizeof(arp->ar_op);
        printf("\n发送端 MAC 地址: ");
        for (i = 0; i < arp->ar_hln; i++)
            printf("%02X:", add[i]);
        add += arp->ar_hln;
        printf("\n发送端 IP 地址: ");
        for (i = 0; i < arp->ar_pln; i++)
            printf("%0d.", add[i]);
        add += arp->ar_pln;
        printf("\n目的端 MAC 地址: ");
        for (i = 0; i < arp->ar_hln; i++)
            printf("%02X:", add[i]);
        add += arp->ar_hln;
        printf("\n目的端 IP 地址: ");
        for (i = 0; i < arp->ar_pln; i++)
            printf("%0d.", add[i]);
        add += arp->ar_pln;
        printf("\n");
    }
    else if (type == ETHERTYPE_REVARP) {
        printf("=== %s ===\n", "RARP");
    }
    else {
        printf("=== %s ===\n", "未知");
    }

    return;
}

int main(int argc, char* argv[])
{
    bpf_u_int32 netaddr = 0, mask = 0;
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t* descr = NULL;
    struct pcap_pkthdr pkthdr;
    const unsigned char* packet = NULL;
    memset(errbuf, 0, PCAP_ERRBUF_SIZE);

    if (argc < 2) {
        printf("用法: %s <接口>\n", argv[0]);
        exit(1);
    }

    descr = pcap_open_live(argv[1], MAXBYTE2CAPTURE, 0, 512, errbuf);
    pcap_lookupnet(argv[1], &netaddr, &mask, errbuf);

    struct bpf_program filter;
    pcap_compile(descr, &filter, "udp", 1, mask);
    pcap_setfilter(descr, &filter);

    for (;;) {
        packet = pcap_next(descr, &pkthdr);
        process_packet(&pkthdr, packet);
    }

    return 0;
}
