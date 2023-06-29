Subject: Inquiry Regarding V-MAC

Dear Mohammed Elbadry,

I hope this email finds you well. My name is Changkang Mo and I am reaching out to you regarding the research work with my tutor, Yaodong Huang.

I have been studying code of V-MAC. But I encountered a few questions in experiments part. I would be very appreciated if you could give us some help.

Here are some questions:

1. Have you been encountered the problem which is shown at Figure 1? We met this problem in our experiments. Specifically, this error occasionally occurs in one-to-one scenarios and becomes more erratic as the number of receivers increases. It still works fine with three receivers, but once the number of receivers reaches four, the problem is more likely to occur.

2. In our current experimental environment (in an office with many additional 2.4GHz Wi-Fi signals), we conducted one-to-one transmission experiments (with your Userspace code) and achieved a goodput range of 2-4Mbps. This is significantly lower than our results from last year (at least 7-8Mbps). We are considering that the issue may be due to device aging or increasing complexity of signal interference, among other factors. 

  In summary, our experiments are not very satisfactory, and do you have any plan to release a newer version of V-MAC, using a better dongle. If there are any bugs in the new code, maybe we can try to help with the repairs.

3. While we use recomplied vmac module on bullseye version, Some odd things happened. The system will be freezed. Have you been encountered this problem? According to call trace(partly shown as below, fully shown in the attachment <kern1.log>), the reason was pointed to ath9k_htc_tx_start() function, which is called by local->ops->tx(&local->hw, &ctr, skb) using in vmac_low_tx() function. I am wondering whether it is because of NULL point of skb passed to ath9k?

   Call trace:
    ath9k_htc_tx_start+0x50/0x2b0 [ath9k_htc]
    ath9k_htc_tx+0xd0/0x104 [ath9k_htc]
    vmac_low_tx+0x100/0x130 [vmac]

4. Still, sometimes the spin_lock will cause freeze  on bullseye version as well. Call trace is shown in attachment <kern2.log>. I've tried to debug it with gdb, and it turns out to be the problem of dackreqnum++ in the request_DACK() function. And I am confused about this. Actually, I am not quite familiar with GDB. Have you been met this problem and debug with gdb? Any advice of debugging?

   

Thank you in advance for considering my request. Your insights would greatly help us. We are look forward to hearing from you and discussing these matters at your convenience.

Best regards,

Changkang Mo
mochangkang2022@email.szu.edu.cn