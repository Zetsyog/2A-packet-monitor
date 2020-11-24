#ifndef PARSER_H
#define PARSER_H

#include <stdint.h>


void parse_ethernet(const unsigned char *packet);
void parse_ip(const unsigned char *packet);
void parse_tcp(const unsigned char *packet);
void parse_udp(const unsigned char *packet);
void parse_http(const unsigned char *packet);
void parse_dhcp(const unsigned char *packet);
void parse_bootp(const unsigned char *packet);

#endif