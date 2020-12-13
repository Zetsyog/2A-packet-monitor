# 2A-packet-monitor

## Usage

Use `make` to compile the program.
The generated executable is the `bin/monitor` file

Usage :

```
bin/monitor (-i <interface> | -o <file>) [-f filter] [-v <1..3>]
```

When run with -i option, the program will live capture packets on the given interface
Be careful, it needs super user rights (run with sudo)

When run with -o option, the program will read the given file and parse the packets.
It must be a valid capture file
Some samples are given in the test folder.

The -f option allow to apply a filter on the capture.
The argument is a valid pcap filter as one string.
Follow this link to get more info on pcap filters [https://www.tcpdump.org/manpages/pcap-filter.7.html]

The last option (-v) sets the verbosity level.

-   Verbosity 1 : One concise line per packet
-   Verbosity 2 : One line per packet protocol. Display very synthetic information
-   Verbosity 3 : Display a maximum of information for each packet. Flood the console.

## Supported protocols

Layer 2 (Data link layer)
* Ethernet

Layer 3 (Network layer)
* IPv4
* IPv6
* ARP / RARP

Layer 4 (Transport layer)
* TCP
* UDP

Layer 5 (Application layer)
* Bootp / DHCP
* DNS
* FTP
* HTTP
* IMAP
* POP
* SMTP
* TELNET

Parsing of certain protocols can be partial.
## Credit

-   bootp.h : https://samy.pl/packet/MISC/tcpdump-3.7.1/bootp.h

All packet captured used for tests are from

-   https://wiki.wireshark.org/SampleCaptures
-   https://packetlife.net/captures/
