# Private class
class echoes_alert_sms_router::service inherits echoes_alert_sms_router {
  if $caller_module_name != $module_name {
    fail("Use of private class ${name} by ${caller_module_name}")
  }

  $monit_file = "/etc/monit/conf.d/${echoes_alert_sms_router::service_name}"

  wt::service { $echoes_alert_sms_router::service_name:
    binary_path    => $echoes_alert_sms_router::binary_path,
    cert_file      => $echoes_alert_sms_router::cert_file,
    http_port      => $echoes_alert_sms_router::http_port,
    https          => $echoes_alert_sms_router::https,
    https_port     => $echoes_alert_sms_router::https_port,
    priv_key_file  => $echoes_alert_sms_router::priv_key_file,
    service_enable => $echoes_alert_sms_router::service_enable,
    service_ensure => $echoes_alert_sms_router::service_ensure,
    service_name   => $echoes_alert_sms_router::service_name,
  }->
  monit::check { $echoes_alert_sms_router::service_name:
    content => template("${module_name}/${echoes_alert_sms_router::branch}/${echoes_alert_sms_router::version}${monit_file}.erb"),
  }

}
