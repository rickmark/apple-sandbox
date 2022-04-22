#!/usr/bin/env python

import re
import subprocess
import sys

symbol_re = re.compile('^([0-9a-f]+) (.) (.*)$')
symbols = []
symbols_by_name = {}

# With the advent of DSC you will need to specify this file explicitly

if len(sys.argv) == 2:
  libsandbox = sys.argv[1]
else:
  libsandbox = "/usr/lib/libsandbox.dylib"

p = subprocess.Popen(["nm", libsandbox],
                     stdout=subprocess.PIPE)
while True:
  line = p.stdout.readline().decode('utf8')
  if not line:
    break

  mo = symbol_re.match(line)
  if mo is not None:
    symbols.append(mo.groups())
    symbols_by_name[mo.group(3)] = mo.groups()

#print symbols
#print symbols_by_name['_scheme_init']

print("""struct symbol {
  const char *name;
  unsigned int offset;
} symbols[] = {""")

for addr, typ, name in symbols:
  if typ in ['T', 't', 's']:
    print('  {{ \"{}\", 0x{} }},'.format(name, addr))

print("""};

unsigned int get_offset(const char *name) {
  unsigned int curr;
  unsigned int count = sizeof(symbols) / sizeof(struct symbol);

  for(curr = 0; curr < count; ++curr) {
    if (strcmp(name, symbols[curr].name) == 0) {
      return symbols[curr].offset;
    }
  }

  return 0;
}
""")


