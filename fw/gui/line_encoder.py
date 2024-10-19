
# 2. Monitor EC_STATUS=/mnt/tmp/encoder_client.status
# This file is open/write/close by encoder_client
import os
from pathlib import Path
from manage_cfg import GetConfiguration, CreateEClientConfiguration
import time

cfg = GetConfiguration("configuration.json")
CreateEClientConfiguration(cfg)

# Read the encoder_client status file 
print(f"Read { cfg["status_file"] }, until 'gui_stop' found")
stop_signal = False
stop_counter = 0
while not stop_signal :
  path = Path(cfg["status_file"])
  if path.is_file() :
    status = path.read_text()
    lines = status.splitlines()
    if "gui_stop" in lines :
      stop_signal = True
    else :
      for line in lines :
        print(line)
  else :
    print("waiting for encoder client")
    time.sleep(5)
  
  time.sleep(cfg["gui_sleep_sec"])
  stop_counter +=1
  if stop_counter > 20 :
    stop_signal = True
  else :
    print( f"sleeping {stop_counter}")

print(lines)
print( "Done!")







