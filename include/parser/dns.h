#ifndef DNS_H
#define DNS_H

#include <netinet/in.h>

struct dns_hdr {
	uint16_t id;
	uint16_t flags;
	uint16_t total_questions;
	uint16_t total_answer_rr;
	uint16_t total_authority_rr;
	uint16_t total_additional_rr;
};

#define QR_QUERY 0
#define QR_RESPONSE 1

#define OPCODE_STD_QUERY 0
#define OPCODE_IQUERY 1 // inverse query
#define OPCODE_STATUS 2 // server status request
#define OPCODE_NOTIFY 4
#define OPCODE_UPDATE 5

#define AA_NOT_AUTHORITATIVE 0
#define AA_AUTHORITATIVE 1

#define TC_NOT_TRUNCATED 0
#define TC_TRUNCATED 1

#define RD_RECURSION_NOT_DESIRED 0
#define RD_RECURSION_DESIRED 1

#define RA_RECURSION_NOT_AVAILABLE 0
#define RA_RECURSION_AVAILABLE 1

// DNS types
#define DNS_TYPE_A 1
#define DNS_TYPE_NS 2
#define DNS_TYPE_CNAME 5
#define DNS_TYPE_SOA 6
#define DNS_TYPE_PTR 12
#define DNS_TYPE_AAAA 28

#define DNS_QR_MASK 0x8000
#define DNS_OPCODE_MASK 0x7800
#define DNS_AA_MASK 0x0400
#define DNS_TC_MASK 0x0200
#define DNS_RD_MASK 0x0100
#define DNS_RA_MASK 0x0080
// Reserved bit
#define DNS_AD_MASK 0x0020
#define DNS_CD_MASK 0x0010
#define DNS_RCODE_MASK 0x000f

char *dns_opcode_name[OPCODE_UPDATE + 1] = {
    [OPCODE_STD_QUERY] = "Standard query",
    [OPCODE_IQUERY] = "Inverse query",
    [OPCODE_STATUS] = "Server status request",
    [OPCODE_NOTIFY] = "Notify",
    [OPCODE_UPDATE] = "Update",
};

char *dns_rcode_name[23] = {
    [0] = "No error",
    [1] = "Format error",
    [2] = "Server failure",
    [3] = "Name error",
    [4] = "Not implemented",
    [5] = "Refused",
    [6] = "YXDomain",
    [7] = "YXRRSet",
    [8] = "NXRRSet",
    [9] = "Not auth",
    [10] = "Not zone",
    [11] = "",
    [12] = "",
    [13] = "",
    [14] = "",
    [15] = "",
    [16] = "Bad OPT version",
    [17] = "Bad key",
    [18] = "Bad time",
    [19] = "Bad mode",
    [20] = "Bad name",
    [21] = "Bad alg",
    [22] = "Bad trunc"
};

#define DNS_OPCODE(flag) dns_opcode_name[(flag) & DNS_OPCODE_MASK]
#define DNS_AA(flag) (((flag) & DNS_AA_MASK) == AA_AUTHORITATIVE)
#define DNS_TC(flag) (((flag) & DNS_TC_MASK) == TC_TRUNCATED )
#define DNS_RD(flag) (((flag) & DNS_RD_MASK) == RD_RECURSION_DESIRED)
#define DNS_RA(flag) (((flag) & DNS_RA_MASK) == RA_RECURSION_AVAILABLE)
#define DNS_AD(flag) ((flag) & DNS_AD_MASK)
#define DNS_CD(flag) ((flag) & DNS_CD_MASK)
#define DNS_CD(flag) ((flag) & DNS_CD_MASK)
#define DNS_RCODE(flag) dns_rcode_name[(flag) & DNS_RCODE_MASK]

#define IS_POINTER(label) ((uint16_t)(label) & 0xC000)
#define GET_OFFSET(label) ((uint16_t)(label) & 0x03FF)

#endif