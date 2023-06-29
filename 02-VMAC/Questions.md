# Questions

## 1、

```c
/**
* 如果是interest包

    查看tx table 上有没有相同的编码

    如果没有（没有entry「entry是table上的一个条目」）
        新建一个entry（虚拟内存）
        初始化变量
        将key编码
        设置编码的到期时间
        将entry插到LET中
*/    
```

1. tx table的结构是什么？盲猜：queue

2. LET表的结构

`kernel :ath:phy0: Short RX data len, dropping(dlen:4)`

This warning is useful for developers to recognize that the device (Wi-Fi dongle or USB hub etc) is noisy but not for general users

## 2、consumer开始发送interest帧就卡住了，producer也没有收到interest帧。

```log
Message from syslogd@raspberrypi at Jan  2 16:51:41 ...
 kernel:[  398.458607] Internal error: Oops: 96000007 [#1] PREEMPT SMP

Message from syslogd@raspberrypi at Jan  2 16:51:41 ...
 kernel:[  398.459467] Code: 12001c95 f9400ec2 f9406665 b4000f62 (394b6044)
Jan  2 16:51:41 raspberrypi kernel: [  398.455831] VMAC-upper: userspace PID Registered
Jan  2 16:51:41 raspberrypi kernel: [  398.456245] VMACTX: TEST
Jan  2 16:51:41 raspberrypi kernel: [  398.456262] VMACTX: making new entry
Jan  2 16:51:41 raspberrypi kernel: [  398.456319] CALLING MOD TIMER 704909279156963608
Jan  2 16:51:41 raspberrypi kernel: [  398.456346] VMAC_MID: Data rate: 0c
Jan  2 16:51:41 raspberrypi kernel: [  398.458351] VMAC_LOW: Data rate: 12
Jan  2 16:51:41 raspberrypi kernel: [  398.458402] Unable to handle kernel paging request at virtual address ffffffc011debc78
Jan  2 16:51:41 raspberrypi kernel: [  398.458422] Mem abort info:
Jan  2 16:51:41 raspberrypi kernel: [  398.458435]   ESR = 0x96000007
Jan  2 16:51:41 raspberrypi kernel: [  398.458452]   EC = 0x25: DABT (current EL), IL = 32 bits
Jan  2 16:51:41 raspberrypi kernel: [  398.458465]   SET = 0, FnV = 0
Jan  2 16:51:41 raspberrypi kernel: [  398.458477]   EA = 0, S1PTW = 0
Jan  2 16:51:41 raspberrypi kernel: [  398.458490] Data abort info:
Jan  2 16:51:41 raspberrypi kernel: [  398.458502]   ISV = 0, ISS = 0x00000007
Jan  2 16:51:41 raspberrypi kernel: [  398.458514]   CM = 0, WnR = 0
Jan  2 16:51:41 raspberrypi kernel: [  398.458529] swapper pgtable: 4k pages, 39-bit VAs, pgdp=00000000010da000
Jan  2 16:51:41 raspberrypi kernel: [  398.458544] [ffffffc011debc78] pgd=00000001fffff003, p4d=00000001fffff003, pud=00000001fffff003, pmd=0000000100afc003, pte=0000000000000000
Jan  2 16:51:41 raspberrypi kernel: [  398.458607] Internal error: Oops: 96000007 [#1] PREEMPT SMP
Jan  2 16:51:41 raspberrypi kernel: [  398.458618] Modules linked in: rfcomm algif_hash algif_skcipher af_alg bnep hci_uart btbcm bluetooth ecdh_generic ecc 8021q garp stp llc ath9k_htc(O) vmac(O) ath9k_common(O) ath9k_hw(O) ath snd_soc_hdmi_codec cfg80211 rfkill vc4 cec v3d bcm2835_codec(C) bcm2835_v4l2(C) bcm2835_isp(C) drm_kms_helper gpu_sched bcm2835_mmal_vchiq(C) v4l2_mem2mem videobuf2_vmalloc videobuf2_dma_contig videobuf2_memops videobuf2_v4l2 raspberrypi_hwmon snd_soc_core videobuf2_common snd_compress snd_bcm2835(C) snd_pcm_dmaengine vc_sm_cma(C) videodev snd_pcm dwc2 mc i2c_brcmstb roles rpivid_mem snd_timer snd syscopyarea sysfillrect sysimgblt fb_sys_fops nvmem_rmem uio_pdrv_genirq uio i2c_dev aes_neon_bs aes_neon_blk crypto_simd cryptd drm drm_panel_orientation_quirks backlight fuse ip_tables x_tables ipv6
Jan  2 16:51:41 raspberrypi kernel: [  398.458978] CPU: 1 PID: 363 Comm: tx1 Tainted: G         C O      5.10.92-v8+ #1514
Jan  2 16:51:41 raspberrypi kernel: [  398.458988] Hardware name: Raspberry Pi 4 Model B Rev 1.4 (DT)
Jan  2 16:51:41 raspberrypi kernel: [  398.459003] pstate: 60000005 (nZCv daif -PAN -UAO -TCO BTYPE=--)
Jan  2 16:51:41 raspberrypi kernel: [  398.459040] pc : ath9k_htc_tx_start+0x50/0x2b0 [ath9k_htc]
Jan  2 16:51:41 raspberrypi kernel: [  398.459060] lr : ath9k_htc_tx+0xd0/0x104 [ath9k_htc]
Jan  2 16:51:41 raspberrypi kernel: [  398.459068] sp : ffffffc012f4bd00
Jan  2 16:51:41 raspberrypi kernel: [  398.459076] x29: ffffffc012f4bd00 x28: 0000000000000000
Jan  2 16:51:41 raspberrypi kernel: [  398.459093] x27: ffffff8106327c48 x26: ffffffc008eb2848
Jan  2 16:51:41 raspberrypi kernel: [  398.459109] x25: ffffffc008eb2878 x24: 0000000000000000
Jan  2 16:51:41 raspberrypi kernel: [  398.459125] x23: ffffffc008eb2920 x22: ffffff811790e328
Jan  2 16:51:41 raspberrypi kernel: [  398.459140] x21: 0000000000000000 x20: ffffff8101f275e0
Jan  2 16:51:41 raspberrypi kernel: [  398.459157] x19: ffffff811790e300 x18: 0000000000000001
Jan  2 16:51:41 raspberrypi kernel: [  398.459173] x17: 0000000000000000 x16: 0000000000000000
Jan  2 16:51:41 raspberrypi kernel: [  398.459188] x15: ffffffffffffffff x14: ffffffc010e1d178
Jan  2 16:51:41 raspberrypi kernel: [  398.459204] x13: ffffffc011472c10 x12: ffffffc0113154f8
Jan  2 16:51:41 raspberrypi kernel: [  398.459219] x11: 0000000000000003 x10: ffffffc0112fd4b8
Jan  2 16:51:41 raspberrypi kernel: [  398.459235] x9 : ffffffc008ebcea0 x8 : 0000000000017fe8
Jan  2 16:51:41 raspberrypi kernel: [  398.459251] x7 : c0000000ffffefff x6 : ffffffc012f4bc70
Jan  2 16:51:41 raspberrypi kernel: [  398.459266] x5 : ffffff810602f019 x4 : 0000000000000000
Jan  2 16:51:41 raspberrypi kernel: [  398.459282] x3 : 0000000000000000 x2 : ffffffc011deb9a0
Jan  2 16:51:41 raspberrypi kernel: [  398.459298] x1 : 0000000000000000 x0 : ffffff8101f275e0
Jan  2 16:51:41 raspberrypi kernel: [  398.459314] Call trace:
Jan  2 16:51:41 raspberrypi kernel: [  398.459334]  ath9k_htc_tx_start+0x50/0x2b0 [ath9k_htc]
Jan  2 16:51:41 raspberrypi kernel: [  398.459353]  ath9k_htc_tx+0xd0/0x104 [ath9k_htc]
Jan  2 16:51:41 raspberrypi kernel: [  398.459375]  vmac_low_tx+0x100/0x130 [vmac]
Jan  2 16:51:41 raspberrypi kernel: [  398.459392]  queuethread+0xfc/0x174 [vmac]
Jan  2 16:51:41 raspberrypi kernel: [  398.459416]  kthread+0x148/0x158
Jan  2 16:51:41 raspberrypi kernel: [  398.459432]  ret_from_fork+0x10/0x30
Jan  2 16:51:41 raspberrypi kernel: [  398.459467] Code: 12001c95 f9400ec2 f9406665 b4000f62 (394b6044)
Jan  2 16:51:41 raspberrypi kernel: [  398.459486] ---[ end trace 1fe506e5a873a3a3 ]---
Jan  2 16:52:11 raspberrypi kernel: [  429.027477] VMAC_CLEAN: rx cleaning started
Jan  2 16:52:11 raspberrypi kernel: [  429.027536] KB:process function in.
Jan  2 16:52:11 raspberrypi kernel: [  429.027557] CLEAN: starting process
Jan  2 16:52:11 raspberrypi kernel: [  429.027581] CLEAN: removing element
Jan  2 16:52:11 raspberrypi kernel: [  429.027610] KB:process function out.
Jan  2 16:53:21 raspberrypi kernel: [  498.457189] VMACTX: TEST
Jan  2 16:53:21 raspberrypi kernel: [  498.457207] VMACTX: making new entry
Jan  2 16:53:21 raspberrypi kernel: [  498.457265] CALLING MOD TIMER 704909279156963608
Jan  2 16:53:52 raspberrypi kernel: [  498.457291] VMAC_MID: Data rate: 0c
Jan  2 16:53:52 raspberrypi kernel: [  529.380064] VMAC_CLEAN: rx cleaning started
Jan  2 16:53:52 raspberrypi kernel: [  529.380107] KB:process function in.
Jan  2 16:53:52 raspberrypi kernel: [  529.380121] CLEAN: starting process
Jan  2 16:53:52 raspberrypi kernel: [  529.380135] CLEAN: removing element
Jan  2 16:53:52 raspberrypi kernel: [  529.380158] KB:process function out.
Jan  2 16:55:01 raspberrypi kernel: [  598.458440] VMACTX: TEST
Jan  2 16:55:01 raspberrypi kernel: [  598.458459] VMACTX: making new entry
Jan  2 16:55:01 raspberrypi kernel: [  598.458521] CALLING MOD TIMER 704909279156963608
Jan  2 16:55:32 raspberrypi kernel: [  598.458551] VMAC_MID: Data rate: 0c
Jan  2 16:55:32 raspberrypi kernel: [  629.732824] VMAC_CLEAN: rx cleaning started
Jan  2 16:55:32 raspberrypi kernel: [  629.732884] KB:process function in.
Jan  2 16:55:32 raspberrypi kernel: [  629.732905] CLEAN: starting process
Jan  2 16:55:32 raspberrypi kernel: [  629.732924] CLEAN: removing element
Jan  2 16:55:32 raspberrypi kernel: [  629.732948] KB:process function out.
Jan  2 16:56:41 raspberrypi kernel: [  698.459443] VMACTX: TEST
Jan  2 16:56:41 raspberrypi kernel: [  698.459459] VMACTX: making new entry
Jan  2 16:56:41 raspberrypi kernel: [  698.459510] CALLING MOD TIMER 704909279156963608
```

