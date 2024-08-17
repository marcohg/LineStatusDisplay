# ncurses in ubuntu 24.04
Package has been updated to ncurses6, mcuxpresso still not official for 24.04

https://www.reddit.com/r/Ubuntu/comments/1cm97bg/libncurses5dev/

wget http://archive.ubuntu.com/ubuntu/pool/universe/n/ncurses/libtinfo5_6.4-2_amd64.deb && sudo dpkg -i libtinfo5_6.4-2_amd64.deb && rm -f libtinfo5_6.4-2_amd64.deb
wget http://archive.ubuntu.com/ubuntu/pool/universe/n/ncurses/libncurses5_6.4-2_amd64.deb && sudo dpkg -i libncurses5_6.4-2_amd64.deb && rm -f libncurses5_6.4-2_amd64.deb
sudo apt install lib32ncurses5-dev libncurses5 libncurses5-dev -y

wget http://archive.ubuntu.com/ubuntu/pool/universe/n/ncurses/libtinfo5_6.4-2_amd64.deb && sudo dpkg -i libtinfo5_6.4-2_amd64.deb && rm -f libtinfo5_6.4-2_amd64.deb
wget http://archive.ubuntu.com/ubuntu/pool/universe/n/ncurses/libncursesw5_6.4-2_amd64.deb && sudo dpkg -i libncursesw5_6.4-2_amd64.deb && rm -f libncursesw5_6.4-2_amd64.deb
sudo apt install lib32ncursesw5-dev libncursesw5 libncursesw5-dev -y

universe for packages:  
http://archive.ubuntu.com/ubuntu/pool/universe/n/ncurses

