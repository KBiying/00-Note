ath9k

```c
static void ath9k_htc_tx(struct ieee80211_hw *hw,
             struct ieee80211_tx_control *control,
             struct sk_buff *skb)
{
    struct ieee80211_hdr *hdr;
    struct ath9k_htc_priv *priv = hw->priv;
    struct ath_common *common = ath9k_hw_common(priv->ah);
    int padpos, padsize, ret, slot;

    hdr = (struct ieee80211_hdr *) skb->data;

    /* Add the padding after the header if this is not already done */
    padpos = ieee80211_hdrlen(hdr->frame_control);
    padsize = padpos & 3;
    if (padsize && skb->len > padpos) {
        if (skb_headroom(skb) < padsize) {
            ath_dbg(common, XMIT, "No room for padding\n");
            goto fail_tx;
        }
        skb_push(skb, padsize);
        memmove(skb->data, skb->data + padsize, padpos);
    }

    slot = ath9k_htc_tx_get_slot(priv);
    if (slot < 0) {
        ath_dbg(common, XMIT, "No free TX slot\n");
        goto fail_tx;
    }

    ret = ath9k_htc_tx_start(priv, control->sta, skb, slot, false);
    if (ret != 0) {
        ath_dbg(common, XMIT, "Tx failed\n");
        goto clear_slot;
    }

    ath9k_htc_check_stop_queues(priv);

    return;

clear_slot:
    ath9k_htc_tx_clear_slot(priv, slot);
fail_tx:
    dev_kfree_skb_any(skb);
}


int ath9k_htc_tx_start(struct ath9k_htc_priv *priv,
               struct ieee80211_sta *sta,
               struct sk_buff *skb,
               u8 slot, bool is_cab)
{
    struct ieee80211_hdr *hdr;
    struct ieee80211_tx_info *tx_info = IEEE80211_SKB_CB(skb);
    struct ieee80211_vif *vif = tx_info->control.vif;
    struct ath9k_htc_sta *ista;
    struct ath9k_htc_vif *avp = NULL;
    u8 sta_idx, vif_idx;

    hdr = (struct ieee80211_hdr *) skb->data;

    /*
     * Find out on which interface this packet has to be
     * sent out.
     */
    if (vif) {
        avp = (struct ath9k_htc_vif *) vif->drv_priv;
        vif_idx = avp->index;
    } else {
        if (!priv->ah->is_monitoring) {
            ath_dbg(ath9k_hw_common(priv->ah), XMIT,
                "VIF is null, but no monitor interface !\n");
            return -EINVAL;
        }

        vif_idx = priv->mon_vif_idx;
    }

    /*
     * Find out which station this packet is destined for.
     */
    if (sta) {
        ista = (struct ath9k_htc_sta *) sta->drv_priv;
        sta_idx = ista->index;
    } else {
        sta_idx = priv->vif_sta_pos[vif_idx];
    }

    if (ieee80211_is_data(hdr->frame_control))
        ath9k_htc_tx_data(priv, vif, skb,
                  sta_idx, vif_idx, slot, is_cab);
    else
        ath9k_htc_tx_mgmt(priv, avp, skb,
                  sta_idx, vif_idx, slot);


    return htc_send(priv->htc, skb);
}
```

```c
struct ieee80211_ops ath9k_htc_ops = {
    .tx                 = ath9k_htc_tx,
    .start              = ath9k_htc_start,
    .stop               = ath9k_htc_stop,
    .add_interface      = ath9k_htc_add_interface,
    .remove_interface   = ath9k_htc_remove_interface,
    .config             = ath9k_htc_config,
    .configure_filter   = ath9k_htc_configure_filter,
    .sta_add            = ath9k_htc_sta_add,
    .sta_remove         = ath9k_htc_sta_remove,
    .conf_tx            = ath9k_htc_conf_tx,
    .sta_rc_update      = ath9k_htc_sta_rc_update,
    .bss_info_changed   = ath9k_htc_bss_info_changed,
    .set_key            = ath9k_htc_set_key,
    .get_tsf            = ath9k_htc_get_tsf,
    .set_tsf            = ath9k_htc_set_tsf,
    .reset_tsf          = ath9k_htc_reset_tsf,
    .ampdu_action       = ath9k_htc_ampdu_action,
    .sw_scan_start      = ath9k_htc_sw_scan_start,
    .sw_scan_complete   = ath9k_htc_sw_scan_complete,
    .set_rts_threshold  = ath9k_htc_set_rts_threshold,
    .rfkill_poll        = ath9k_htc_rfkill_poll_state,
    .set_coverage_class = ath9k_htc_set_coverage_class,
    .set_bitrate_mask   = ath9k_htc_set_bitrate_mask,
    .get_stats        = ath9k_htc_get_stats,
    .get_antenna        = ath9k_htc_get_antenna,
    .channel_switch_beacon    = ath9k_htc_channel_switch_beacon,

#ifdef CONFIG_ATH9K_HTC_DEBUGFS
    .get_et_sset_count  = ath9k_htc_get_et_sset_count,
    .get_et_stats       = ath9k_htc_get_et_stats,
    .get_et_strings     = ath9k_htc_get_et_strings,
#endif
};
```

