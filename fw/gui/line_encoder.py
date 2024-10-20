
# 2. Monitor EC_STATUS=/mnt/tmp/encoder_client.status
# This file is open/write/close by encoder_client
import os
from pathlib import Path
from manage_cfg import GetConfiguration, CreateEClientConfiguration
import time
import re


def GetArgs(str, argc) :
  """Get argc in str arg1,arg2,..,argc"""
  list = []
  for n in range(argc) :
    argr = re.match(r'[+-]*[0-9]+\.*[0-9]*,*',str)
    if argr :
      arg = argr.group().rstrip(',')
      str = str[argr.end():]
      list.append(arg)
  return list

cfg = GetConfiguration("configuration.json")
CreateEClientConfiguration(cfg)

# Read the encoder_client status file 
print(f"Read { cfg["status_file"] }, until 'gui_stop' found")
stop_signal = False
while not stop_signal :
  path = Path(cfg["status_file"])
  if path.is_file() :
    status = path.read_text()
    lines = status.splitlines()
    if "gui_stop" in lines :
      stop_signal = True

    # 1729395360,0,-91473,139639,0,0,
    args = GetArgs(lines.pop(),6) # from last one
    print(f"{args[2]}, { args[3] }, { args[4] },  { args[5] }")
    # else :
    #   for line in lines :
    #     print(line)
  else :
    print("waiting for encoder client")
    time.sleep(5)
  
  time.sleep(cfg["gui_sleep_sec"])

print(lines)
print( "Done!")







