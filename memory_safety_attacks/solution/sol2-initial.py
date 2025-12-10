import sys

malicious_count = b"\x00\x00\x00\x80"

sys.stdout.buffer.write(malicious_count)

