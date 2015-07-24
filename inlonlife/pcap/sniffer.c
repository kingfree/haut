#include <pcap.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <ctype.h>
#include <stdarg.h>
#include <netinet/if_ether.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>
#include <netinet/tcp.h>
#include <netinet/udp.h>
#include <net/if_arp.h>
#include <arpa/inet.h>
#include <sys/queue.h>
#include <zlib.h>

#include "sniffer.h"
#include "http_parser.h"

#define koko \
    printf("line %d: ここまで\n", __LINE__)

struct capture_state {
    char ip;
    char icmp;
    char tcp;
    char http;
    char udp;
    char dns;
    char arp;
};

static struct capture_state config;

struct buffer {
#define BUFF_SIZE 4096
    char buf[BUFF_SIZE];
    long len;
    char *p;
};

struct buffer print_buffer;

void update_buf() { print_buffer.p = print_buffer.buf + print_buffer.len; }

void clear_buf()
{
    print_buffer.len = 0;
    print_buffer.buf[0] = 0;
    update_buf();
}

void print_buf()
{
    puts(print_buffer.buf);
    clear_buf();
}

void pf(const char *fmt, ...)
{
    va_list p;

    va_start(p, fmt);
    long len = vsnprintf(print_buffer.p, BUFF_SIZE - print_buffer.len, fmt, p);
    if ((print_buffer.len = len + print_buffer.len) < BUFF_SIZE) update_buf();
    va_end(p);
}

void print_mem(const void *mem, size_t len)
{
    fprintf(stderr, "[数据长度: %ld]\n", len);
    u_char *data = (u_char *)mem;
    size_t i, j;
    for (i = 0; i < len; i++) {
        if (i != 0 && i % 16 == 0) {
            fprintf(stderr, "         ");
            for (j = i - 16; j < i; j++)
                fprintf(stderr, "%c", isprint(data[j]) ? data[j] : '.');
            fprintf(stderr, "\n");
        }

        if (i % 16 == 0) fprintf(stderr, "   ");
        fprintf(stderr, " %02X", data[i]);

        if (i == len - 1) {
            for (j = 0; j < 15 - i % 16; j++)
                fprintf(stderr, "   ");

            fprintf(stderr, "         ");
            for (j = i - i % 16; j <= i; j++)
                fprintf(stderr, "%c", isprint(data[j]) ? data[j] : '.');
            fprintf(stderr, "\n");
        }
    }
}

size_t get_domain_name(const void *head, void *qname, char *dst)
{
    int i = 0, j = 0, l = 0;
    struct compression_name *cn;
    u_char *p = (u_char *)qname;
    while (1) {
        cn = (struct compression_name *)(p + i);
        if (cn->mark == 3) {  // 11
            short offset = ((cn->off << 8) + cn->set);
            // pf("压缩: %x %x %x %d\n", cn->mark, cn->off, cn->set,
            // offset);
            l = get_domain_name(head, (char *)head + offset, dst + j);
            j += strlen(dst + j);
            dst[j++] = '.';
            i += 2;
            break;
        } else if (p[i] == 0) {
            i += 1;
            // pf("结束: [%d]\n", i);
            break;
        } else {
            // pf("正常: %x\n", p[i]);
            for (int k = 1; k <= p[i]; k++) dst[j++] = p[k + i];
            dst[j++] = '.';
            l = p[i];
            i += l + 1;
        }
    }
    dst[j - 1] = '\0';
    return i;
}

