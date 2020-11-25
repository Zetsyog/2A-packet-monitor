#ifndef COMMON_H
#define COMMON_H

#include <pcap/pcap.h>

pcap_t *handle;

void start_capture();
void compile_filter(struct bpf_program *fp);

#endif