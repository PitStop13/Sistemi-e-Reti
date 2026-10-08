import base64

def format_mac(val):
    return ':'.join(f"{v:02x}" for v in val)


def print_hex(val):
    return ' '.join(f"{v:02x}" for v in val)


def format_ip(val):
    return '.'.join(str(v) for v in val)


raw_b64 = (
    "xK00hOd1kBsOZaUlCABFAAH/xzRAAIAGAAAKAGTd1ZBHos09AFC8rDKk67DDQVAYIBSOAQAAR0VUIC8gSFRUUC8xLjENCkhvc3Q6IHd3dy52YWxsYXVyaS5lZHUNCkNvbm5lY3Rpb246IGtlZXAtYWxpdmUNClVwZ3JhZGUtSW5zZWN1cmUtUmVxdWVzdHM6IDENClVzZXItQWdlbnQ6IE1vemlsbGEvNS4wIChXaW5kb3dzIE5UIDEwLjA7IFdpbjY0OyB4NjQpIEFwcGxlV2ViS2l0LzUzNy4zNiAoS0hUTUwsIGxpa2UgR2Vja28pIENocm9tZS8xNDguMC4wLjAgU2FmYXJpLzUzNy4zNg0KQWNjZXB0OiB0ZXh0L2h0bWwsYXBwbGljYXRpb24veGh0bWwreG1sLGFwcGxpY2F0aW9uL3htbDtxPTAuOSxpbWFnZS9hdmlmLGltYWdlL3dlYnAsaW1hZ2UvYXBuZywqLyo7cT0wLjgsYXBwbGljYXRpb24vc2lnbmVkLWV4Y2hhbmdlO3Y9YjM7cT0wLjcNCkFjY2VwdC1FbmNvZGluZzogZ3ppcCwgZGVmbGF0ZQ0KQWNjZXB0LUxhbmd1YWdlOiBpdC1JVCxpdDtxPTAuOSxlbi1VUztxPTAuOCxlbjtxPTAuNw0KQ29va2llOiB2YWxsYXVyaT0NCg0K"
)
pkt = base64.b64decode(raw_b64)

# --- Ethernet header ---
if len(pkt) < 14:
    raise SystemExit("Frame troppo corto per header Ethernet")

dst_mac = pkt[0:6]
src_mac = pkt[6:12]
eth_type = int.from_bytes(pkt[12:14], 'big')

print("Source MAC address:      ", format_mac(src_mac))
print("Destination MAC address: ", format_mac(dst_mac))
print("Type/Length:             ", f"0x{eth_type:04x} ({eth_type})")

