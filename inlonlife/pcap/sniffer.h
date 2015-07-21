#ifndef _SNIFFER_H_
#define _SNIFFER_H_

#include <ctype.h>
#include <netinet/ip.h>

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
#define QTYPE_A 1
#define QTYPE_NS 2
#define QTYPE_CNAME 5
#define QTYPE_PTR 12
#define QTYPE_HINFO 13
#define QTYPE_MX 15
#define QTYPE_AXFR 252
#define QTYPE_ANY 255
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

void process_ip(const u_char *packet);
struct arphdr *process_arp(const u_char *packet);

void process_icmp(const void *hdr);
struct tcphdr *process_tcp(const void *hdr);
struct udphdr *process_udp(const void *hdr);

void *process_dns(const struct udphdr *udp);
struct dns_question *process_dns_hdr(const struct dns_header *dns, void *tail);
void *process_answer(const struct dns_header *dns, void *tail);
size_t get_domain_name(const void *head, void *qname, char *dst);

void print_mem(void *mem, size_t len);

#endif
