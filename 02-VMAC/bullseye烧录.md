# debug阶段

## 1 、烧录树莓派及准备工作

#### // 改boot/config.txt：`dtoverlay=dwc2,dr_mode=host`

#### 新建ssh空白文件、

#### 换源、

**换成国内源(南科大或清华源，优先南科大)：**

**把/etc/apt/sources.list所有内容替换为：**

```c
# 默认注释了源码镜像以提高 apt update 速度，如有需要可自行取消注释
deb https://mirrors.sustech.edu.cn/debian/ bullseye main contrib non-free
# deb-src https://mirrors.sustech.edu.cn/debian/ bullseye main contrib non-free
deb https://mirrors.sustech.edu.cn/debian/ bullseye-updates main contrib non-free
# deb-src https://mirrors.sustech.edu.cn/debian/ bullseye-updates main contrib non-free
deb https://mirrors.sustech.edu.cn/debian/ bullseye-backports main contrib non-free
# deb-src https://mirrors.sustech.edu.cn/debian/ bullseye-backports main contrib non-free
deb https://mirrors.sustech.edu.cn/debian-security bullseye-security main contrib non-free
# deb-src https://mirrors.sustech.edu.cn/debian-security bullseye-security main contrib non-free

```

```
# 默认注释了源码镜像以提高 apt update 速度，如有需要可自行取消注释
deb https://mirrors.tuna.tsinghua.edu.cn/debian/ bullseye main contrib non-free
# deb-src https://mirrors.tuna.tsinghua.edu.cn/debian/ bullseye main contrib non-free
deb https://mirrors.tuna.tsinghua.edu.cn/debian/ bullseye-updates main contrib non-free
# deb-src https://mirrors.tuna.tsinghua.edu.cn/debian/ bullseye-updates main contrib non-free
deb https://mirrors.tuna.tsinghua.edu.cn/debian/ bullseye-backports main contrib non-free
# deb-src https://mirrors.tuna.tsinghua.edu.cn/debian/ bullseye-backports main contrib non-free
deb https://mirrors.tuna.tsinghua.edu.cn/debian-security bullseye-security main contrib non-free
# deb-src https://mirrors.tuna.tsinghua.edu.cn/debian-security bullseye-security main contrib non-free

```

**然后在把/etc/apt/sources.list.d/raspi.list所有内容替换为：**

```
deb https://mirrors.tuna.tsinghua.edu.cn/raspberrypi/ bullseye main
```

#### 复制 raspberry_init到sd上的某位置

**另外再将rpisource、头tar、Modulesyver到路径/home/pi**

---

#### 开机

#### 下载软件

`sudo apt install vim lshw git bc bison flex libssl-dev libncurses5-dev  `

## 2、重新编译内核

1、打开ath9k debug config



#### vbgmove kernel object

`sudo cp * /usr/lib/modules/5.10.92-v8+`

`sudo cp ath9k_htc`到原htc.ko所在位置

**更新依赖关系**

`sudo depmod -a`

#### 禁用其中一个wifi

```
sudo vim /etc/modprobe.d/raspi-blacklist.conf
>> blacklist brcmfmac
```

#### swap firmware

```
sudo cp /home/pi/already-built-package/firmware/htc_9271-1.4.0.fw /lib/firmware/ath9k_htc
reboot
```

重启

#### insert modules

```
sudo rmmod ath9k_htc
sudo rmmod ath9k_common
sudo rmmod ath9k_hw
sudo rmmod mac80211
sudo modprobe vmac,ath9k_hw,ath9k_common,ath9k_htc 
