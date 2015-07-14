#include <pcap.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define MAXBYTE2CAPTURE 2048

void process_packet(u_char *arg, const struct pcap_pkthdr *pkthdr, const u_char *packet)
{
    int *counter = (int *) arg;

    printf("序号: %d\n", ++(*counter));
    printf("大小: %d\n", pkthdr->len);
    printf("内容: \n");
    for (size_t i = 0; i < pkthdr->len; i++) {
        printf("%c ", isprint(packet[i]) ? packet[i] : '.');
        if (((i + 1) % 16 == 0 && i != 0) || i == pkthdr->len - 1) {
            printf("\n");
        }
    }

    return;
}

int main()
{
    int count = 0;
    pcap_t *descr = NULL;
    char errbuf[PCAP_ERRBUF_SIZE], *device = NULL;
    memset(errbuf, 0, PCAP_ERRBUF_SIZE);

    device = pcap_lookupdev(errbuf);
    printf("打开设备 %s\n", device);

    descr = pcap_open_live(device, MAXBYTE2CAPTURE, 1, 512, errbuf);
    pcap_loop(descr, -1, process_packet, (u_char *)&count);

    return 0;
}

