import base64

def format_mac(val):
    return ':'.join(f"{v:02x}" for v in val)

def print_hex(val):
    return ' '.join(f"{v:02x}" for v in val)

def format_ip(val):
    # Converte i byte dell'IP in stringhe e li unisce con il punto alneo si vede in maniera decente
    return '.'.join(str(v) for v in val)

data = "xK00hOd1kBsOZaUlCABFAAH/xzRAAIAGAAAKAGTd1ZBHos09AFC8rDKk67DDQVAYIBSOAQAAR0VUIC8gSFRUUC8xLjENCkhvc3Q6IHd3dy52YWxsYXVyaS5lZHUNCkNvbm5lY3Rpb246IGtlZXAtYWxpdmUNClVwZ3JhZGUtSW5zZWN1cmUtUmVxdWVzdHM6IDENClVzZXItQWdlbnQ6IE1vemlsbGEvNS4wIChXaW5kb3dzIE5UIDEwLjA7IFdpbjY0OyB4NjQpIEFwcGxlV2ViS2l0LzUzNy4zNiAoS0hUTUwsIGxpa2UgR2Vja28pIENocm9tZS8xNDguMC4wLjAgU2FmYXJpLzUzNy4zNg0KQWNjZXB0OiB0ZXh0L2h0bWwsYXBwbGljYXRpb24veGh0bWwreG1sLGFwcGxpY2F0aW9uL3htbDtxPTAuOSxpbWFnZS9hdmlmLGltYWdlL3dlYnAsaW1hZ2UvYXBuZywqLyo7cT0wLjgsYXBwbGljYXRpb24vc2lnbmVkLWV4Y2hhbmdlO3Y9YjM7cT0wLjcNCkFjY2VwdC1FbmNvZGluZzogZ3ppcCwgZGVmbGF0ZQ0KQWNjZXB0LUxhbmd1YWdlOiBpdC1JVCxpdDtxPTAuOSxlbi1VUztxPTAuOCxlbjtxPTAuNw0KQ29va2llOiB2YWxsYXVyaT0NCg0K"
data = base64.b64decode(data)

destination_mac = data[0:6]
source_mac = data[6:12]
ethertype = data[12:14]

# IP Version: Il byte 14 contiene sia la versione (primi 4 bit) che la lunghezza dell'header. shifto a destra (>> 4) per isolare i primi 4 bit.
ip_version = data[14] >> 4 

# TTL (Time To Live): È l'8° byte dell'header IP, quindi si trova al byte 22 assoluto (14 + 8)
ttl = data[22]

# Encapsulated Protocol: È il 9° byte dell'header IP, quindi byte 23 assoluto (14 + 9)
protocol = data[23]

# Source IP: È lungo 4 byte e inizia al byte 26 assoluto (14 + 12)
source_ip = data[26:30]

# Destination IP: È lungo 4 byte e inizia al byte 30 assoluto (14 + 16)
destination_ip = data[30:34]

print("Source MAC address:      ", format_mac(source_mac))
print("Destination MAC address: ", format_mac(destination_mac))
print("Type/Length:             ", print_hex(ethertype))
print("IP Version:              ", ip_version)
print("TTL:                     ", ttl)

if protocol == 6:
    proto_name = "TCP (6)"
else:
    proto_name = str(protocol)

print("Encapsulated Protocol:   ", proto_name)
print("Source IP address:       ", format_ip(source_ip))
print("Destination IP address:  ", format_ip(destination_ip))