**疑点1**:当前`pc`点为`ath9k_htc_tx_start`，说明卡这里。根据call trace打印当前栈的信息，往前找调用函数，定位到`vmac_low_tx`函数

```log
pc : ath9k_htc_tx_start+0x50/0x2b0
Call trace:
ath9k_htc_tx_start+0x50/0x2b0 [ath9k_htc]
ath9k_htc_tx+0xd0/0x104 [ath9k_htc]
vmac_low_tx+0x100/0x130 [vmac]
queuethread+0xfc/0x174 [vmac]
kthread+0x148/0x158
ret_from_fork+0x10/0x30
```

**疑点2**：`VMAC_LOW: Data rate: 12`只出现了一次，说明这句之前的函数都是没问题的。

```c
/**
 * @brief    { function_description }
 *
 * @param      skb    The skb
 * @param[in]  rate    The rate
 * 
 * @code{.unparsed}
 *  copy spoofed 802.11 header format
 *  push spoofed 802.11 header to frame
 *  set flags to not fragment and request tx_status from hardware
 *  request no ACK needed for frame (i.e. 802.11 standard broadcast)
 *  call lower level driver tx function passing control struct and frame
 * @endcode
 */
void vmac_low_tx(struct sk_buff* skb, u8 rate)
{
    struct ieee80211_local *local = hw_to_local(getvhw());
    struct ieee80211_hdr hdr;
    u8 src[ETH_ALEN] __aligned(2) = {0xfe, 0xfe, 0xfe, 0xfe, 0xfe, 0xfe};
    u8 dest[ETH_ALEN]__aligned(2) = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff};
    u8 bssid[ETH_ALEN]__aligned(2) = {0xfe, 0xfe, 0xfe, 0xfe, 0xfe, 0xfe};
    hdr.duration_id = rate;
    memcpy(hdr.addr1, dest, ETH_ALEN); //was target
    memcpy(hdr.addr2, src, ETH_ALEN);// was target
    memcpy(hdr.addr3, bssid, ETH_ALEN);
    hdr.frame_control = cpu_to_le16(IEEE80211_FTYPE_DATA | IEEE80211_STYPE_DATA);
    memcpy(skb_push(skb, sizeof(struct ieee80211_hdr)), &hdr, sizeof(struct ieee80211_hdr));
    #ifdef DEBUG_VMAC
        printk(KERN_INFO "VMAC_LOW: Data rate: %d", rate);
    #endif
    IEEE80211_SKB_CB(skb)->control.vif = getvmon();
    IEEE80211_SKB_CB(skb)->flags = 0;
    skb->priority = 256 + 5;
    local->ops->tx(&local->hw, &ctr, skb);
}
```

