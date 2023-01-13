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




```


