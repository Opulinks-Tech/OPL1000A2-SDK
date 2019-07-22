/*
 * WPA Supplicant / main() function for UNIX like OSes and MinGW
 * Copyright (c) 2003-2007, Jouni Malinen <j@w1.fi>
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
#include <stdio.h>
#include "wpa_supplicant_i.h"
#include "../src/utils/common.h"
#include "../src/utils/os.h"
//#include "driver_i.h"
//#include "../src/drivers/driver.h"
//#include "../src/utils/wpa_debug.h"
//#include "../src/utils/wpabuf.h"

//extern struct wpa_driver_ops *wpa_drivers[];

#ifndef CONFIG_ANSI_C_EXTRA
#define CONFIG_ANSI_C_EXTRA
#endif //CONFIG_ANSI_C_EXTRA

#ifndef SIZE_T
typedef unsigned int size_t;
#endif //SIZE_T


#include "wpa_supplicant_i.h"

#include "includes.h"
#include "common.h"


int main(int argc, char *argv[])
{
	
	struct wpa_interface iface;
	int exitcode = 0;
	struct wpa_params params;
	struct wpa_global *global;

	memset(&params, 0, sizeof(params));
	params.wpa_debug_level = MSG_INFO;


	global = wpa_supplicant_init(&params);
	if (global == NULL)
		return -1;

	memset(&iface, 0, sizeof(iface));
	/* TODO: set interface parameters */

	if (wpa_supplicant_add_iface(global, &iface) == NULL)
		exitcode = -1;

	if (exitcode == 0)
		exitcode = wpa_supplicant_run(global);

	wpa_supplicant_deinit(global);

	
	return exitcode;
}


int stdout_putchar(void){
  return 0;
}


void doTestSuppliant() {

}