最后一句`local->ops->tx(&local->hw, &ctr, skb);`是调用`ath9k_htc_tx`函数

![](/Users/bailey/Pictures/20161215225700390.png)

```c
//硬件相关  
static void ath9k_htc_tx(struct ieee80211_hw *hw, struct sk_buff *skb)
    ret = ath9k_htc_tx_start(priv, skb, slot, false);
        //判断是数据还是管理包，至于如何分辨的需要分析sk_buff,暂时保留
        if (ieee80211_is_data(hdr->frame_control))  
            ath9k_htc_tx_data(priv, vif, skb, sta_idx, vif_idx, slot, is_cab);
        else
            ath9k_htc_tx_mgmt(priv, avp, skb, sta_idx, vif_idx, slot);
        htc_send(priv->htc, skb);
            htc_issue_send(target, skb, skb->len, 0, tx_ctl->epid);
                hif_usb_send
                    //根据数据包的类型，发送给对应的端点
                    ret = hif_usb_send_tx(hif_dev, skb);    
                    ret = hif_usb_send_regout(hif_dev, skb);
                        usb_fill_bulk_urb(urb, hif_dev->udev, 
                            usb_sndbulkpipe(hif_dev->udev, 
                            USB_REG_OUT_PIPE),
                            skb->data, 
                            skb->len, 
                            hif_usb_regout_cb, 
                            cmd);
```

