#ifndef WLAN_SIMULATION_H__
#define WLAN_SIMULATION_H__

#include "ns3.h"

namespace WLan {

struct EdcaAc;

class Simulation {
public:
  Simulation();
  void Run();
  void TraceThroughput(
    ns3::Ptr<ns3::Application> app,
    uint32_t& oldTotalBytes,
    int n
  );
private:
  ns3::ApplicationContainer SetTcpOnOffApplication(
    ns3::Ptr<ns3::Node> src,
    ns3::Ptr<ns3::Node> dest,
    WLan::EdcaAc srcAc,
    WLan::EdcaAc destAc,
    int    portRemote,
    double timeStartSec,
    double timeStopSec
  );

  ns3::ApplicationContainer SetUdpOnOffApplication(
    ns3::Ptr<ns3::Node> src,
    ns3::Ptr<ns3::Node> dest,
    WLan::EdcaAc srcAc,
    WLan::EdcaAc destAc,
    int    portRemote,
    double timeStartSec,
    double timeStopSec
  );

  ns3::ApplicationContainer SetOnOffApplication(
    ns3::Ptr<ns3::Node> src,
    ns3::Ptr<ns3::Node> dest,
    WLan::EdcaAc srcAc,
    WLan::EdcaAc destAc,
    int    portRemote,
    double timeStartSec,
    double timeStopSec,
    std::string transportProtocol
  );
};

} // namespace WLan

#endif  // WLAN_SIMULATION_H__
