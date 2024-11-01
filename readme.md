# Line Status Display
Status indicator from process line encoder

Quadrature encoder signals are captured by a remote modbus rtu 485.  
Modbus client in a embedded pc which sends the data to a graphic output application.

## Add encoder_client simulator
- Script `LineStatusDisplay/fw/encoder_client_01/src/encoder_client.sh` 
- Simulates `/mnt/tmp/encoder_client.status` data for `encoder_gui.py`

## Framework: mount tmpfs, modbus client and Python application
Oct.24/2024  
The init process runs at user (scia) level.  

### Add user `scia` 
- Boot RPi5 as root (marco, pi)
- Add scia:
`marco@raspberry $ sudo useradd -m -c "RPi5 Display" scia`  
use a simple password (will change later)
- Add scia to the following groups: `sudo, audio, video`  
  - `sudo usermod -aG sudo scia`
  - `sudo usermod -aG audio scia`
  - `sudo usermod -aG video scia`

### Allow scia to access the main display
- Boot RPi5
- login ssh to scia, from a host terminal: `ssh scia@192.x.x.x`
- test that scia has sudo operations (like touch /mnt/foo, etc)

#### run raspi-config: `sudo raspi-config`
1. System Options
    - S5. Boot / Auto Login
      - B4. Desktop Autologin (scia has a Desktop GUI)
    - S6. Splash Screen   
      - Yes, splash at boot  (customiza later)
        - `/usr/share/plymouth/themes/pix/splash.png`

### clone LineStatusDisplay
- login scia
- clone repository at /home/scia
  - `git clone https://github.com/marcohg/LineStatusDisplay.git .`
  - project folder is:
    - `/home/scia/LineStatusDisplay`
### install systemd services
  - Change to directory `fw/boot` under project folder
    - First list installed services status using `./list.sh`
    - `sudo ./remove scia` all or `sudo ./remove especific.service`
    - The `./install` will install services for current user

### Monitor Processes initiated bt systemd
- `df -h ` verify the tmpfs mounted partion at `/mnt/tmp`
In this RAM parition, some files are created by the different processes.
- `tail -F /mnt/tmp/encoder_client.status` display the activity of the encoder
 client (ctr-c to end). If the RPi5 communicates with the encoder node, 
 `1729992270,0,71,-2,0,0` 
  - stamp date in seconds since epoch.
  - status, 
  - 0.1 secs tick, 
  - total counts, 
  - delta counts and 
  - frequency.

### Configuration
The file `configuration.json` is created at user's home, WIP



