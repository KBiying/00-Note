## 编译模块

1. 按照之前 `buster` 操作步骤，直到执行编译 `vmac` 模块前；
2.  struct ieee80211_hw *ieee80211_alloc_hw(size_t priv_data_len,  538                     const struct ieee80211_ops *ops)  539 {  540     struct ieee80211_local *local;  541     int priv_size, i;  542     struct wiphy *wiphy;  543   544     if (WARN_ON(!ops->tx || !ops->start || !ops->stop || !ops->config ||  545             !ops->add_interface || !ops->remove_interface ||  546             !ops->configure_filter))  547         return NULL;  548   549     if (WARN_ON(ops->sta_state && (ops->sta_add || ops->sta_remove)))  550         return NULL;  551   552     /* Ensure 32-byte alignment of our private data and hw private data.  553      * We use the wiphy priv data for both our ieee80211_local and for  554      * the driver's private data  555      *  556      * In memory it'll be like this:  557      *  558      * +-------------------------+  559      * | struct wiphy       |  560      * +-------------------------+  561      * | struct ieee80211_local  |  562      * +-------------------------+  563      * | driver's private data   |  564      * +-------------------------+  565      *  566      */  567     priv_size = ALIGN(sizeof(*local), NETDEV_ALIGN) + priv_data_len;  568   569     wiphy = wiphy_new(&mac80211_config_ops, priv_size);  570   571     if (!wiphy)  572         return NULL;  573   574     wiphy->mgmt_stypes = ieee80211_default_mgmt_stypes;  575   576     wiphy->privid = mac80211_wiphy_privid;  577   578     wiphy->flags |= WIPHY_FLAG_NETNS_OK |  579             WIPHY_FLAG_4ADDR_AP |  580             WIPHY_FLAG_4ADDR_STATION |  581             WIPHY_FLAG_REPORTS_OBSS |  582             WIPHY_FLAG_OFFCHAN_TX;  583   584     if (ops->remain_on_channel)  585         wiphy->flags |= WIPHY_FLAG_HAS_REMAIN_ON_CHANNEL;  586   587     wiphy->features = NL80211_FEATURE_SK_TX_STATUS |  588               NL80211_FEATURE_HT_IBSS;  589   590     if (!ops->set_key)  591         wiphy->flags |= WIPHY_FLAG_IBSS_RSN;  592   593     wiphy->bss_priv_size = sizeof(struct ieee80211_bss);  594   595     local = wiphy_priv(wiphy);  596   597     local->hw.wiphy = wiphy;  598   599     local->hw.priv = (char *)local + ALIGN(sizeof(*local), NETDEV_ALIGN);  600   601     local->ops = ops;  602   603     /* set up some defaults */  604     local->hw.queues = 1;  605     local->hw.max_rates = 1;  606     local->hw.max_report_rates = 0;  607     local->hw.max_rx_aggregation_subframes = IEEE80211_MAX_AMPDU_BUF;  608     local->hw.max_tx_aggregation_subframes = IEEE80211_MAX_AMPDU_BUF;  609     local->hw.offchannel_tx_hw_queue = IEEE80211_INVAL_HW_QUEUE;  610     local->hw.conf.long_frame_max_tx_count = wiphy->retry_long;  611     local->hw.conf.short_frame_max_tx_count = wiphy->retry_short;  612     local->hw.radiotap_mcs_details = IEEE80211_RADIOTAP_MCS_HAVE_MCS |  613                      IEEE80211_RADIOTAP_MCS_HAVE_GI |  614                      IEEE80211_RADIOTAP_MCS_HAVE_BW;  615     local->user_power_level = -1;  616     wiphy->ht_capa_mod_mask = &mac80211_ht_capa_mod_mask;  617   618     INIT_LIST_HEAD(&local->interfaces);  619   620     __hw_addr_init(&local->mc_list);  621   622     mutex_init(&local->iflist_mtx);  623     mutex_init(&local->mtx);  624   625     mutex_init(&local->key_mtx);  626     spin_lock_init(&local->filter_lock);  627     spin_lock_init(&local->queue_stop_reason_lock);  628   629     /*  630      * The rx_skb_queue is only accessed from tasklets,  631      * but other SKB queues are used from within IRQ  632      * context. Therefore, this one needs a different  633      * locking class so our direct, non-irq-safe use of  634      * the queue's lock doesn't throw lockdep warnings.  635      */  636     skb_queue_head_init_class(&local->rx_skb_queue,  637                   &ieee80211_rx_skb_queue_class);  638   639     INIT_DELAYED_WORK(&local->scan_work, ieee80211_scan_work);  640   641     INIT_WORK(&local->restart_work, ieee80211_restart_work);  642   643     INIT_WORK(&local->reconfig_filter, ieee80211_reconfig_filter);  644     INIT_WORK(&local->recalc_smps, ieee80211_recalc_smps_work);  645     local->smps_mode = IEEE80211_SMPS_OFF;  646   647     INIT_WORK(&local->dynamic_ps_enable_work,  648           ieee80211_dynamic_ps_enable_work);  649     INIT_WORK(&local->dynamic_ps_disable_work,  650           ieee80211_dynamic_ps_disable_work);  651     setup_timer(&local->dynamic_ps_timer,  652             ieee80211_dynamic_ps_timer, (unsigned long) local);  653   654     INIT_WORK(&local->sched_scan_stopped_work,  655           ieee80211_sched_scan_stopped_work);  656   657     spin_lock_init(&local->ack_status_lock);  658     idr_init(&local->ack_status_frames);  659     /* preallocate at least one entry */  660     idr_pre_get(&local->ack_status_frames, GFP_KERNEL);  661   662     sta_info_init(local);  663   664     for (i = 0; i < IEEE80211_MAX_QUEUES; i++) {  665         skb_queue_head_init(&local->pending[i]);  666         atomic_set(&local->agg_queue_stop[i], 0);  667     }  668     tasklet_init(&local->tx_pending_tasklet, ieee80211_tx_pending,  669              (unsigned long)local);  670   671     tasklet_init(&local->tasklet,  672              ieee80211_tasklet_handler,  673              (unsigned long) local);  674   675     skb_queue_head_init(&local->skb_queue);  676     skb_queue_head_init(&local->skb_queue_unreliable);  677   678     /* init dummy netdev for use w/ NAPI */  679     init_dummy_netdev(&local->napi_dev);  680   681     ieee80211_led_names(local);  682   683     ieee80211_roc_setup(local);  684   685     return &local->hw;  686 }  687 EXPORT_SYMBOL(ieee80211_alloc_hw);c
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