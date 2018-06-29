#ifndef WLAN_DOMAIN_H__
#define WLAN_DOMAIN_H__

#include "ns3.h"

#include <string>

namespace WLan {

class Domain {
public:
  Domain(
    std::string ssid,
    int ch,
    std::string networkAddress,
    std::string subnetmast,
    int numOfNodes
  );
private:
  int ch_;
  std::string naddr_;
  std::string smask_;
  ns3::Ssid ssid_;
  ns3::NodeContainer apNodes_;
  ns3::NodeContainer staNodes_;
  ns3::NetDeviceContainer apDevs_;
  ns3::NetDeviceContainer staDevs_;

  void ConfigureDataLinkLayer();
  void ConfigureNetworkLayer();
};

} // namespace WLan

#endif  // WLAN_DOMAIN_H__