最后导致

```log
Unable to handle kernel paging request at virtual address ffffffc011debc78
.
.
Internal error: Oops: 96000007 [#1] PREEMPT SMP
.
.
```

传入参数问题？

## 3、正常运行一阵子后卡住，出现spin_lock问题

```log
Dec 25 09:02:52 raspberrypi kernel: [  737.414986] VMAC: Short GI
Dec 25 09:02:52 raspberrypi kernel: [  737.414990] VMAC: BW40
Dec 25 09:02:52 raspberrypi kernel: [  737.414995] V-MAC, Rate: 3
Dec 25 09:02:52 raspberrypi kernel: [  737.414999] VMAC: signal val: 45
Dec 25 09:02:52 raspberrypi kernel: [  737.415003] Encoding received is: 704909279156963608
Dec 25 09:02:54 raspberrypi kernel: [  739.813864] ------------[ cut here ]------------
Dec 25 09:02:54 raspberrypi kernel: [  739.813880] NETDEV WATCHDOG: eth0 (bcmgenet): transmit queue 2 timed out
Dec 25 09:02:54 raspberrypi kernel: [  739.813930] WARNING: CPU: 3 PID: 0 at net/sched/sch_generic.c:468 dev_watchdog+0x3a0/0x3a8
Dec 25 09:02:54 raspberrypi kernel: [  739.813933] Modules linked in: joydev uhid rfcomm algif_hash algif_skcipher af_alg bnep hci_uart btbcm bluetooth ecdh_generic ecc 8021q garp stp llc snd_soc_hdmi_codec ath9k_htc(O) vmac(O) ath9k_common(O) ath9k_hw(O) ath cfg80211 rfkill bcm2835_codec(C) v4l2_mem2mem bcm2835_v4l2(C) vc4 bcm2835_isp(C) bcm2835_mmal_vchiq(C) videobuf2_vmalloc videobuf2_dma_contig cec v3d videobuf2_memops videobuf2_v4l2 videobuf2_common drm_kms_helper dwc2 gpu_sched raspberrypi_hwmon videodev snd_soc_core roles i2c_brcmstb mc snd_bcm2835(C) snd_compress snd_pcm_dmaengine vc_sm_cma(C) rpivid_mem snd_pcm snd_timer snd syscopyarea sysfillrect sysimgblt fb_sys_fops uio_pdrv_genirq uio nvmem_rmem i2c_dev aes_neon_bs aes_neon_blk crypto_simd cryptd drm drm_panel_orientation_quirks backlight fuse ip_tables x_tables ipv6
Dec 25 09:02:54 raspberrypi kernel: [  739.814080] CPU: 3 PID: 0 Comm: swapper/3 Tainted: G         C O      5.10.92-v8+ #1514
Dec 25 09:02:54 raspberrypi kernel: [  739.814082] Hardware name: Raspberry Pi 4 Model B Rev 1.4 (DT)
Dec 25 09:02:54 raspberrypi kernel: [  739.814087] pstate: 80000005 (Nzcv daif -PAN -UAO -TCO BTYPE=--)
Dec 25 09:02:54 raspberrypi kernel: [  739.814091] pc : dev_watchdog+0x3a0/0x3a8
Dec 25 09:02:54 raspberrypi kernel: [  739.814094] lr : dev_watchdog+0x3a0/0x3a8
Dec 25 09:02:54 raspberrypi kernel: [  739.814096] sp : ffffffc0115d3d10
Dec 25 09:02:54 raspberrypi kernel: [  739.814099] x29: ffffffc0115d3d10 x28: ffffff8101593580 
Dec 25 09:02:54 raspberrypi kernel: [  739.814105] x27: 0000000000000004 x26: 0000000000000140 
Dec 25 09:02:54 raspberrypi kernel: [  739.814111] x25: 00000000ffffffff x24: 0000000000000003 
Dec 25 09:02:54 raspberrypi kernel: [  739.814116] x23: ffffffc011286000 x22: ffffff81016303dc 
Dec 25 09:02:54 raspberrypi kernel: [  739.814121] x21: ffffff8101630000 x20: ffffff8101630480 
Dec 25 09:02:54 raspberrypi kernel: [  739.814127] x19: 0000000000000002 x18: 0000000000000000 
Dec 25 09:02:54 raspberrypi kernel: [  739.814133] x17: 0000000000000000 x16: 0000000000000000 
Dec 25 09:02:54 raspberrypi kernel: [  739.814138] x15: ffffffffffffffff x14: ffffffc010e1d178 
Dec 25 09:02:54 raspberrypi kernel: [  739.814143] x13: ffffffc011472c10 x12: ffffffc011308a30 
Dec 25 09:02:54 raspberrypi kernel: [  739.814148] x11: 0000000000000003 x10: ffffffc0112ced30 
Dec 25 09:02:54 raspberrypi kernel: [  739.814154] x9 : ffffffc0100e6558 x8 : 000000000000b520 
Dec 25 09:02:54 raspberrypi kernel: [  739.814159] x7 : c00000010014678c x6 : ffffffc0115d3970 
Dec 25 09:02:54 raspberrypi kernel: [  739.814164] x5 : 0000000000000001 x4 : 0000000000000000 
Dec 25 09:02:54 raspberrypi kernel: [  739.814169] x3 : 0000000000000103 x2 : 0000000000000102 
Dec 25 09:02:54 raspberrypi kernel: [  739.814175] x1 : 07990790d9daf000 x0 : 0000000000000000 
Dec 25 09:02:54 raspberrypi kernel: [  739.814180] Call trace:
Dec 25 09:02:54 raspberrypi kernel: [  739.814184]  dev_watchdog+0x3a0/0x3a8
Dec 25 09:02:54 raspberrypi kernel: [  739.814191]  call_timer_fn+0x38/0x200
Dec 25 09:02:54 raspberrypi kernel: [  739.814195]  run_timer_softirq+0x298/0x548
Dec 25 09:02:54 raspberrypi kernel: [  739.814198]  __do_softirq+0x1a8/0x510
Dec 25 09:02:54 raspberrypi kernel: [  739.814203]  irq_exit+0xe8/0x108
Dec 25 09:02:54 raspberrypi kernel: [  739.814207]  __handle_domain_irq+0xa0/0x110
Dec 25 09:02:54 raspberrypi kernel: [  739.814210]  gic_handle_irq+0xb0/0xf0
Dec 25 09:02:54 raspberrypi kernel: [  739.814213]  el1_irq+0xcc/0x180
Dec 25 09:02:54 raspberrypi kernel: [  739.814217]  arch_cpu_idle+0x18/0x28
Dec 25 09:02:54 raspberrypi kernel: [  739.814221]  default_idle_call+0x58/0x1d4
Dec 25 09:02:54 raspberrypi kernel: [  739.814226]  do_idle+0x25c/0x270
Dec 25 09:02:54 raspberrypi kernel: [  739.814230]  cpu_startup_entry+0x30/0x70
Dec 25 09:02:54 raspberrypi kernel: [  739.814234]  secondary_start_kernel+0x168/0x178
Dec 25 09:02:54 raspberrypi kernel: [  739.814237] ---[ end trace f3e30a4dfa5833b8 ]---
Dec 25 09:03:13 raspberrypi kernel: [  758.414032] rcu: INFO: rcu_preempt self-detected stall on CPU
Dec 25 09:03:13 raspberrypi kernel: [  758.414048] rcu:     0-....: (1 GPs behind) idle=49e/1/0x4000000000000004 softirq=23529/23530 fqs=2562 
Dec 25 09:03:13 raspberrypi kernel: [  758.414053]     (t=5250 jiffies g=27585 q=7601)
Dec 25 09:03:13 raspberrypi kernel: [  758.414056] Task dump for CPU 0:
Dec 25 09:03:13 raspberrypi kernel: [  758.414060] task:DACKGEN         state:R  running task     stack:    0 pid:  312 ppid:     2 flags:0x0000002a
Dec 25 09:03:13 raspberrypi kernel: [  758.414069] Call trace:
Dec 25 09:03:13 raspberrypi kernel: [  758.414082]  dump_backtrace+0x0/0x1b8
Dec 25 09:03:13 raspberrypi kernel: [  758.414086]  show_stack+0x20/0x30
Dec 25 09:03:13 raspberrypi kernel: [  758.414090]  sched_show_task+0x16c/0x190
Dec 25 09:03:13 raspberrypi kernel: [  758.414096]  dump_cpu_task+0x4c/0x5c
Dec 25 09:03:13 raspberrypi kernel: [  758.414099]  rcu_dump_cpu_stacks+0xb8/0xf8
Dec 25 09:03:13 raspberrypi kernel: [  758.414104]  rcu_sched_clock_irq+0xa04/0xd18
Dec 25 09:03:13 raspberrypi kernel: [  758.414109]  update_process_times+0x68/0xe0
Dec 25 09:03:13 raspberrypi kernel: [  758.414113]  tick_sched_handle.isra.19+0x48/0x60
Dec 25 09:03:13 raspberrypi kernel: [  758.414116]  tick_sched_timer+0x60/0xb8
Dec 25 09:03:13 raspberrypi kernel: [  758.414120]  __hrtimer_run_queues+0x11c/0x470
Dec 25 09:03:13 raspberrypi kernel: [  758.414124]  hrtimer_interrupt+0xfc/0x258
Dec 25 09:03:13 raspberrypi kernel: [  758.414131]  arch_timer_handler_phys+0x38/0x48
Dec 25 09:03:13 raspberrypi kernel: [  758.414135]  handle_percpu_devid_irq+0xa8/0x2a0
Dec 25 09:03:13 raspberrypi kernel: [  758.414139]  generic_handle_irq+0x38/0x50
Dec 25 09:03:13 raspberrypi kernel: [  758.414142]  __handle_domain_irq+0x9c/0x110
Dec 25 09:03:13 raspberrypi kernel: [  758.414145]  gic_handle_irq+0xb0/0xf0
Dec 25 09:03:13 raspberrypi kernel: [  758.414148]  el1_irq+0xcc/0x180
Dec 25 09:03:13 raspberrypi kernel: [  758.414153]  queued_spin_lock_slowpath+0x1f0/0x400
Dec 25 09:03:13 raspberrypi kernel: [  758.414157]  _raw_spin_lock+0x74/0x78
Dec 25 09:03:13 raspberrypi kernel: [  758.414174]  request_DACK+0x64/0xa0 [vmac]
Dec 25 09:03:13 raspberrypi kernel: [  758.414180]  vmac_rx+0x3a0/0x5a0 [vmac]
Dec 25 09:03:13 raspberrypi kernel: [  758.414186]  ieee80211_rx_vmac+0xe8/0x110 [vmac]
Dec 25 09:03:13 raspberrypi kernel: [  758.414195]  ath9k_rx_tasklet+0xf0/0x1c0 [ath9k_htc]
Dec 25 09:03:13 raspberrypi kernel: [  758.414200]  tasklet_action_common.isra.20+0x15c/0x1a0
Dec 25 09:03:13 raspberrypi kernel: [  758.414203]  tasklet_action+0x40/0x50
Dec 25 09:03:13 raspberrypi kernel: [  758.414206]  __do_softirq+0x1a8/0x510
Dec 25 09:03:13 raspberrypi kernel: [  758.414210]  irq_exit+0xe8/0x108
Dec 25 09:03:13 raspberrypi kernel: [  758.414213]  __handle_domain_irq+0xa0/0x110
Dec 25 09:03:13 raspberrypi kernel: [  758.414216]  gic_handle_irq+0xb0/0xf0
Dec 25 09:03:13 raspberrypi kernel: [  758.414219]  el1_irq+0xcc/0x180
Dec 25 09:03:13 raspberrypi kernel: [  758.414225]  dackgen+0x70/0xb0 [vmac]
Dec 25 09:03:13 raspberrypi kernel: [  758.414230]  kthread+0x148/0x158
Dec 25 09:03:13 raspberrypi kernel: [  758.414233]  ret_from_fork+0x10/0x30
```

