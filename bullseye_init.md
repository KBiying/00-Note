## 编译模块

1. 按照之前 `buster` 操作步骤，直到执行编译 `vmac` 模块前；
2. 修改 `vmac/hooks.c` 中的 `ieee80211_csa_is_complete` 函数，替换成 `ieee80211_beacon_cntdwn_is_complete`；
3. 修改 `vmac/main.c` 中的 `MODULE_LICENSE` 许可证由私有 `Proprietary` 改成 `Dual BSD/GPL` ；
4. 进入 `ath9k/` 进行编译，修改 `Makefile` 文件添加如下内容：

```makefile
KCOV_INSTRUMENT := n
KBUILD_EXTRA_SYMBOLS = /home/pi/V-MAC-Kernel-main/vmac/Module.symvers
```

同时记得修改 `ieee80211_rx`（两处） 和 `ath9k_htc` 的那个函数；

注意这里没有重新编译 `ath.ko`，目前想法是应该不用编译这个模块，以后有出错或者需要再重新编译。

> 找到有新旧版本有差异的函数主要是通过报错信息来确定，看好 `htc` 模块出错的信息，哪些没有定义，如果需要新定义，则去 `hooks.c` 里面重新定义一个，或者修改名字之类

## 插入模块

最后执行插入模块时，用了 `modprobe` 方法，不过用回 `insmod` 应该也是一样，这里记录一下 `modprobe` 步骤：

1. 编译出 `vmac.ko` 后，复制一份到 `/lib/modules/$(uname -r)` 中；
2. 编译出 `ath9k_xx.ko` 后，也同样复制到 `/lib/modules/$(uname -r)` 中；

> 注意 `ath9k_htc.ko` 有可能还需要替换去原系统存放该文件的目录中去，可以通过 `modinfo ath9k_htc` 查看位置，一般在 `/lib/modules/$(uname -r)/kernel/drivers/net/wireless/ath/ath9k` 中

3. 然后执行 `sudo depmod -a`，这样会修改 `/lib/modules/$(uname -r)/modules.dep` 文件，以后如果依赖不正确的话，也可以查看里面的依赖关系是否正常先；
4. 最后依次执行 `sudo modprobe vmac` `sudo modprobe ath9k_hw` `sudo modprobe ath9k_common` `sudo modprobe ath9k_htc`

# 编译userspace之前

修改文件vmac-usrsp.h和.c
