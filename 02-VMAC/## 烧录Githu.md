## 烧录Github仓库自带镜像后的操作

1. 修改 `boot/config.txt` ，在 `62` 行添加 `dtoverlay=dwc2,dr_mode=host`；
2. 修改 `/etc/apt/sources.list`，将内容替换成下面内容：

   ```
   deb http://mirrors.tuna.tsinghua.edu.cn/raspbian/raspbian/ buster main contrib non-free rpi
   deb-src http://mirrors.tuna.tsinghua.edu.cn/raspbian/raspbian/ buster main contrib non-free rpi
   
   或者
   
   deb https://mirrors.sustech.edu.cn/raspbian/raspbian/ buster main contrib non-free rpi
   deb-src https://mirrors.sustech.edu.cn/raspbian/raspbian/ buster main contrib non-free rpi
   
   ```
3. 修改 `/etc/apt/sources.list.d/raspi.list`，将内容替换成下面内容：

   ```
   deb http://mirrors.tuna.tsinghua.edu.cn/raspberrypi/ buster main ui
   deb-src http://mirrors.tuna.tsinghua.edu.cn/raspberrypi/ buster main ui
   
   或者
   
   deb https://mirrors.sustech.edu.cn/raspberrypi buster main ui
   deb-src https://mirrors.sustech.edu.cn/raspberrypi/ buster main ui
   
   ```
4. 将 '树莓派init' 文件夹的内容复制进 `/home/pi`；
5. 按顺序执行以下 `SHELL` 命令：

   ```shell
   sudo apt-get update
   sudo apt install vim dkms lshw
   sudo apt install git bc bison flex libssl-dev libncurses5-dev
   
   ./rpi-source --dest=/home/pi
   
   # lshw 查看自带网卡的名称，记录下来并添加黑名单，如下面的网卡名称是 brcmfmac
   lshw
   sudo vim /etc/modprobe.d/raspi-blacklist.conf
   >> blacklist brcmfmac
   
   cp /lib/firmware/ath9k_htc/htc_9271-1.4.0.fw /home/pi/
   sudo mv -f /home/pi/already-built-package/firmware/htc_9271-1.4.0.fw /lib/firmware/ath9k_htc
   reboot
   
   cd /home/pi/already-built-package/kernel/pi4 && chmod 775 install.sh
   ```

> `sudo wget https://raw.yzuu.cf/RPi-Distro/rpi-source/master/rpi-source -O /usr/local/bin/rpi-source && sudo chmod +x /usr/local/bin/rpi-source && /usr/local/bin/rpi-source -q --tag-update`
>
> `sudo mv -f /home/pi/rpi-source /usr/local/bin/`
