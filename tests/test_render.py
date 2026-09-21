"""Compare exact cell text/padding and UTF-8 output against known layouts."""
import errno
import fcntl
import os
from pathlib import Path
import pty
import re
import select
import struct
import subprocess
import sys
import termios
import time


executable = str(Path(sys.argv[1]).resolve())
ansi = re.compile(r"\x1b\[[0-9;?]*[A-Za-z]|\x1b[78]")


def frame(width, rows):
    separator = "┃" + "━" * (width - 2) + "┃"
    return (
        "┏" + "━" * (width - 2) + "┓"
        + separator.join("┃" + row + "┃" for row in rows)
        + "┗" + "━" * (width - 2) + "┛"
    )


cases = {
    "accents": frame(22, [
        "Ana" + " " * 7 + " " * 5 + "Ação ",
        "João" + " " * 6 + " " * 4 + "Saúde ",
    ]),
    "truncate": frame(14, ["João… 界界… ", "e\u0301" * 4 + "… 12345 "]),
    "wide_gap": frame(12, ["界…   界… "]),
    "custom": frame(24, [" " * 8 + "João " + "Saúde" + " " * 4]),
    "all_custom": frame(20, ["João " + " " * 11 + "é "]),
    "narrow_custom": frame(9, ["Ação…  "]),
    "too_narrow": "",
    "one": frame(4, ["… "]),
    "zero": frame(3, [" "]),
    "ascii_locale": frame(4, [". "]),
    "long": frame(650, ["é" * 400 + " " * 248]),
    "center_invalid": "",
}

for name, expected in cases.items():
    result = subprocess.run([executable, name], capture_output=True, check=True, timeout=5)
    output = result.stdout.decode("utf-8", errors="strict")
    actual = ansi.sub("", output)
    assert actual == expected, f"{name}: {actual!r} != {expected!r}"
    assert not result.stderr, result.stderr.decode(errors="replace")


def run_terminal(name):
    master, slave = pty.openpty()
    fcntl.ioctl(slave, termios.TIOCSWINSZ, struct.pack("HHHH", 24, 20, 0, 0))
    process = subprocess.Popen([executable, name], stdout=slave, stderr=subprocess.PIPE)
    os.close(slave)
    data = bytearray()
    try:
        deadline = time.monotonic() + 5
        while time.monotonic() < deadline:
            ready, _, _ = select.select([master], [], [], 0.05)
            if ready:
                try:
                    block = os.read(master, 65536)
                except OSError as error:
                    if error.errno == errno.EIO:
                        break
                    raise
                if not block:
                    break
                data.extend(block)
        _, errors = process.communicate(timeout=1)
        assert process.returncode == 0, errors.decode(errors="replace")
        assert not errors, errors.decode(errors="replace")
        return data.decode("utf-8", errors="strict")
    finally:
        if process.poll() is None:
            process.kill()
            process.wait()
        os.close(master)


assert run_terminal("center") == "\x1b[9GJoão"
assert run_terminal("center_long") == "\x1b[1G1234567890123456789…"
print(f"rendering: {len(cases) + 2} scenarios OK")
