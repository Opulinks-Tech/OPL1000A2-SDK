/*
 * Driver interface for RADIUS server or WPS ER only (no driver)
 * Copyright (c) 2008, Atheros Communications
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * Alternatively, this software may be distributed under the terms of BSD
 * license.
 *
 * See README and COPYING for more details.
 */

#include "includes.h"

#include "common.h"
#include "driver.h"


struct none_driver_data {
	struct hostapd_data *hapd;
	void *ctx;
};


static void * none_driver_hapd_init(struct hostapd_data *hapd,
				    struct wpa_init_params *params)
{
	struct none_driver_data *drv;

	drv = os_zalloc(sizeof(struct none_driver_data));
	if (drv == NULL) {
		wpa_printf(MSG_ERROR, "Could not allocate memory for none "
			   "driver data");
		return NULL;
	}
	drv->hapd = hapd;

	return drv;
}


static void none_driver_hapd_deinit(void *priv)
{
	struct none_driver_data *drv = priv;

	os_free(drv);
}


static int none_driver_send_ether(void *priv, const u8 *dst, const u8 *src,
				  u16 proto, const u8 *data, size_t data_len)
{
	return 0;
}


static void * none_driver_init(void *ctx, const char *ifname)
{
	struct none_driver_data *drv;

	drv = os_zalloc(sizeof(struct none_driver_data));
	if (drv == NULL) {
		wpa_printf(MSG_ERROR, "Could not allocate memory for none "
			   "driver data");
		return NULL;
	}
	drv->ctx = ctx;

	return drv;
}

struct wpa_driver_netlink_data {
	void *ctx;
	int ioctl_sock;
	struct netlink_data *netlink;
	char ifname[IFNAMSIZ + 1];
	u8 *assoc_req_ies;
	size_t assoc_req_ies_len;
	u8 *assoc_resp_ies;
	size_t assoc_resp_ies_len;
	int no_of_pmkid;
	struct ndis_pmkid_entry *pmkid;
	int we_version_compiled;
	int ap_scan;
	int scanning_done;
	u8 g_driver_down;
	//BOOLEAN	bAddWepKey;
};


static void none_driver_deinit(void *priv)
{
	struct none_driver_data *drv = priv;

	os_free(drv);
}


static int none_driver_send_eapol(void *priv, const u8 *dest, u16 proto,
				  const u8 *data, size_t data_len)
{
	return -1;
}


static int wpa_driver_netlink_get_bssid(void *priv, u8 *bssid)
{
	struct wpa_driver_ralink_data *drv = priv;

	return 0;
}

static int wpa_driver_netlink_get_ssid(void *priv, u8 *ssid)
{
	struct wpa_driver_ralink_data *drv = priv;

	return 0;
}

static int wpa_driver_netlink_set_ssid(struct wpa_driver_ralink_data *drv,
				      const u8 *ssid, size_t ssid_len)
{

	return 0;
}

static int wpa_driver_netlink_set_key(const char *ifname, void *priv,
				     enum wpa_alg alg, const u8 *addr,
				     int key_idx, int set_tx,
				     const u8 *seq, size_t seq_len,
				     const u8 *key, size_t key_len)
{
	struct wpa_driver_ralink_data *drv = priv;

	return 0;
}

static struct wpa_scan_results *
wpa_driver_netlink_get_scan_results(void *priv)
{
	struct wpa_driver_ralink_data *drv = priv;
	struct wpa_scan_results *res = NULL;

	return res;
}

static void * wpa_driver_netlink_init(void *ctx, const char *ifname)
{
	struct wpa_driver_ralink_data *drv = NULL;

	return drv;
}

static void wpa_driver_netlink_deinit(void *priv)
{
	struct wpa_driver_ralink_data *drv = priv;

}

static int wpa_driver_netlink_deauthenticate(void *priv, const u8 *addr,
					  int reason_code)
{
	struct wpa_driver_ralink_data *drv = priv;

	return 0;
}

static int wpa_driver_netlink_disassociate(void *priv, const u8 *addr,
					int reason_code)
{
	struct wpa_driver_ralink_data *drv = priv;

	return 0;
}

static int
wpa_driver_netlink_associate(void *priv,
			    struct wpa_driver_associate_params *params)
{
	struct wpa_driver_ralink_data *drv = priv;

	return 0;
}

static int wpa_driver_netlink_scan(void *priv,
				  struct wpa_driver_scan_params *params)
{
	struct wpa_driver_ralink_data *drv = priv;

	return 0;
}

static void wpa_driver_netlink_connect(void *priv)
{
	struct wpa_driver_ralink_data *drv = priv;

}

static void wpa_driver_netlink_disconnect(void *priv)
{
	struct wpa_driver_ralink_data *drv = priv;

}


/*
const struct wpa_driver_ops wpa_driver_none_ops = {
	.name;
	.desc;
	.hapd_init;
	.hapd_deinit;
	.send_ether;
	.init;
	.deinit;
	.send_eapol;
};
*/

//const struct wpa_driver_ops wpa_driver_none_ops;
const struct wpa_driver_ops wpa_driver_none_ops = {
	"netlink",
	"Netlink Wireless Client driver",
	wpa_driver_netlink_get_bssid,
	wpa_driver_netlink_get_ssid,
	wpa_driver_netlink_set_key,
	wpa_driver_netlink_init,
	wpa_driver_netlink_deinit,
    NULL,
    NULL,
    wpa_driver_netlink_deauthenticate,
	wpa_driver_netlink_disassociate,
	wpa_driver_netlink_associate,
	wpa_driver_netlink_scan,
	NULL,
};


