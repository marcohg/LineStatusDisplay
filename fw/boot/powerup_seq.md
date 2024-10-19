# Powerup Sequence

The startup process is controlled using systemd units which StartExec the scripts.

The `PROJECT_LOC=/home/user/Documents/LineStatusDisplay` is the project folder location from git repository.

## Startup scripts
- Script files are located in `$PROJECT_LOC/fw/boot`
- `install.sh` moves them to location `/usr/local/sbin` and chmod them to 777 
- Three scripts
  - mount_tmpfs.sh -mount temporary storage (root level, chmod), one-shot
  - encoder_client.sh starts modbus comunication
  - encoder_gui.sh coordinates encoder client, display

## Process coordination
Mounting the tmpfs partition is a one-shot process, it is ran as root and the folder permission is changed to allow user-level to r/w and execute scripts.

The two remaining processes encoder_client and encoder_gui, are ran at user level (either marco, pi).

## Encoder Client
The script starts the application which is an executable from c/c++. 
The sources are compiled for the target ARM aarch64 and located placed at `$PROJECT/boot/encoder_client`. 
A `build_rp5.sh` is provided to compile and place the elf.

This client process is non-ending and does the following: 
1. Wait for the configuration `/mnt/tmp/encoder_client.cfg` to be created
2. Communicate with described nodes and place current data at `/mnt/tmp/encoder_client.status`

To execute the script, the service is enabled and explained in systemd section bellow.

To stop the process, we can disable the service using systemctl:  
`systemctl disable encoder_client.service`

## Encoder GUI
The GUI is provided using a python script, these are the functions
1. Create `/mnt/tmp/encoder_client.cfg` to start communication
2. Read `/mnt/tmp/encoder_client.status`
3. Format and display data in monitor.

## SystemD 
There are three services started during init:
- mount_tmpfs.service
- encoder_client.service
- encoder_gui.service

sudo ./remove_service [service] 
 