if eth_type == 0x0800:  # IPv4
    ip_start = 14
    if len(pkt) < ip_start + 20:
        raise SystemExit("Frame troppo corto per header IP base")

    ver_ihl = pkt[ip_start]
    ip_version = ver_ihl >> 4
    ihl = ver_ihl & 0x0F
    ip_header_length = ihl * 4

    dscp = pkt[ip_start + 1] >> 2
    ecn = pkt[ip_start + 1] & 0x03

    total_length = int.from_bytes(pkt[ip_start + 2:ip_start + 4], 'big')
    identification = int.from_bytes(pkt[ip_start + 4:ip_start + 6], 'big')
    flags_frag = int.from_bytes(pkt[ip_start + 6:ip_start + 8], 'big')
    flags = (flags_frag >> 13) & 0x7
    frag_offset = flags_frag & 0x1FFF

    ttl = pkt[ip_start + 8]
    protocol = pkt[ip_start + 9]
    header_checksum = int.from_bytes(pkt[ip_start + 10:ip_start + 12], 'big')

    src_ip = pkt[ip_start + 12:ip_start + 16]
    dst_ip = pkt[ip_start + 16:ip_start + 20]

    print("IP Version:              ", ip_version)
    print("IHL (header length):     ", ip_header_length, "bytes")
    print("DSCP:                    ", dscp)
    print("ECN:                     ", ecn)
    print("Total Length:            ", total_length)
    print("Identification:          ", identification)
    print("Flags:                   ", bin(flags))
    print("Fragment Offset:         ", frag_offset)
    print("TTL:                     ", ttl)
    proto_name = {6: "TCP (6)", 17: "UDP (17)", 1: "ICMP (1)"}.get(protocol, str(protocol))
    print("Encapsulated Protocol:   ", proto_name)
    print("Header Checksum:         ", f"0x{header_checksum:04x}")
    print("Source IP address:       ", format_ip(src_ip))
    print("Destination IP address:  ", format_ip(dst_ip))

    # --- Transport layer ---
    transport_start = ip_start + ip_header_length
    ip_payload_end = ip_start + total_length
    if ip_payload_end > len(pkt):
        ip_payload_end = len(pkt)

    if protocol == 6 and len(pkt) >= transport_start + 20:  # TCP
        tcp_src_port = int.from_bytes(pkt[transport_start:transport_start + 2], 'big')
        tcp_dst_port = int.from_bytes(pkt[transport_start + 2:transport_start + 4], 'big')
        tcp_seq = int.from_bytes(pkt[transport_start + 4:transport_start + 8], 'big')
        tcp_ack = int.from_bytes(pkt[transport_start + 8:transport_start + 12], 'big')
        tcp_offset = (pkt[transport_start + 12] >> 4) * 4
        tcp_flags_byte = pkt[transport_start + 13]
        flags_dict = {
            'CWR': bool(tcp_flags_byte & 0x80),
            'ECE': bool(tcp_flags_byte & 0x40),
            'URG': bool(tcp_flags_byte & 0x20),
            'ACK': bool(tcp_flags_byte & 0x10),
            'PSH': bool(tcp_flags_byte & 0x08),
            'RST': bool(tcp_flags_byte & 0x04),
            'SYN': bool(tcp_flags_byte & 0x02),
            'FIN': bool(tcp_flags_byte & 0x01),
        }
        tcp_window = int.from_bytes(pkt[transport_start + 14:transport_start + 16], 'big')
        tcp_checksum = int.from_bytes(pkt[transport_start + 16:transport_start + 18], 'big')
        tcp_urg_ptr = int.from_bytes(pkt[transport_start + 18:transport_start + 20], 'big')
        print("TCP Source Port:         ", tcp_src_port)
        print("TCP Destination Port:    ", tcp_dst_port)
        print("TCP Seq Number:          ", tcp_seq)
        print("TCP Ack Number:          ", tcp_ack)
        print("TCP Header Length:       ", tcp_offset, "bytes")
        print("TCP Flags:               ", ','.join(k for k, v in flags_dict.items() if v) or "None")
        print("TCP Window Size:         ", tcp_window)
        print("TCP Checksum:            ", f"0x{tcp_checksum:04x}")
        print("TCP Urgent Pointer:      ", tcp_urg_ptr)
        payload = pkt[transport_start + tcp_offset: ip_payload_end]
        print("TCP Payload (hex):       ", print_hex(payload))

    elif protocol == 17 and len(pkt) >= transport_start + 8:  # UDP
        udp_src = int.from_bytes(pkt[transport_start:transport_start + 2], 'big')
        udp_dst = int.from_bytes(pkt[transport_start + 2:transport_start + 4], 'big')
        udp_len = int.from_bytes(pkt[transport_start + 4:transport_start + 6], 'big')
        udp_checksum = int.from_bytes(pkt[transport_start + 6:transport_start + 8], 'big')
        print("UDP Source Port:         ", udp_src)
        print("UDP Destination Port:    ", udp_dst)
        print("UDP Length:              ", udp_len)
        print("UDP Checksum:            ", f"0x{udp_checksum:04x}")
        payload = pkt[transport_start + 8: transport_start + udp_len] if transport_start + udp_len <= len(pkt) else pkt[transport_start + 8:ip_payload_end]
        print("UDP Payload (hex):       ", print_hex(payload))

    elif protocol == 1 and len(pkt) >= transport_start + 4:  # ICMP
        icmp_type = pkt[transport_start]
        icmp_code = pkt[transport_start + 1]
        icmp_checksum = int.from_bytes(pkt[transport_start + 2:transport_start + 4], 'big')
        print("ICMP Type:               ", icmp_type)
        print("ICMP Code:               ", icmp_code)
        print("ICMP Checksum:           ", f"0x{icmp_checksum:04x}")
        print("ICMP Data (hex):         ", print_hex(pkt[transport_start + 4:ip_payload_end]))

    else:
        print("Transport payload (hex): ", print_hex(pkt[transport_start:ip_payload_end]))

else:
    print("Non-IPv4 Ethertype, payload hex:")
    print(print_hex(pkt[14:]))
    