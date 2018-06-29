#ifndef WLAN_DOMAIN_H__
#define WLAN_DOMAIN_H__

#include "ns3.h"

#include <string>
#include <vector>

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
  void ConfigureMobility(
    ns3::Vector3D base,
    std::vector<ns3::Vector3D> relativePositionStas
  );
  ns3::Ptr<ns3::Node> GetApNode(int index) { return apNodes_.Get(index); }
  ns3::Ptr<ns3::Node> GetStaNode(int index) { return staNodes_.Get(index); }
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
