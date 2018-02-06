<?php
global $priv_list;
$priv_list['page-services-cloudinit'] = array();
$priv_list['page-services-cloudinit']['name'] = "WebCfg - Services: cloudinit";
$priv_list['page-services-cloudinit']['descr'] = "Allow access to cloudinit package GUI";
$priv_list['page-services-cloudinit']['match'] = array();
$priv_list['page-services-cloudinit']['match'][] = "pkg_edit.php?xml=cloudinit.xml*";
?>
