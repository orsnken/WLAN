#ifndef WLAN_SIMULATION_H__
#define WLAN_SIMULATION_H__

#include "ns3.h"

namespace WLan {

struct EdcaAc;

class Simulation {
public:
  Simulation();
  void Run();
private:
  void SetTcpOnOffApplication(
    ns3::Ptr<ns3::Node> src,
    ns3::Ptr<ns3::Node> dest,
    WLan::EdcaAc srcAc,
    WLan::EdcaAc destAc,
    int    portRemote,
    double timeStartSec,
    double timeStopSec
  );
};

} // namespace WLan

#endif  // WLAN_SIMULATION_H__
