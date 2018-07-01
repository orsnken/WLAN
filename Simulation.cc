#include <iostream>

#include "ConstantParam.h"
#include "Simulation.h"

using namespace ns3;

namespace WLan {

Simulation::Simulation() {
  ;
}

ns3::ApplicationContainer Simulation::SetOnOffApplication(
  ns3::Ptr<ns3::Node> src,
  ns3::Ptr<ns3::Node> dest,
  WLan::EdcaAc srcAc,
  WLan::EdcaAc destAc,
  int    portRemote,
  double timeStartSec,
  double timeStopSec,
  std::string transportProtocol
) {
  Ipv4InterfaceAddress destAddr = dest->GetObject<Ipv4>()->GetAddress(1, 0);
  InetSocketAddress remoteSockAddr(destAddr.GetLocal(), portRemote);
  remoteSockAddr.SetTos(srcAc.id);

  OnOffHelper ftp(transportProtocol, Address());
  ftp.SetAttribute("OnTime", StringValue("ns3::ConstantRandomVariable[Constant=1]"));
  ftp.SetAttribute("OffTime", StringValue("ns3::ConstantRandomVariable[Constant=0]"));
  ftp.SetAttribute("DataRate", StringValue(kAppDataRate));
  ftp.SetAttribute("PacketSize", UintegerValue(kAppPacketSizeByte));
  ftp.SetAttribute("Remote", AddressValue(remoteSockAddr));
  ApplicationContainer app = ftp.Install(src);

  InetSocketAddress remoteSockAddrT(destAddr.GetLocal(), portRemote);
  remoteSockAddrT.SetTos(destAc.id);
  PacketSinkHelper sinkHelper(transportProtocol, Address(remoteSockAddrT));
  app.Add(sinkHelper.Install(dest));

  app.Start(Seconds(timeStartSec));
  app.Stop(Seconds(timeStopSec));

  return app;
}

ns3::ApplicationContainer Simulation::SetTcpOnOffApplication(
  ns3::Ptr<ns3::Node> src,
  ns3::Ptr<ns3::Node> dest,
  WLan::EdcaAc srcAc,
  WLan::EdcaAc destAc,
  int    portRemote,
  double timeStartSec,
  double timeStopSec
) {
  return SetOnOffApplication(src, dest, srcAc, destAc, portRemote, timeStartSec, timeStopSec, "ns3::TcpSocketFactory");
}

ns3::ApplicationContainer Simulation::SetUdpOnOffApplication(
  ns3::Ptr<ns3::Node> src,
  ns3::Ptr<ns3::Node> dest,
  WLan::EdcaAc srcAc,
  WLan::EdcaAc destAc,
  int    portRemote,
  double timeStartSec,
  double timeStopSec
) {
  return SetOnOffApplication(src, dest, srcAc, destAc, portRemote, timeStartSec, timeStopSec, "ns3::UdpSocketFactory");
}

} // namespace WLan
