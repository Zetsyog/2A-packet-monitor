#include "logger.h"
#include "parser/dns.h"
#include <arpa/inet.h>
#include <stdlib.h>

static void read_name(uint8_t *hdr, uint8_t **base, int *label_nb,
					  int *label_len, int pointer) {
	uint8_t *cursor = *base;
	uint16_t type;

	while (1) {
		uint8_t len = *cursor;
		// log_formatln("\ndebug : len = %02x", len);
		// log_offset();
		if (len == 0) {
			if (!pointer)
				*base += 2;
			break;
		}

		type = ntohs(*((uint16_t *)cursor));

		if (IS_POINTER(type)) {
			// log_formatln("ispointer %04x (%04x)", type, GET_OFFSET(type));
			cursor = hdr + GET_OFFSET(type);
			read_name(hdr, &cursor, label_nb, label_len, 1);
			if (!pointer)
				*base += 2;

			break;
		}

		if (label_nb != NULL)
			(*label_nb)++;

		cursor += 1;
		if (!pointer)
			*base += 1;

		for (int i = 0; i < len; i++) {
			if (label_len != NULL)
				*label_len += 1;
			log_format("%c", (unsigned char)*cursor);
			cursor++;
			if (!pointer)
				(*base)++;
		}

		if (*cursor != 0) {
			log_format(".");
		}
	}
	if (!pointer)
		log_format("\n");
}

static void parse_rr(uint8_t *hdr, uint8_t **payload) {
	int name_len = 0, label_nb = 0;

	log_offset();
	log_format("%-15s", "Name");
	read_name((uint8_t *)hdr, payload, &label_nb, &name_len, 0);

	log_formatln("%-15s%u", "[Name len]", name_len);
	log_formatln("%-15s%u", "[Label nb]", label_nb);
	log_formatln("%-15s%u", "Type", ntohs(*((uint16_t *)*payload)));
	*payload += 2;
	log_formatln("%-15s%u", "Class", ntohs(*((uint16_t *)*payload)));
	*payload += 2;
	log_formatln("%-15s%lu", "TTL", ntohl(*((uint32_t *)*payload)));
	*payload += 4;
	log_formatln("%-15s%u", "Data len", ntohs(*((uint16_t *)*payload)));
	*payload += 2;

	log_offset();
	log_format("%-15s", "Data");
	read_name((uint8_t *)hdr, payload, NULL, NULL, 0);

	log_format("\n");
}

void parse_dns(const unsigned char *packet) {
	struct dns_hdr *hdr = (struct dns_hdr *)packet;
	uint16_t flags = ntohs(hdr->flags);
	uint16_t questions = ntohs(hdr->total_questions);
	uint16_t answer_rr = ntohs(hdr->total_answer_rr);
	uint16_t authority_rr = ntohs(hdr->total_authority_rr);
	uint16_t additional_rr = ntohs(hdr->total_additional_rr);

	set_offset(3);
	BEGIN_LOG(COMPLETE);
	log_title("DNS");
	log_formatln("%-15s0x%04x", "Trans. ID", ntohs(hdr->id));

	log_formatln("%-15s", "Flags");
	set_offset(4);
	// TODO print flags
	log_formatln("%s%s", "Response: Message is a ",
				 (flags & DNS_QR_MASK) == QR_QUERY ? "query" : "response");
	log_formatln("%s: %s", "Opcode", DNS_OPCODE(flags));
	log_formatln("%s: %s %s", "Authoritative",
				 "Server is an authority for domain?",
				 DNS_AA(flags) ? "yes" : "no");
	log_formatln("%s: %s %s", "Truncated", "Message is truncated?",
				 DNS_TC(flags) ? "yes" : "no");
	log_formatln("%s: %s", "Recursion desired", DNS_RD(flags) ? "yes" : "no");
	log_formatln("%s: %s", "Recursion available", DNS_RD(flags) ? "yes" : "no");
	log_formatln("%s: %s %s", "Auth", "Answer was authenticated by server?",
				 DNS_AD(flags) ? "yes" : "no");
	log_formatln("%s: %s", "Non-authenticated data",
				 DNS_CD(flags) ? "acceptable" : "unacceptable");
	log_formatln("%s: %s", "Reply code", DNS_RCODE(flags));
	set_offset(3);

	log_formatln("%-15s%u", "Questions", questions);
	log_formatln("%-15s%u", "Answer RRs", answer_rr);
	log_formatln("%-15s%u", "Authority RRs", authority_rr);
	log_formatln("%-15s%u", "Additional RRs", additional_rr);

	uint8_t *payload = (uint8_t *)packet + sizeof(struct dns_hdr);

	// Questions
	if (questions > 0) {
		set_offset(3);
		log_formatln("Questions");
		set_offset(4);

		for (int i = 0; i < questions; i++) {
			int name_len = 0, label_nb = 0;
			log_formatln("Question %i", i);
			set_offset(5);
			log_offset();
			log_format("%-15s", "Name");
			read_name((uint8_t *)hdr, &payload, &label_nb, &name_len, 0);

			log_formatln("%-15s%u", "[Name len]", name_len);
			log_formatln("%-15s%u", "[Label nb]", label_nb);
			log_formatln("%-15s%u", "Type", *((uint16_t *)payload));
			payload += 2;
			log_formatln("%-15s%u", "Class", *((uint8_t *)payload));
			payload += 1;
		}
	}

	// Answers
	if (answer_rr > 0) {
		set_offset(3);
		log_formatln("Answers RR");
		set_offset(4);

		for (int i = 0; i < answer_rr; i++) {
			set_offset(4);
			log_formatln("- Answer %i", i);
			set_offset(5);
			parse_rr((uint8_t *) hdr, &payload);
		}
	}

	// Authority
	if (authority_rr > 0) {
		set_offset(3);
		log_formatln("Authority RR");
		set_offset(4);

		for (int i = 0; i < answer_rr; i++) {
			set_offset(4);
			log_formatln("- Authority RR %i", i);
			set_offset(5);
			parse_rr((uint8_t *) hdr, &payload);
		}
	}

	// Additional
	if (additional_rr > 0) {
		set_offset(3);
		log_formatln("Additional RR");
		set_offset(4);

		for (int i = 0; i < answer_rr; i++) {
			set_offset(4);
			log_formatln("- Additional RR %i", i);
			set_offset(5);
			parse_rr((uint8_t *) hdr, &payload);
		}
	}

	END_LOG();
}