**疑点1**：`rcu: INFO: rcu_preempt self-detected stall on CPU`意思是CPU长时间超负荷工作。

spin_lock是自旋锁，因为系统里面有线程、锁、睡眠，持有锁时睡眠会导致死锁问题，但我看代码，上锁解锁之间并没有调用有关睡眠的函数。

```c
/**
 * @brief      Adds a DACK creation request for appropriate encoding and round number
 *
 * @param[in]  enc    encoding of interest name
 * @param[in]  round  round i.e. 5 * latest sequence of data frame received
 */
void request_DACK(u64 enc, u16 round)
{
    struct vmac_queue* item;
    //prepDACK(enc, round);
    item = kmalloc(sizeof(struct vmac_queue), GFP_KERNEL);
    if (item)
    {
        item->enc = enc;
        item->seq = round;       
        spin_lock(&dackfreelok);
        dackreqnum++;
        list_add_tail( &(item->list), &(dqueue.list));
        spin_unlock(&dackfreelok);
    }
}
```

`list_add_tail()`是linux内核双向链表`list_head`的拓展接口，作用：从链表尾处向前添加节点的接口

 request_DACK+0x64/0xc0 [vmac] 0x64是函数开始位置的偏移



## 4、rmmod ath9k_htc后卡住

因为vmac模块很多处调用了while(1)，程序没办法终止，且一直在调用ath9k_htc模块，因此rmmod会出现问题  

