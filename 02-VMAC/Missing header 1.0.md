# -CD Missing corresponding kernel header

## 环境

* **设备：raspberry 4**
* **操作系统 : raspbian **
* **内核版本: 4.19.79-v7l+**
* **本文有参考类似情况的博客 **[https://blog.csdn.net/dxxdxxd/article/details/108302135](https://blog.csdn.net/dxxdxxd/article/details/108302135)

## 问题描述

> **直接加载别人修改的 wifi 驱动模块后，**
>
> **出现了问题 **`insmod:ERROR: could not insert module ath.ko: Unknown symbol in module`，类似的好几个`.ko`文件都加载不成功。出现问题是因为A版本内核编译出来的模块，并不适用于B版本的内核。因此问题解决方案是重新编译修改后的wifi驱动源码`make -C /lib/modules/$(uname -r)/build M=$(pwd) modules`。
>
> **又出现了问题:**`***/lib/modules/4.19.79-v7l+/build: No such file or directory  stop`。意思是缺少build文件。因此问题解决方案是重新编译内核源码，但问题是我们不想要最新的内核版本，只想要对应4.19.79-v7l+内核版本。遂指定内核版本`apt install linux=headers-$(uname -r)` 。
>
> **又出现了问题:**`E：couldnot find any package by regex 'linux-headers-4.19.79-v7l'`。因此问题解决方案是网上扒源码。

## 下载内核源码

**1、先安装必要的包**

```
$ sudo apt install git bc bison flex libssl-dev libncurses5-dev   
```

**2、下载 rpi-source文件：**

**方法一：资源链接：**[https://github.com/RPi-Distro/rpi-source](https://github.com/RPi-Distro/rpi-source)

**方法二: 命令行**

```
$ sudo wget https://raw.githubusercontent.com/notro/rpi-source/master/rpi-source -O /usr/local/bin/rpi-source && sudo chmod +x /usr/local/bin/rpi-source && /usr/local/bin/rpi-source -q --tag-update
```

**3、更改权限后执行rpi-source文件，得到一个链接**

```
$ chmod 777 rpi-source
$ rpi-source
.
.
https://raw.githubusercontent.com/raspberrypi/firmware/ef72c17bcaaeb890
93d87bcf71f3228e1b5e1fff/extra/git_hash
.
.
```

* **根据所给的链接得到新的commit值（我的是f16e91dad2af9d57aef477cc1f522040353849f5），有3个用处**
  **1、下载对应版本的内核源码，放到/home/pi路径下**
  [https://github.com/raspberrypi/linux/archive/f16e91dad2af9d57aef477cc1f522040353849f5.tar.gz](https://github.com/raspberrypi/linux/archive/f16e91dad2af9d57aef477cc1f522040353849f5.tar.gz)
  **2、下载modulde.symver，放到/home/pi路径下**
  [https://raw.githubusercontent.com/raspberrypi/firmware/ef72c17bcaaeb89093d87bcf71f3228e1b5e1fff/extra/Module7l.symvers](https://raw.githubusercontent.com/raspberrypi/firmware/ef72c17bcaaeb89093d87bcf71f3228e1b5e1fff/extra/Module7l.symvers)
  **（不要）3、将417行**`download_to(kernel.symvers, os.path.join(linux_dir, "Module.symvers"))`修改为`sh("cp /home/pi/Module7l.symvers %s/Module.symvers" % linux_dir)`

**		**4、将271行`kernel.git_hash = download("%s/%s/extra/git_hash" % (repo_raw, fw_rev)).strip()` 修改为`kernel.git_hash = "f16e91dad2af9d57aef477cc1f522040353849f5"`

## 编译内核源码

```
$ rpi-source --dest=/home/pi/ --verbose
```

**解释一下：由于已经下载好对应内核源码， 需指定编译内核源码。--dest是给脚本传入一个路径参数，这个路经下有刚下载好的内核源码tar包。而 --verbose是用于debug**

**注意：多次执行**`rpi-source`命令时需注意，先执行命令`rm -rf linux-f16e91dad2af9d57aef477cc1f522040353849f5`。由于每次执行`rpi-source`后，系统会解压tar包。根据354的if语句。当想再次执行`rpi-source`时，由于linux_dir目录存在，会直接退出执行。

```
$ rpi-source --dest=/home/pi/ --verbose 

 *** SoC: BCM2835

 *** rpi-update: https://github.com/Hexxeh/rpi-firmware

 *** Firmware revision: e1050e94821a70b2e4c72b318d6c6c968552e9a2

 *** Linux source commit: f16e91dad2af9d57aef477cc1f522040353849f5

 *** Download kernel source: Already downloaded /home/pi/linux-f16e91dad2af9d57aef477cc1f522040353849f5.tar.gz

 *** Unpack kernel source
...............................................................................................................................................................................................................................................................................................................................................................................................................................................................................................................................................................................................................................................................................................................................................................................................................................................................
 *** Add '+' to kernel release string

 *** Create symlink: /home/pi/linux

 *** Create /lib/modules/<ver>/{build,source} symlinks

 *** .config

 *** Module.symvers

 *** make modules_prepare
  HOSTCC  scripts/basic/fixdep
  HOSTCC  scripts/kconfig/conf.o
  YACC    scripts/kconfig/zconf.tab.c
  LEX     scripts/kconfig/zconf.lex.c
  HOSTCC  scripts/kconfig/zconf.tab.o
  HOSTLD  scripts/kconfig/conf
scripts/kconfig/conf  --syncconfig Kconfig
*
* Restart config...
*
*
* Kernel hacking
*
Magic SysRq key (MAGIC_SYSRQ) [Y/?] y
  Enable magic SysRq key functions by default (MAGIC_SYSRQ_DEFAULT_ENABLE) [0x1] 0x1
  Enable magic SysRq key over serial (MAGIC_SYSRQ_SERIAL) [Y/n/?] y
Kernel debugging (DEBUG_KERNEL) [Y/?] y
Code coverage for fuzzing (KCOV) [N/y/?] (NEW) 

```

**可以看到顺利开始安装对应版本的头文件，在编译 modules_prepare 时，会卡住，这里接受默认值，回车即可。**

**至此可看到 /lib/modules/$(uname -r)/路径下有build文件了**
