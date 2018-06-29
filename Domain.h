#ifndef WLAN_DOMAIN_H__
#define WLAN_DOMAIN_H__

#include "ns3.h"

#include <string>

namespace WLan {

class Domain {
public:
  Domain(std::string ssid, int ch, int numOfNodes);
private:
  int ch_;
  ns3::Ssid ssid_;
  ns3::NodeContainer apNodes_;
  ns3::NodeContainer staNodes_;

  void ConfigureDataLinkLayer();
};

} // namespace WLan

#endif  // WLAN_DOMAIN_H__
