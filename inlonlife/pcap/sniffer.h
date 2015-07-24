#ifndef _SNIFFER_H_
#define _SNIFFER_H_

#include <stdlib.h>
#include <ctype.h>
#include <netinet/ip.h>
#include <sys/queue.h>

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

void process_ip(const void *tail, size_t len);
struct arphdr *process_arp(const u_char *packet);

void process_icmp(const void *hdr);
struct tcphdr *process_tcp(const void *hdr, size_t len);
struct udphdr *process_udp(const void *hdr);

void *process_dns(const struct udphdr *udp);
struct dns_question *process_dns_hdr(const struct dns_header *dns, void *tail);
void *process_answer(const struct dns_header *dns, void *tail);
size_t get_domain_name(const void *head, void *qname, char *dst);

int is_http(void *data, size_t len);
void process_http(struct tcphdr *tcp,void *data, size_t len);

void print_mem(const void *mem, size_t len);

struct tcp_payload {
    unsigned long seq;
    char *data;
    size_t len;
    SLIST_ENTRY(tcp_payload) entries;
};
SLIST_HEAD(tcp_packs, tcp_payload);
struct tcp_packs tcps_head = SLIST_HEAD_INITIALIZER(&tcps_head);

void save_tcp_payload(unsigned long seq, const void *payload, size_t len);
struct tcp_payload *find_tcp_by_seq(unsigned long seq);
void free_tcp_payload(struct tcp_payload *np);

#endif

