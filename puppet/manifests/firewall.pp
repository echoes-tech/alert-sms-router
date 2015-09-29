# Private class
class echoes_alert_sms_router::firewall inherits echoes_alert_sms_router {
  if $caller_module_name != $module_name {
    fail("Use of private class ${name} by ${caller_module_name}")
  }

  if $echoes_alert_sms_router::manage_firewall {
    if defined('::firewall') {
      if $echoes_alert_sms_router::https {
        firewall { '101 allow sms router HTTPs access':
          port  => [ $echoes_alert_sms_router::https_port ],
          proto => 'tcp',
          jump  => 'allowed',
        }->
        firewall { '101 allow sms router HTTPs access - IPv6':
          port     => [ $echoes_alert_sms_router::https_port ],
          proto    => 'tcp',
          jump     => 'allowed',
          provider => 'ip6tables',
        }
      }

      firewall { '100 allow sms router HTTP access':
        port  => [ $echoes_alert_sms_router::http_port ],
        proto => 'tcp',
        jump  => 'allowed',
      }->
      firewall { '100 allow sms router HTTP access - IPv6':
        port     => [ $echoes_alert_sms_router::http_port ],
        proto    => 'tcp',
        jump     => 'allowed',
        provider => 'ip6tables',
      }
    }
  }
}
