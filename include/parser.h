#ifndef PARSER_H
#define PARSER_H

#include <stdint.h>

void parse_ethernet(const unsigned char *packet);
void parse_ip(const unsigned char *packet);
void parse_ipv6(const unsigned char *packet);
void parse_tcp(const unsigned char *packet, uint16_t size);
void parse_udp(const unsigned char *packet);
void parse_bootp(const unsigned char *packet);
void parse_dhcp(const unsigned char *packet);
void parse_dns(const unsigned char *packet);
void parse_smtp(const unsigned char *packet, uint16_t size);
void parse_telnet(const unsigned char *packet, uint16_t size);
void parse_http(const unsigned char *packet, uint16_t size);
void parse_ftp(const unsigned char *packet, uint16_t size);

#endif