struct dns_question *process_dns_hdr(const struct dns_header *dns, void *tail)
{
    static char qname[64];
    int qlen = get_domain_name(dns, tail, qname);
    // pf("[qlen=%d]\n", qlen);
    struct dns_question *dq = (struct dns_question *)((char *)tail + qlen);
    pf("域名: %s\n", qname);
    // print_mem(dq, 20);
    short qtype = ntohs(dq->qtype);
    pf("类型: (%d) ", dq->qtype = qtype);
    switch (qtype) {
    case QTYPE_A:
        pf("A IP地址");
        break;
    case QTYPE_NS:
        pf("NS 名字服务器");
        break;
    case QTYPE_CNAME:
        pf("CNAME 规范名称");
        break;
    case QTYPE_PTR:
        pf("PTR 指针记录");
        break;
    case QTYPE_HINFO:
        pf("HINFO 主机信息");
        break;
    case QTYPE_MX:
        pf("MX 邮件交换记录");
        break;
    case QTYPE_AXFR:
        pf("AXFR 对区域转换的请求");
        break;
    case QTYPE_ANY:
        pf("ANY 对所有记录的请求");
        break;
    }
    short qclass = ntohs(dq->qclass);
    pf("\n类: (%d) %s\n", dq->qclass = qclass, qclass == 1 ? "IPv4" : "其他");
    return dq;
}

void *process_answer(const struct dns_header *dns, void *tail)
{
    struct dns_question *dq = process_dns_hdr(dns, tail);
    tail = dq + 1;
    struct dns_resource_record *rr = (struct dns_resource_record *)tail;
    pf("生存时间: %d s\n", ntohl(rr->rr_ttl));
    unsigned short rlen = ntohs(rr->rr_rdlength);
    pf("数据长度: %d\n", rlen);
    if (dq->qtype == QTYPE_A) {
        pf("资源地址: %s\n", inet_ntoa(*(struct in_addr *)rr->rr_data));
    } else if (dq->qtype == QTYPE_CNAME) {
        static char qname[64];
        get_domain_name(dns, rr->rr_data, qname);
        pf("规范名称: %s\n", qname);
    }
    tail = rr->rr_data + rlen;
    return tail;
}

void save_tcp_payload(unsigned long seq, const void *payload, size_t len)
{
    struct tcp_payload *t = malloc(sizeof(struct tcp_payload));
    char *data = malloc(len);
    memcpy(data, payload, len);
    t->seq = seq;
    t->data = data;
    t->len = len;
    SLIST_INSERT_HEAD(&tcps_head, t, entries);
}

struct tcp_payload *find_tcp_by_seq(unsigned long seq)
{
    struct tcp_payload *i, *tmp;
    SLIST_FOREACH_SAFE(i, &tcps_head, entries, tmp) {
        if (i->seq == seq)
            return i;
    }
    return NULL;
}

void free_tcp_payload(struct tcp_payload *np)
{
    if (!np) return;
    if (np->data) free(np->data);
    SLIST_REMOVE(&tcps_head, np, tcp_payload, entries);
    free(np);
}

struct tcphdr *process_tcp(const void *hdr, size_t len)
{
    pf("--- %s ---\n", "TCP");
    pf("[len=%ld]\n", len);
    struct tcphdr *tcp = (struct tcphdr *)hdr;
    size_t tcplen = 0;
    unsigned long seq = 0;
    pf("源端口号: %d\n", ntohs(tcp->th_sport));
    pf("目的端口号: %d\n", ntohs(tcp->th_dport));
    pf("序号: %u\n", seq = ntohl(tcp->th_seq));
    pf("确认序号: %u\n", ntohl(tcp->th_ack));
    pf("首部长度: %d\n", tcplen = tcp->th_off * 4);
    char *tail = (char *)tcp + tcplen;
    pf("标志:");
    if (tcp->th_flags & TH_FIN) pf(" [FIN] 完成");
    if (tcp->th_flags & TH_SYN) pf(" [SYN] 同步");
    if (tcp->th_flags & TH_RST) pf(" [RST] 重连");
    if (tcp->th_flags & TH_PUSH) pf(" [PSH] 接收方尽快转交");
    if (tcp->th_flags & TH_ACK) pf(" [ACK] 确认");
    if (tcp->th_flags & TH_URG) pf(" [URG] 紧急指针");
    if (tcp->th_flags & TH_ECE) pf(" [ECE] ");
    if (tcp->th_flags & TH_CWR) pf(" [CWR] ");
    pf("\n窗口大小: %d\n", ntohs(tcp->th_win));
    pf("校验和: 0x%x\n", ntohs(tcp->th_sum));
    pf("紧急指针: %d\n", ntohs(tcp->th_urp));
    char *now = (char *)(tcp + 1);
    char l;
    while (now != tail) {
        switch (*now) {
        case 0:
            pf("结束\n");
            now++;
            break;
        case 1:
            pf("无操作\n");
            now++;
            break;
        case 2:
            now += 2;
            pf("最大报文段长度: %d\n", ntohs(*now));
            now += 2;
            break;
        case 3:
            now += 2;
            pf("窗口缩放因子位移值: %d\n", *now);
            now += 1;
            break;
        case 8:
            now += 2;
            pf("时间戳值: %u\n", ntohl(*now));
            now += 4;
            pf("时间戳回显应答: %u\n", ntohl(*now));
            now += 4;
            break;
        default:
            l = *(now + 1);
            pf("未知选项: %d, 长度 %d\n", *now, l);
            now += l;
        }
    }
    if (config.tcp) print_buf();
    len -= tcplen;
    if (len > 0) {
        save_tcp_payload(seq, tail, len);
        // struct tcp_payload *tp = SLIST_FIRST(tcps_head);
        // if (tp) printf("%ld\n", tp->len);
        process_http(tcp, tail, len);
    }
    return tcp;
}

