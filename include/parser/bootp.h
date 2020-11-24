#ifndef MONITOR_BOOTP_H
#define MONITOR_BOOTP_H

/**
 * Cf RFC 1542
 */

#include <sys/types.h>

#define BOOTP_SERVER_PORT 67
#define BOOTP_CLIENT_PORT 68

/** 
 * Op codes
 */
#define BOOTREQUEST 1
#define BOOTREPLY 2

/**
 * Hardware types
 */
#define ETHERNET 1
#define IEEE_802 6

/**
 * Flags
 */
#define FLAG_BROADCAST 1

struct bootp_hdr {
	u_int8_t	op;
	u_int8_t	htype;
	u_int8_t	hlen;
	u_int8_t	hops;
	u_int32_t	xid;
	u_int16_t	secs;
	u_int16_t	flags;
	u_int32_t	ciaddr;
	u_int32_t	yiaddr;
	u_int32_t	siaddr;
	u_int32_t	giaddr;
	u_int8_t	chaddr[16];
	char	server_name[64];
	char	boot_file[128];
	/* 312 bytes of extensions */
};

#endif