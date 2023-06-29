 1、写脚本，例如

```    shell
#!/bin/sh
cd /home/pi
python3 test.py
```

2、给脚本文件添加执行权限
    `sudo chmod 777 hh.sh`
3、向rc.local文件添加启动代码
    `sudo nano /etc/rc.local`
4、在打开的文本中找到exit 0，在此之前添加的代码在启动时都会被执行，在exit 0 之前添加一行代码：
      `su pi -c "exec /home/pi/Desktop/demo/pytorch-expression-detection/hh.sh`

​    ctrl+o保存，ctrl+x退出，然后在终端输入：sudo reboot ,重启系统测试。
​    su命令是指定在pi用户下执行这条命令，-c 表示执行完这条命令之后恢复原来的用户。
​    注意：系统启动时在执行这段代码时是使用root用户权限的，如果不指定pi用户，可能会因为权限问题
导致脚本执行失败。

5、在/etc/init.d/目录下新建一个服务脚本文件
    例如 first_start_up
    sudo vi /etc/init.d/first_start_up
    写入如下内容
        case "$1" in
         start)
         echo "start your app here."
         su pi -c "sh /home/pi/Desktop/demo/pytorch-expression-detection/hh.sh"
         ;;
         stop)
         echo "stop your app here."
         ;;
         *)
         echo "Usage: service first_start_up start|stop"
         exit 1
         ;;
    esac
    exit 0
6、设置脚本可执行权限
    sudo chmod 777 /etc/init.d/first_start_up
7、将该脚本作为服务设置开机自动加载
    sudo update-rc.d first_start_up defaults
8、重启测试
    sudo reboot
9、service 命令 启动/停止 脚本
    sudo service first_start_up start
    sudo service first_start_up stop




要验证rc.local是否成功执行，每次都需要重启比较麻烦，可以先用下面命令做测试。不过由于在开机启动的时候不同服务优先级不同，可能会出现一些依赖还没完成，所以最终还是要手动重启试一下。





```bash
sudo systemctl restart rc-local.service
sudo systemctl status rc-local.service
```

作者：linzelin
链接：https://www.jianshu.com/p/8e5d5497dd24
来源：简书
著作权归作者所有。商业转载请联系作者获得授权，非商业转载请注明出处。