int is_http_head(void *data)
{
    char *http = (char *)data;
    if (strncmp(http, "HTTP", 4) == 0) return 1;
    if (strncmp(http, "GET", 3) == 0) return 2;
    if (strncmp(http, "HEAD", 4) == 0) return 3;
    if (strncmp(http, "POST", 4) == 0) return 4;
    if (strncmp(http, "PUT", 3) == 0) return 5;
    if (strncmp(http, "DELETE", 6) == 0) return 6;
    if (strncmp(http, "TRACE", 5) == 0) return 7;
    if (strncmp(http, "CONNECT", 7) == 0) return 8;
    // char *res = strnstr(http, "\r\n", len);
    // if (res) return res - http;
    return 0;
}

typedef struct string {
    char *data;
    size_t len;
    size_t size;
} string;

struct http_data {
    char has;
    http_parser *parser;
    http_parser_settings settings;
    string head;
    string body;
    uint64_t hope_len;
    unsigned long next_seq;
};

string *stringncat(string *s1, const char *s2, size_t n)
{
koko;
    if (s1->len + n + 1 >= s1->size) {
        s1->size += n + 1 > 128 ? n + 1 : 128;
    koko;
        if (s1->data == NULL) {
    koko;
            s1->data = malloc(s1->size);
    koko;
        } else {
    koko;
            s1->data = realloc(s1->data, s1->size);
    koko;
        }
    koko;
        if (s1->data == NULL) return NULL;
    }
    koko;
    char *res = strncat(s1->data + s1->len, s2, n);
    printf("%ld %ld %s %ld\n", s1->len, s1->size, s1->data, n);
    s1->data[s1->len] = '\0'; // strncat() 会自动加 '\0'
    koko;
    return res == s1->data ? s1 : NULL;
}

void stringfree(string *s)
{
    if (s->data)
        free(s->data);
    s->data = NULL;
    s->len = s->size = 0;
}

struct http_data httpd;

int http_on_header_field(http_parser *p, const char *buf, size_t len)
{
    if (p != httpd.parser) return 0;
    koko;
    stringncat(&httpd.head, buf, len);
    koko;
    stringncat(&httpd.head, ": ", 2);
    koko;
    return 0;
}

int http_on_header_value(http_parser *p, const char *buf, size_t len)
{
    if (p != httpd.parser) return 0;
    stringncat(&httpd.head, buf, len);
    stringncat(&httpd.head, "\n", 1);
    return 0;
}

int http_on_headers_complete(http_parser *p)
{
    if (p != httpd.parser) return 0;
    httpd.hope_len = p->content_length;
    return 0;
}

