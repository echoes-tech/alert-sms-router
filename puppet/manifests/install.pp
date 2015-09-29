# Private class
class echoes_alert_sms_router::install inherits echoes_alert_sms_router {
  if $caller_module_name != $module_name {
    fail("Use of private class ${name} by ${caller_module_name}")
  }

  if $echoes_alert_sms_router::https {
    require openssl
  }

  file { $echoes_alert_sms_router::install_dir:
    ensure => 'directory',
    owner  => 0,
    group  => 0,
    mode   => '0755'
  }->
  file { $echoes_alert_sms_router::binary_path:
    ensure => 'directory',
    owner  => 0,
    group  => 0,
    mode   => '0755'
  }->
  file { "${echoes_alert_sms_router::binary_path}/${echoes_alert_sms_router::service_name}":
    ensure => 'file',
    owner  => 0,
    group  => 0,
    mode   => '0755',
    source => "puppet:///modules/${module_name}/${echoes_alert_sms_router::branch}/${echoes_alert_sms_router::version}/sms_router",
  }->
  class { 'echoes_alert_probe':
    addons          => $echoes_alert_sms_router::probe_addons,
    sms_router_install_dir => $echoes_alert_sms_router::install_dir,
    branch          => $echoes_alert_sms_router::probe_branch,
    version         => $echoes_alert_sms_router::probe_version,
  }

}
