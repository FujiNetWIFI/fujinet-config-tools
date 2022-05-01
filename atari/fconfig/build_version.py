import subprocess
import datetime
import re
import sys

if sys.version_info[0] < 3:
  ver_build = subprocess.check_output(["git", "rev-parse", "--short", "HEAD"]).strip()
else:
  ver_build = subprocess.check_output(["git", "rev-parse", "--short", "HEAD"],encoding="ASCII").strip()

header_file = "src/version.h"

ver_date = datetime.datetime.utcnow().strftime("%Y-%m-%d %H:%M:%S")

rxs = ['^#define FCONFIG_VERSION_MAJOR (\w+)', '^#define FCONFIG_VERSION_MINOR (\w+)',
  '^(#define FCONFIG_VERSION_BUILD)', '^(#define FCONFIG_VERSION_DATE)', '^(#define FCONFIG_VERSION_FULL)']

ver_maj = ""
ver_min = ""

#txt = [line.rstrip('\n') for line in open(header_file)]
txt = [line for line in open(header_file)]

fout = open(header_file, "w")

for line in txt:

  for i in range(len(rxs)):
    m = re.match(rxs[i], line)
    if m is not None:
      break

  if m is not None:
    if i == 0:
      ver_maj = m.groups(0)[0]
      fout.write(line)
    elif i == 1:
      ver_min = m.groups(0)[0]
      fout.write(line)
    elif i == 2:
      line = m.groups(0)[0] + " \"" + ver_build + "\"\n"
      fout.write(line)
    elif i == 3:
      line = m.groups(0)[0] + " \"" + ver_date + "\"\n"
      fout.write(line)
    elif i == 4:
      line = m.groups(0)[0] + " \"" + ver_maj + "." + ver_min + "." + ver_build + "\"\n"
      fout.write(line)
  else:
    fout.write(line)

fout.close()