int http_on_message_complete(http_parser *p)
{
    if (p != httpd.parser) return 0;
    httpd.hope_len = 0;
    return 0;
}

int http_on_body(http_parser *p, const char *buf, size_t len)
{
    if (p != httpd.parser) return 0;
    stringncat(&httpd.body, buf, len);
    return 0;
}

void clear_httpd()
{
    httpd.has = 0;
    httpd.hope_len = 0;
    httpd.next_seq = 0;
    stringfree(&httpd.head);
    stringfree(&httpd.body);
}

void process_http(struct tcphdr *tcp, void *data, size_t len)
{
    if (!config.http)
        return;
    else
        print_buf();
    char *http = (char *)data;
    http[len] = '\0';
    unsigned long seq = ntohl(tcp->th_seq);
    if (is_http_head(data)) {
        if (httpd.has) return;
        httpd.has = 1;
        httpd.next_seq = seq + len;
        // httpd.data = realloc(httpd.data, httpd.len + len);
        // memcpy(httpd.data + httpd.len, http, len);
        http_parser_init(httpd.parser, HTTP_BOTH);

        http_parser_execute(httpd.parser, &httpd.settings, http, len);
        if (!httpd.hope_len) {
            printf("+++ %s +++\n", "HTTP");
koko;
            puts(httpd.head.data);
koko;
            clear_httpd();
koko;
        }
        /* size_t hope = process_http_header(http, len);
        if (hope < len) {
            process_http_body(http + hope, len - hope);
        } else if (hope == len) {
            httpd.has = 0;
        } */
    } else if (httpd.has) {
        while (seq < httpd.next_seq) {
            struct tcp_payload *tp;
            tp = find_tcp_by_seq(seq);
            if (!tp) {
                clear_httpd();
                break;
            }
            http_parser_execute(httpd.parser, &httpd.settings, tp->data, tp->len);
            seq = tp->seq + tp->len;
        }
        httpd.next_seq = seq + len;
        if (!httpd.hope_len) {
            puts(httpd.body.data);
            clear_httpd();
        }
    }

    /* long i;
    printf("+++ %s +++\n", "HTTP");
    long length = 0, tmp;
    int text = 0, gzip = 0, chunked = 0;
    static char tmpt[64];
    while (strncmp(http, "\r\n\r\n", 4)) {
        char *eol = strchr(http, '\r');
        if (!eol) break;
        if (eol[1] == '\n') {
            *eol = '\0';
            printf("%s\n", http);
            if (sscanf(http, "Content-Length: %ld", &tmp) == 1) {
                length = tmp;
            } else if (sscanf(http, "Content-Type: %s", tmpt) == 1) {
                if (strncmp(tmpt, "text/", 5) == 0 ||
                    strncmp(tmpt, "application/j", 13) == 0)
                    text = 1;
            } else if (sscanf(http, "Content-Encoding: %s", tmpt) == 1) {
                if (strncmp(tmpt, "gzip", 4) == 0) gzip = 1;
            } else if (sscanf(http, "Transfer-Encoding: %s", tmpt) == 1) {
                if (strncmp(tmpt, "chunked", 7) == 0) chunked = 1;
            }
            http = eol + 2;
            if (http[0] == '\r' && http[1] == '\n') {
                http += 2;
                break;
            }
        }
    }
    char *body = http;
    size_t headlen = body - (char *)data;
    print_mem(data, headlen);
    fprintf(stderr, "\n");
    print_mem(body, len - headlen);
    printf("\n");
    if (text) {
        if (chunked) {
            unsigned long block = 0;
            do {
                if (sscanf(http, "%lx\r\n", &block) == 1) {
                    switch (strlen(tmpt)) {
                    case 1:
                        block = *(u_int8_t *)tmpt + 1;
                        break;
                    case 2:
                        block = ntohs(*(u_int16_t *)tmpt + 1);
                        break;
                    case 3:
                        tmpt[0] = 0;
                        block = ntohl(*(u_int32_t *)tmpt);
                        break;
                    case 4:
                        block = ntohl(*(u_int32_t *)tmpt + 1);
                        break;
                    default:
                        block = 0;
                    }
                    char *eol = strchr(http, '\r');
                    if (!eol) break;
                    http = eol + 2;
                    unsigned long now = len - (http - (char *)data);
                    static unsigned char co[BUFF_SIZE];
                    static unsigned char de[BUFF_SIZE];
                    unsigned long l = block > now ? now : block;
                    memcpy(co, http, l);
                    unsigned char *po = co;
                    if (gzip) {
                        uncompress(de, &block, co, l);
                        po = de;
                    }
                    for (unsigned long i = 0; i < block; i++)
                        if (isprint(http[i])) printf("%c", po[i]);
                    if (block > now) {
                        printf("\n截断在%ld处, 期望到%ld\n", now, block);
                        length += now;
                        break;
                    }
                    length += block;
                    http += block + 2;
                }
            } while (block);
        } else {
            if (length == 0) length = len - (http - (char *)data);
            for (i = 0; i < length; i++)
                if (isprint(http[i])) printf("%c", http[i]);
        }
        printf("\n正文长度: %ld\n", length);
    } else if (length) {
        printf("不可打印的数据 [%ld]\n", length);
    } */
}

