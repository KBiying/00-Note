1、手动blacklist另一个wifi
2、添加sh文件如下内容
#!/bin/bash
sudo ifconfig wlan0 down
sudo iwconfig wlan0 mode ad-hoc essid "rpad" key off
sudo ifconfig wlan0 192.168.0.4x
sudo ifconfig wlan0 up
sudo ip route add defult via 192.168.4x

3、终端执行
sudo chmod 777 adhoc_test.sh

sudo vim /etc/rc.local

bash ./home/pi/adhoc_test.sh 
