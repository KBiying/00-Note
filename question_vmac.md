# 1.

### 现象：

出现很多`ERROR: Received type= 255`. 这种类型的80211mac帧头部目的地止是0xfe，但vmac帧类型不属于interest、data、dack、injected、announcement。在传输过程中，出现`ERROR: Received type= 255后有时候可恢复成正常传输，但时间长了之后就不可恢复。

### 疑问：

255类型的帧只在第一次时候设置过，表示检查 用户空间与内核空间是否能通过vmac正常通信。但是这操作只进行一次，**因此后续接收到的255类型的帧，是哪里来的？**

# 2.

### 现象：

consumer开始发送interest帧就卡住了，producer也没有收到interest帧。

出现`Unable to handle kernel paging request at virtual address ffffffc011debc78`并导致`Internal error: Oops: 96000007 [#1] PREEMPT SMP`

分析call trace。

出错位置是ath9k驱动调用`ath9k_htc_tx_start()`函数。

> `vmac_low_tx()`函数中调用`local->ops->tx(&local->hw, &ctr, skb);`调用ath9k驱动函数：`ath9k_htc_tx_start()`

```log
Call trace:
 ath9k_htc_tx_start+0x50/0x2b0 [ath9k_htc]
 ath9k_htc_tx+0xd0/0x104 [ath9k_htc]
 vmac_low_tx+0x100/0x130 [vmac]
```

### 疑问：

会不会是传入参数的问题？

Full log：

```log
Message from syslogd@raspberrypi at Jan  2 16:51:41 ...
 kernel:[  398.458607] Internal error: Oops: 96000007 [#1] PREEMPT SMP

Message from syslogd@raspberrypi at Jan  2 16:51:41 ...

Jan  2 16:51:41 raspberrypi kernel: [  398.458607] Internal error: Oops: 96000007 [#1] PREEMPT SMP
Jan  2 16:51:41 raspberrypi kernel: [  398.458618] Modules linked in: rfcomm algif_hash algif_skcipher af_alg bnep hci_uart btbcm bluetooth ecdh_generic ecc 8021q garp stp llc ath9k_htc(O) vmac(O) ath9k_common(O) ath9k_hw(O) ath snd_soc_hdmi_codec cfg80211 rfkill vc4 cec v3d bcm2835_codec(C) bcm2835_v4l2(C) bcm2835_isp(C) drm_kms_helper gpu_sched bcm2835_mmal_vchiq(C) v4l2_memmem videobuf2_vmalloc videobuf2_dma_contig videobuf2_memops videobuf2_v4l2 raspberrypi_hwmon snd_soc_core videobuf2_common snd_compress snd_bcm2835(C) snd_pcm_dmaengine vc_sm_cma(C) videodev snd_pcm dwc2 mc i2c_brcmstb roles rpivid_mem snd_timer snd syscopyarea sysfillrect sysimgblt fb_sys_fops nvmem_rmem uio_pdrv_genirq uio i2c_dev aes_neon_bs aes_neon_blk crypto_simd cryptd drm drm_panel_orientation_quirks backlight fuse ip_tables x_tables ipv6
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
```