void *process_dns(const struct udphdr *udp)
{
    int i;
    pf("+++ %s +++\n", "DNS");
    struct dns_header *dns = (struct dns_header *)(udp + 1);
    pf("标识: 0x%04x\n", ntohs(dns->id));
    pf("报文类型: (%d) %s\n", dns->qr, dns->qr ? "响应" : "查询");
    static char *opstring[] = {"标准查询", "反向查询", "服务器状态请求"};
    unsigned short opcode = dns->opcode;
    pf("查询类型: (%d) %s\n", opcode, opcode > 2 ? "其他" : opstring[opcode]);
    pf("授权回答: (%d) %s\n", dns->aa, dns->aa ? "是" : "否");
    pf("可截断的: (%d) %s\n", dns->tc, dns->tc ? "是" : "否");
    pf("期望递归: (%d) %s\n", dns->rd, dns->rd ? "是" : "否");
    pf("可用递归: (%d) %s\n", dns->ra, dns->ra ? "是" : "否");
    pf("返回码: %d\n", dns->rcode);
    unsigned short qdcount = ntohs(dns->qdcount);
    unsigned short ancount = ntohs(dns->ancount);
    unsigned short nscount = ntohs(dns->nscount);
    unsigned short arcount = ntohs(dns->arcount);
    pf("问题数: %d\n", qdcount);
    pf("资源记录数: %d\n", ancount);
    pf("授权资源记录数: %d\n", nscount);
    pf("额外资源记录数: %d\n", arcount);
    void *tail = dns + 1;
    for (i = 0; i < qdcount; i++) {
        pf("[问题%2d]\n", i + 1);
        tail = process_dns_hdr(dns, tail) + 1;
    }
    for (i = 0; i < ancount; i++) {
        pf("[资源%2d]\n", i + 1);
        tail = process_answer(dns, tail);
    }
    for (i = 0; i < nscount; i++) {
        pf("[授权资源%2d]\n", i + 1);
        tail = process_answer(dns, tail);
    }
    for (i = 0; i < arcount; i++) {
        pf("[额外资源%2d]\n", i + 1);
        tail = process_answer(dns, tail);
    }
    if (config.dns) print_buf();
    return tail;
}

struct udphdr *process_udp(const void *hdr)
{
    pf("--- %s ---\n", "UDP");
    struct udphdr *udp = (struct udphdr *)hdr;
    unsigned short sport, dport;
    pf("源端口号: %d\n", sport = ntohs(udp->uh_sport));
    pf("目的端口号: %d\n", dport = ntohs(udp->uh_dport));
    pf("长度: %d\n", ntohs(udp->uh_ulen));
    pf("校验和: 0x%x\n", ntohs(udp->uh_sum));
    if (config.udp) print_buf();
    if (sport == 53 || dport == 53) {
        process_dns(udp);
    }
    return udp;
}

