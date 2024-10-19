# Manage Line Display Global Settings
from pathlib import Path
import json

defaults = {
  # Encoder Client
  "port" : "/dev/ttyUSB0",
  "baud" : 115200, # 57200, 38400, 19200],
  "poll_rate_ms" : 250,
  "timeout_ms"  : 200,
  "nodes" : [ # Upto 4 nodes
    {
      "address" : 2, "time_base_ms" : 1000
    }
  ],
  "config_file": "/mnt/tmp/encoder_client.cfg",
  "status_file": "/mnt/tmp/encoder_client.status",
  
  # GUI Settings
  "gui_sleep_sec" : 2.5
}

def GetConfiguration( file_name) :
  """Get the configuration from file_name or from defaults"""
  path = Path(file_name)
  if path.is_file() :
    content = path.read_text()
    configuration = json.loads(content)
  else :
    configuration = defaults
    content = json.dumps(configuration, indent=2)
    path.write_text(content) 
  return configuration


def CreateEClientConfiguration ( c ) :
  """Create Encoder Client Config (Starts Modbus)"""
  print (f"Create { c["config_file"] } file" )
  str =  f"port {c["port"]}\n"
  str += f"baud {c["baud"]}\n"
  str += f"poll_rate_ms {c["poll_rate_ms"]}\n"
  str += f"timeout_ms {c["timeout_ms"]}\n"
  str += "-- Node Address Time_base_ms\n"
  n = 1
  for node in c["nodes"] :
    str += f"Node	{n} {node["address"]} {node["time_base_ms"]} \n"
    n += 1
  str += "--end--\n"
  path = Path(c["config_file"])
  path.write_text(str)




# configuration = get_configuration("configuration.json")
# print(json.dumps(configuration, indent=2))

# # with open("data.json", mode="w", encoding="utf-8") as write_file :
# #   json.dump(defaults, write_file, indent=2)

# # line_encoder_edited = {}
# v = input("Enter when edited data.py complete")
# with open("configuration.json", mode="r", encoding="utf-8") as read_file :
#   line_encoder_edited = json.load(read_file) #, indent="\t")

# # print( json.dumps(line_encoder_edited,indent=2) )
# for node in line_encoder_edited["nodes"] :
#   print(node)