吗





[你好，这是必应。我为你的查询“rcu_preempt self-detected stall on CPU”找到了一些结果。rcu_preempt是一个内核配置选项，用于在可抢占的内核中实现RCU（读-复制-更新）机制](https://www.kernel.org/doc/html/latest/RCU/stallwarn.html)[1](https://www.kernel.org/doc/html/latest/RCU/stallwarn.html)[。RCU是一种同步机制，用于允许多个读者和少数写者并发访问共享数据结构，而不需要锁](https://www.kernel.org/doc/html/latest/RCU/stallwarn.html)[1](https://www.kernel.org/doc/html/latest/RCU/stallwarn.html)。

[当RCU检测到某个CPU或任务长时间没有响应RCU回调函数时，就会报告一个CPU停滞（stall）的错误](https://www.kernel.org/doc/html/latest/RCU/stallwarn.html)[1](https://www.kernel.org/doc/html/latest/RCU/stallwarn.html)。这可能是由于以下原因之一：

- [有一个实时进程占用了所有CPU资源，并且由于其实时调度优先级，系统没有足够的资源来执行其他任务](https://unix.stackexchange.com/questions/252045/rcu-preempt-detected-stalls-on-cpus-tasks-message-appears-to-continue)[2](https://unix.stackexchange.com/questions/252045/rcu-preempt-detected-stalls-on-cpus-tasks-message-appears-to-continue)。
- [有一个用户空间进程或驱动程序阻止了中断或抢占，导致RCU无法完成宽限期](https://stackoverflow.com/questions/35401317/rcu-preempt-self-detected-stall-on-cpu-0)[3](https://stackoverflow.com/questions/35401317/rcu-preempt-self-detected-stall-on-cpu-0)。
- [系统过载或负载不均衡，导致某些CPU无法及时处理RCU回调函数](https://www.kernel.org/doc/html/latest/RCU/stallwarn.html)[1](https://www.kernel.org/doc/html/latest/RCU/stallwarn.html)。

解决这个问题的思路可能有以下几种：

- [禁用CONFIG_PREEMPT_RCU配置选项（但这需要重新编译内核），这样RCU读取段就不会被用户空间进程抢占](https://stackoverflow.com/questions/35401317/rcu-preempt-self-detected-stall-on-cpu-0)[3](https://stackoverflow.com/questions/35401317/rcu-preempt-self-detected-stall-on-cpu-0)。
- [调整实时进程的优先级或时间片长度，使其不要长时间霸占CPU资源](https://unix.stackexchange.com/questions/252045/rcu-preempt-detected-stalls-on-cpus-tasks-message-appears-to-continue)[2](https://unix.stackexchange.com/questions/252045/rcu-preempt-detected-stalls-on-cpus-tasks-message-appears-to-continue)。
- [检查用户空间进程或驱动程序是否存在死锁、死循环、内存泄漏等问题，导致阻塞中断或抢占](https://stackoverflow.com/questions/35401317/rcu-preempt-self-detected-stall-on-cpu-0)[3](https://stackoverflow.com/questions/35401317/rcu-preempt-self-detected-stall-on-cpu-0)[1](https://www.kernel.org/doc/html/latest/RCU/stallwarn.html)。
- [使用sysfs或boot-time参数来禁用或调整RCU的CPU停滞检测器的行为](https://www.kernel.org/doc/html/latest/RCU/stallwarn.html)[1](https://www.kernel.org/doc/html/latest/RCU/stallwarn.html)。

```
 Call trace:
  dev_watchdog+0x3a0/0x3a8
  call_timer_fn+0x38/0x200
  run_timer_softirq+0x298/0x548
  __do_softirq+0x1a8/0x510
  irq_exit+0xe8/0x108
  __handle_domain_irq+0xa0/0x110
  gic_handle_irq+0xb0/0xf0
  el1_irq+0xcc/0x180
  arch_cpu_idle+0x18/0x28
  default_idle_call+0x58/0x1d4
  do_idle+0x25c/0x270
  cpu_startup_entry+0x30/0x70
  secondary_start_kernel+0x168/0x178
 ---[ end trace f3e30a4dfa5833b8 ]---
 rcu: INFO: rcu_preempt self-detected stall on CPU
 rcu:     0-....: (1 GPs behind) idle=49e/1/0x4000000000000004 softirq=23529/23530 fqs=2562 
     (t=5250 jiffies g=27585 q=7601)
 Task dump for CPU 0:
 task:DACKGEN         state:R  running task     stack:    0 pid:  312 ppid:     2 flags:0x0000002a
 Call trace:
  dump_backtrace+0x0/0x1b8
  show_stack+0x20/0x30
  sched_show_task+0x16c/0x190
  dump_cpu_task+0x4c/0x5c
  rcu_dump_cpu_stacks+0xb8/0xf8
  rcu_sched_clock_irq+0xa04/0xd18
  update_process_times+0x68/0xe0
  tick_sched_handle.isra.19+0x48/0x60
  tick_sched_timer+0x60/0xb8
  __hrtimer_run_queues+0x11c/0x470
  hrtimer_interrupt+0xfc/0x258
  arch_timer_handler_phys+0x38/0x48
  handle_percpu_devid_irq+0xa8/0x2a0
  generic_handle_irq+0x38/0x50
  __handle_domain_irq+0x9c/0x110
  gic_handle_irq+0xb0/0xf0
  el1_irq+0xcc/0x180
  queued_spin_lock_slowpath+0x1f0/0x400
  _raw_spin_lock+0x74/0x78
  request_DACK+0x64/0xa0 [vmac]
  vmac_rx+0x3a0/0x5a0 [vmac]
  ieee80211_rx_vmac+0xe8/0x110 [vmac]
  ath9k_rx_tasklet+0xf0/0x1c0 [ath9k_htc]
  tasklet_action_common.isra.20+0x15c/0x1a0
  tasklet_action+0x40/0x50
  __do_softirq+0x1a8/0x510
  irq_exit+0xe8/0x108
  __handle_domain_irq+0xa0/0x110
  gic_handle_irq+0xb0/0xf0
  el1_irq+0xcc/0x180
  dackgen+0x70/0xb0 [vmac]
  kthread+0x148/0x158
  ret_from_fork+0x10/0x30
```



这段dmesg显示了一个内核调用栈信息，它显示了在某个时刻内核中正在执行的函数调用序列。从这个调用栈中，我们可以看到一些有关内核状态的信息。例如，我们可以看到这个调用栈涉及到了 `vmac` 模块的 `request_DACK` 和 `vmac_rx` 函数，以及 `ath9k_htc` 模块的 `ath9k_rx_tasklet` 函数。

此外，这段dmesg还显示了一个RCU自检测到的CPU停滞问题。RCU（Read-Copy-Update）是一种同步机制，它能够让多个读者和一个写者并发地访问数据结构。当RCU检测到CPU停滞时，它会输出一些诊断信息来帮助开发人员定位问题。