struct arphdr *process_arp(const u_char *packet)
{
    int i;
    pf("=== %s ===\n", "ARP");
    struct arphdr *arp =
        (struct arphdr *)(packet + sizeof(struct ether_header));
    pf("硬件类型: %04x ", ntohs(arp->ar_hrd));
    switch (ntohs(arp->ar_hrd)) {
    case ARPHRD_ETHER:
        pf("以太网");
        break;
    case ARPHRD_IEEE802:
        pf("ioken-ring hardware format");
        break;
    case ARPHRD_FRELAY:
        pf("frame relay hardware format");
        break;
    case ARPHRD_IEEE1394:
        pf("IEEE1394 hardware address");
        break;
    }
    pf("\n协议类型: %04x\n", ntohs(arp->ar_pro));
    pf("硬件地址长度: %d\n", arp->ar_hln);
    pf("协议地址长度: %d\n", arp->ar_pln);
    pf("操作: %04x ", ntohs(arp->ar_op));
    switch (ntohs(arp->ar_op)) {
    case ARPOP_REQUEST:
        pf("ARP请求");
        break;
    case ARPOP_REPLY:
        pf("ARP应答");
        break;
    case ARPOP_REVREQUEST:
        pf("RARP请求");
        break;
    case ARPOP_REVREPLY:
        pf("RARP应答");
        break;
    case ARPOP_INVREQUEST:
        pf("identify peer请求");
        break;
    case ARPOP_INVREPLY:
        pf("identify peer应答");
        break;
    }
    u_char *add = (u_char *)&arp->ar_op + sizeof(arp->ar_op);
    pf("\n发送端 MAC 地址: ");
    for (i = 0; i < arp->ar_hln; i++) pf("%02X:", add[i]);
    add += arp->ar_hln;
    pf("\n发送端 IP 地址: ");
    for (i = 0; i < arp->ar_pln; i++) pf("%0d.", add[i]);
    add += arp->ar_pln;
    pf("\n目的端 MAC 地址: ");
    for (i = 0; i < arp->ar_hln; i++) pf("%02X:", add[i]);
    add += arp->ar_hln;
    pf("\n目的端 IP 地址: ");
    for (i = 0; i < arp->ar_pln; i++) pf("%0d.", add[i]);
    add += arp->ar_pln;
    pf("\n");
    if (config.arp) print_buf();
    return arp;
}

void process_icmp(const void *hdr)
{
    pf("--- %s ---\n", "ICMP");
    struct icmp *ic = (struct icmp *)hdr;
    pf("类型: %d\n", ic->icmp_type);
    pf("代码: %d\n", ic->icmp_code);
    pf("校验和: 0x%x\n", ic->icmp_cksum);
    if (config.icmp) print_buf();
}