Moham 修改过的函数：`ath9k_htc_get_stats`，通过`ath9k_htc_ops->get_stats`or`ieee80211_ops->get_stats` 调用

```c
 struct ieee80211_sta {
 1087     u32 supp_rates[IEEE80211_NUM_BANDS];
 1088     u8 addr[ETH_ALEN];
 1089     u16 aid;
 1090     struct ieee80211_sta_ht_cap ht_cap;
 1091     bool wme;
 1092     u8 uapsd_queues;
 1093     u8 max_sp;
 1094 
 1095     /* must be last */
 1096     u8 drv_priv[0] __attribute__((__aligned__(sizeof(void *))));
 1097 };
```

```c
struct ieee80211_hw *ieee80211_alloc_hw(size_t priv_data_len,
  538                     const struct ieee80211_ops *ops)
  539 {
  540     struct ieee80211_local *local;
  541     int priv_size, i;
  542     struct wiphy *wiphy;
  543 
  544     if (WARN_ON(!ops->tx || !ops->start || !ops->stop || !ops->config ||
  545             !ops->add_interface || !ops->remove_interface ||
  546             !ops->configure_filter))
  547         return NULL;
  548 
  549     if (WARN_ON(ops->sta_state && (ops->sta_add || ops->sta_remove)))
  550         return NULL;
  551 
  552     /* Ensure 32-byte alignment of our private data and hw private data.
  553      * We use the wiphy priv data for both our ieee80211_local and for
  554      * the driver's private data
  555      *
  556      * In memory it'll be like this:
  557      *
  558      * +-------------------------+
  559      * | struct wiphy       |
  560      * +-------------------------+
  561      * | struct ieee80211_local  |
  562      * +-------------------------+
  563      * | driver's private data   |
  564      * +-------------------------+
  565      *
  566      */
  567     priv_size = ALIGN(sizeof(*local), NETDEV_ALIGN) + priv_data_len;
  568 
  569     wiphy = wiphy_new(&mac80211_config_ops, priv_size);
  570 
  571     if (!wiphy)
  572         return NULL;
  573 
  574     wiphy->mgmt_stypes = ieee80211_default_mgmt_stypes;
  575 
  576     wiphy->privid = mac80211_wiphy_privid;
  577 
  578     wiphy->flags |= WIPHY_FLAG_NETNS_OK |
  579             WIPHY_FLAG_4ADDR_AP |
  580             WIPHY_FLAG_4ADDR_STATION |
  581             WIPHY_FLAG_REPORTS_OBSS |
  582             WIPHY_FLAG_OFFCHAN_TX;
  583 
  584     if (ops->remain_on_channel)
  585         wiphy->flags |= WIPHY_FLAG_HAS_REMAIN_ON_CHANNEL;
  586 
  587     wiphy->features = NL80211_FEATURE_SK_TX_STATUS |
  588               NL80211_FEATURE_HT_IBSS;
  589 
  590     if (!ops->set_key)
  591         wiphy->flags |= WIPHY_FLAG_IBSS_RSN;
  592 
  593     wiphy->bss_priv_size = sizeof(struct ieee80211_bss);
  594 
  595     local = wiphy_priv(wiphy);
  596 
  597     local->hw.wiphy = wiphy;
  598 
  599     local->hw.priv = (char *)local + ALIGN(sizeof(*local), NETDEV_ALIGN);
  600 
  601     local->ops = ops;
  602 
  603     /* set up some defaults */
  604     local->hw.queues = 1;
  605     local->hw.max_rates = 1;
  606     local->hw.max_report_rates = 0;
  607     local->hw.max_rx_aggregation_subframes = IEEE80211_MAX_AMPDU_BUF;
  608     local->hw.max_tx_aggregation_subframes = IEEE80211_MAX_AMPDU_BUF;
  609     local->hw.offchannel_tx_hw_queue = IEEE80211_INVAL_HW_QUEUE;
  610     local->hw.conf.long_frame_max_tx_count = wiphy->retry_long;
  611     local->hw.conf.short_frame_max_tx_count = wiphy->retry_short;
  612     local->hw.radiotap_mcs_details = IEEE80211_RADIOTAP_MCS_HAVE_MCS |
  613                      IEEE80211_RADIOTAP_MCS_HAVE_GI |
  614                      IEEE80211_RADIOTAP_MCS_HAVE_BW;
  615     local->user_power_level = -1;
  616     wiphy->ht_capa_mod_mask = &mac80211_ht_capa_mod_mask;
  617 
  618     INIT_LIST_HEAD(&local->interfaces);
  619 
  620     __hw_addr_init(&local->mc_list);
  621 
  622     mutex_init(&local->iflist_mtx);
  623     mutex_init(&local->mtx);
  624 
  625     mutex_init(&local->key_mtx);
  626     spin_lock_init(&local->filter_lock);
  627     spin_lock_init(&local->queue_stop_reason_lock);
  628 
  629     /*
  630      * The rx_skb_queue is only accessed from tasklets,
  631      * but other SKB queues are used from within IRQ
  632      * context. Therefore, this one needs a different
  633      * locking class so our direct, non-irq-safe use of
  634      * the queue's lock doesn't throw lockdep warnings.
  635      */
  636     skb_queue_head_init_class(&local->rx_skb_queue,
  637                   &ieee80211_rx_skb_queue_class);
  638 
  639     INIT_DELAYED_WORK(&local->scan_work, ieee80211_scan_work);
  640 
  641     INIT_WORK(&local->restart_work, ieee80211_restart_work);
  642 
  643     INIT_WORK(&local->reconfig_filter, ieee80211_reconfig_filter);
  644     INIT_WORK(&local->recalc_smps, ieee80211_recalc_smps_work);
  645     local->smps_mode = IEEE80211_SMPS_OFF;
  646 
  647     INIT_WORK(&local->dynamic_ps_enable_work,
  648           ieee80211_dynamic_ps_enable_work);
  649     INIT_WORK(&local->dynamic_ps_disable_work,
  650           ieee80211_dynamic_ps_disable_work);
  651     setup_timer(&local->dynamic_ps_timer,
  652             ieee80211_dynamic_ps_timer, (unsigned long) local);
  653 
  654     INIT_WORK(&local->sched_scan_stopped_work,
  655           ieee80211_sched_scan_stopped_work);
  656 
  657     spin_lock_init(&local->ack_status_lock);
  658     idr_init(&local->ack_status_frames);
  659     /* preallocate at least one entry */
  660     idr_pre_get(&local->ack_status_frames, GFP_KERNEL);
  661 
  662     sta_info_init(local);
  663 
  664     for (i = 0; i < IEEE80211_MAX_QUEUES; i++) {
  665         skb_queue_head_init(&local->pending[i]);
  666         atomic_set(&local->agg_queue_stop[i], 0);
  667     }
  668     tasklet_init(&local->tx_pending_tasklet, ieee80211_tx_pending,
  669              (unsigned long)local);
  670 
  671     tasklet_init(&local->tasklet,
  672              ieee80211_tasklet_handler,
  673              (unsigned long) local);
  674 
  675     skb_queue_head_init(&local->skb_queue);
  676     skb_queue_head_init(&local->skb_queue_unreliable);
  677 
  678     /* init dummy netdev for use w/ NAPI */
  679     init_dummy_netdev(&local->napi_dev);
  680 
  681     ieee80211_led_names(local);
  682 
  683     ieee80211_roc_setup(local);
  684 
  685     return &local->hw;
  686 }
  687 EXPORT_SYMBOL(ieee80211_alloc_hw);
```
