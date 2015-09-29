class echoes_alert_sms_router (
  $branch          = $echoes_alert_sms_router::params::branch,
  $cert_file       = $echoes_alert_sms_router::params::cert_file,
  $install_dir     = $echoes_alert_sms_router::params::install_dir,
  $http_port       = $echoes_alert_sms_router::params::http_port,
  $https           = $echoes_alert_sms_router::params::https,
  $https_port      = $echoes_alert_sms_router::params::https_port,
  $manage_firewall = $echoes_alert_sms_router::params::manage_firewall,
  $priv_key_file   = $echoes_alert_sms_router::params::priv_key_file,
  $probe_addons    = $echoes_alert_sms_router::params::probe_addons,
  $probe_branch    = $echoes_alert_sms_router::params::probe_branch,
  $probe_version   = $echoes_alert_sms_router::params::probe_version,
  $servername      = $echoes_alert_sms_router::params::servername,
  $service_enable  = $echoes_alert_sms_router::params::service_enable,
  $service_ensure  = $echoes_alert_sms_router::params::service_ensure,
  $service_name    = $echoes_alert_sms_router::params::service_name,
  $version         = $echoes_alert_sms_router::params::version,
) inherits echoes_alert_sms_router::params {
  validate_string($branch)
  validate_absolute_path($install_dir)
  validate_integer($http_port, 65535, 1)
  validate_bool($https)
  if $https {
    validate_absolute_path($cert_file)
    validate_integer($https_port, 65535, 1)
    validate_absolute_path($priv_key_file)
  }
  validate_bool($manage_firewall)
  validate_hash($probe_addons)
  validate_string($probe_branch)
  validate_string($probe_version)
  validate_bool($service_enable)
  validate_string($service_ensure)
  validate_string($service_name)
  validate_string($servername)
  validate_string($version)

  anchor { "${module_name}::begin": } ->
  class { "${module_name}::install": } ->
  class { "${module_name}::service": } ->
  class { "${module_name}::firewall": } ->
  anchor { "${module_name}::end": }
}