void process_ip(const void *tail, size_t len)
{
    pf("=== %s ===\n", "IPv4");
    pf("[len=%ld]\n", len);
    struct ip *ip = (struct ip *)(tail);
    pf("版本: %d\n", ip->ip_v);
    pf("首部长度: %d\n", ip->ip_hl * 4);
    pf("服务类型(TOS): 0x%02x", ip->ip_tos);
    if (ip->ip_tos & IPTOS_LOWDELAY) pf(" 最小时延");
    if (ip->ip_tos & IPTOS_THROUGHPUT) pf(" 最大吞吐量");
    if (ip->ip_tos & IPTOS_RELIABILITY) pf(" 最高可靠性");
    if (ip->ip_tos & IPTOS_MINCOST) pf(" 最小费用");
    pf("\n");
    pf("总长度: %d\n", len = ntohs(ip->ip_len));
    pf("标识: %d\n", ntohs(ip->ip_id));
    pf("保留位: %d\n", ntohs(ip->ip_off) & IP_RF);
    pf("不分片: %s\n", ntohs(ip->ip_off) & IP_DF ? "是" : "否");
    pf("更多分片: %s\n", ntohs(ip->ip_off) & IP_MF ? "是" : "否");
    pf("偏移: %d\n", ntohs(ip->ip_off) & IP_OFFMASK);
    pf("生存时间(TTL): %d\n", ip->ip_ttl);
    pf("协议: %d\n", ip->ip_p);
    pf("标识: %d\n", ntohs(ip->ip_id));
    pf("校验和: %x\n", ip->ip_sum);
    pf("来源IP地址: %s\n", inet_ntoa(ip->ip_src));
    pf("目的IP地址: %s\n", inet_ntoa(ip->ip_dst));
    if (config.ip) print_buf();

    int proto = ip->ip_p;
    u_char *hdr = (u_char *)ip + sizeof(struct ip);
    if (proto == IPPROTO_ICMP) {
        process_icmp(hdr);
    } else if (proto == IPPROTO_TCP) {
        process_tcp(hdr, len - sizeof(struct ip));
    } else if (proto == IPPROTO_UDP) {
        process_udp(hdr);
    }
}

void process_packet(const struct pcap_pkthdr *pkthdr, const u_char *packet)
{
    clear_buf();
    int i;
    if (packet == NULL) {
        return;
    }
    pf("\n大小: %d\n", pkthdr->len);

    struct ether_header *ethheader = (struct ether_header *)packet;
    pf("目的地址: ");
    for (i = 0; i < 6; i++) pf("%02X:", ethheader->ether_dhost[i]);
    pf("\n来源地址: ");
    for (i = 0; i < 6; i++) pf("%02X:", ethheader->ether_shost[i]);

    u_short type = ntohs(ethheader->ether_type);
    pf("\n类型: %04x\n", type);

    if (type == ETHERTYPE_IP) {
        long len = sizeof(struct ether_header);
        process_ip(packet + len, pkthdr->len - len);
    } else if (type == ETHERTYPE_ARP) {
        process_arp(packet);
    } else if (type == ETHERTYPE_REVARP) {
        pf("=== %s ===\n", "RARP");
        print_buf();
    } else {
        pf("=== %s ===\n", "未知");
        print_buf();
    }

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
        pf("用法: %s <接口> [过滤器]\n", argv[0]);
        exit(1);
    }

    descr = pcap_open_live(argv[1], MAXBYTE2CAPTURE, 0, 512, errbuf);
    pcap_lookupnet(argv[1], &netaddr, &mask, errbuf);

    if (argc == 2) {
        config.ip = 1;
        config.icmp = 1;
        config.tcp = 1;
        config.http = 1;
        config.udp = 1;
        config.dns = 1;
        config.arp = 1;
    }

    for (int i = 2; i < argc; i++) {
        if (strcmp(argv[i], "ip") == 0)
            config.ip = 1;
        else if (strcmp(argv[i], "icmp") == 0)
            config.icmp = 1;
        else if (strcmp(argv[i], "tcp") == 0)
            config.tcp = 1;
        else if (strcmp(argv[i], "http") == 0)
            config.http = 1;
        else if (strcmp(argv[i], "udp") == 0)
            config.udp = 1;
        else if (strcmp(argv[i], "dns") == 0)
            config.dns = 1;
        else if (strcmp(argv[i], "arp") == 0)
            config.arp = 1;
    }

    SLIST_INIT(&tcps_head);

    httpd.settings.on_header_field = http_on_header_field;
    httpd.settings.on_header_value = http_on_header_value;
    httpd.settings.on_headers_complete = http_on_headers_complete;
    httpd.settings.on_message_complete = http_on_message_complete;
    httpd.settings.on_body = http_on_body;
    httpd.parser = malloc(sizeof(http_parser));

    for (;;) {
        packet = pcap_next(descr, &pkthdr);
        process_packet(&pkthdr, packet);
    }

    return 0;
}
