#ifndef MONITOR_BOOTP_H
#define MONITOR_BOOTP_H

/**
 * Cf RFC 1542
 */

#include <sys/types.h>

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

/**
 * Bootp Options
 */
#define BOOTP_OPT_DHCP_MSG 53
#define BOOTP_OPT_END 255

char *bootp_options_name[256] = {
	[0] = "PAD",
	[1] = "Subnet Mask",
	[2] = "Time Offset",
	[3] = "Router",
	[4] = "Time Server",
	[5] = "IEN-116 Name Server",
	[6] = "DNS Name Server",
	[7] = "Log Server",
	[8] = "Cookie Server",
	[9] = "LPR Server",
	[10] = "Impress Server",
	[11] = "Resource Location Server",
	[12] = "Host Name",
	[13] = "Boot File Size",
	[14] = "Merit Dump File",
	[15] = "Domain name",
	[16] = "Swap Server",
	[17] = "Root Path",
	[18] = "Extensions Path",
	[19] = "IP Forwaring",
	[20] = "Non Local Source Routing",
	[21] = "Policy Filter",
	[22] = "Max Datagram Reassembly size",
	[23] = "Default IP TTL",
	[24] = "Path MTU Aging Timeout",
	[25] = "Path MTU Plateau Table",
	[26] = "Interface MTU",
	[27] = "All Subnets Are Local",
	[28] = "Broadcast Address",
	[29] = "Perform Mask Discovery",
	[30] = "Mask Supplier",
	[31] = "Perform Router Discovery",
	[32] = "Router Solicitation Address",
	[33] = "Static Route",
	[34] = "Trailer Encapsulation",
	[35] = "ARP Cache Timeout",
	[36] = "Ethernet Encapsulation",
	[37] = "Default TTL",
	[38] = "TCP Keepalive Interval",
	[39] = "TCP Keepalive Garbage",
	[40] = "Network Information Service Domain",
	[41] = "Network Information Servers",
	[42] = "Network Time Protocol Servers",
	[43] = "Vendor Specific Information",
	[45] = "NetBIOS Over TCP/IP Name Servers",
	[46] = "NetBIOS Over TCP/IP Datagram Distribution Servers",
	[47] = "NetBIOS Over TCP/IP Scope",
	[48] = "X Window System Font Servers:",
	[49] = "X Window System Display Manager",
	[50] = "Requested IP Address", // DHCP Options
	[51] = "IP Address Lease Time",
	[52] = "Option Overload",
	[BOOTP_OPT_DHCP_MSG] = "DHCP Message Type", // See DCHP_MESSAGE const
	[54] = "Server Identifier",
	[55] = "Parameter Request List",
	[56] = "Message",
	[57] = "Maximum DHCP Message Size",
	[58] = "Renewal (T1) Time Value",
	[59] = "Rebinding (T2) Time Value",
	[60] = "Vendor Class Identifier",
	[61] = "Client Identifie", // End DHCP Options
	[62] = "",
	[63] = "",
	[64] = "Network Information Service+ Domain",
	[65] = "Network Information Service+ Servers",
	[66] = "TFTP Server Name", // DHCP Option
	[67] = "Bootfile Name",	   // DHCP Option
	[68] = "Mobile IP Home Agent",
	[69] = "Simple Mail Transport Protocol (SMTP) Servers",
	[70] = "Post Office Protocol (POP3) Servers",
	[71] = "Network News Transfer Protocol (NNTP) Servers",
	[72] = "Default World Wide Web (WWW) Servers",
	[73] = "Default Finger Servers",
	[74] = "Default Internet Relay Chat (IRC) Servers",
	[75] = "StreetTalk Servers",
	[76] = "StreetTalk Directory Assistance (STDA) Servers",
	[BOOTP_OPT_END] = "End"};

#define DHCP_MESSAGE_DISCOVER 1
#define DHCP_MESSAGE_OFFER 2
#define DHCP_MESSAGE_REQUEST 3
#define DHCP_MESSAGE_DECLINE 4
#define DHCP_MESSAGE_ACK 5
#define DHCP_MESSAGE_NAK 6
#define DHCP_MESSAGE_RELEASE 7
#define DHCP_MESSAGE_INFORM 8

const char *dhcp_messages[9] = {
	[DHCP_MESSAGE_DISCOVER] = "Discover",
	[DHCP_MESSAGE_OFFER] = "Offer",
	[DHCP_MESSAGE_REQUEST] = "Request",
	[DHCP_MESSAGE_DECLINE] = "Decline",
	[DHCP_MESSAGE_ACK] = "Ack",
	[DHCP_MESSAGE_NAK] = "Nak",
	[DHCP_MESSAGE_RELEASE] = "Release",
	[DHCP_MESSAGE_INFORM] = "Inform"
};

struct bootp_option {
	uint8_t code;
	uint8_t len;
	uint8_t data[];
};

struct bootp_hdr {
	u_int8_t op;
	u_int8_t htype;
	u_int8_t hlen;
	u_int8_t hops;
	u_int32_t xid;
	u_int16_t secs;
	u_int16_t flags;
	u_int32_t ciaddr;
	u_int32_t yiaddr;
	u_int32_t siaddr;
	u_int32_t giaddr;
	u_int8_t chaddr[16];
	unsigned char server_name[64];
	unsigned char boot_file[128];
	unsigned char vendor[64];
};

#endif