<?php
/*
 * cloudinit.inc
 *
 * part of pfSense (https://www.pfsense.org)
 * Copyright (c) 2017 Rubicon Communications, LLC (Netgate)
 * All rights reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

require_once("functions.inc");
require_once("globals.inc");
require_once("pkg-utils.inc");
require_once("service-utils.inc");
require_once("util.inc");

function cloudinit_resync_config() {
	global $g, $config;

	$cloudinit_conf = array();
	if (is_array($config['installedpackages']['cloudinit'])) {
		foreach ($config['installedpackages']['cloudinit']['config'][0] as $k => $v) {
			$cloudinit_conf[$k] = addslashes($v);
		}
	}

	conf_mount_rw();

	/* disable cloudinit if not enabled */
	if ($cloudinit_conf['enable'] != "on") {
		if (is_service_running("cloudinit")) {
			stop_service("cloudinit");
		}
		unlink_if_exists("/usr/local/etc/rc.d/cloudinit.sh");
		return;
	}

	/* generate cloudinit.sh rcfile */
	$pidfile = "{$g['varrun_path']}/cloudinit.pid";
	$logfile = "{$g['varlog_path']}/cloudinit.log";
	write_rcfile(array(
		"file" => "cloudinit.sh",
		"start" => "/usr/sbin/daemon -crP {$pidfile} /usr/local/etc/rc.d/cloudinit start 2> {$logfile}",
		"stop" => "/bin/kill `/bin/cat {$pidfile}`"
		)
	);

	/* (re)start service */
	if (is_service_running("cloudinit")) {
		restart_service("cloudinit");
	} else {
		start_service("cloudinit");
	}

	sleep(1);
	conf_mount_ro();
}
?>
