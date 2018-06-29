#include <iostream>

#include "ConstantParam.h"
#include "Simulation.h"

using namespace ns3;

namespace WLan {

Simulation::Simulation() {
  ;
}

void Simulation::Run() {
  std::cout << "Run Now!!" << std::endl;
}

void Simulation::SetTcpOnOffApplication(
  ns3::Ptr<ns3::Node> src,
  ns3::Ptr<ns3::Node> dest,
  WLan::EdcaAc srcAc,
  WLan::EdcaAc destAc,
  int    portRemote,
  double timeStartSec,
  double timeStopSec
) {
  Ipv4InterfaceAddress destAddr = dest->GetObject<Ipv4>()->GetAddress(1, 0);
  InetSocketAddress remoteSockAddr(destAddr.GetLocal(), portRemote);
  remoteSockAddr.SetTos(srcAc.id);

  OnOffHelper ftp("ns3::TcpSocketFactory", Address());
  ftp.SetAttribute("OnTime", StringValue("ns3::ConstantRandomVariable[Constant=1]"));
  ftp.SetAttribute("OffTime", StringValue("ns3::ConstantRandomVariable[Constant=0]"));
  ftp.SetAttribute("DataRate", StringValue(kAppDataRate));
  ftp.SetAttribute("PacketSize", UintegerValue(1472));
  ftp.SetAttribute("Remote", AddressValue(remoteSockAddr));
  ApplicationContainer app = ftp.Install(src);
  app.Start(Seconds(timeStartSec));
  app.Stop(Seconds(timeStopSec));

  InetSocketAddress remoteSockAddrT(destAddr.GetLocal(), portRemote);
  remoteSockAddrT.SetTos(destAc.id);
  PacketSinkHelper sinkHelper("ns3::TcpSocketFactory", Address(remoteSockAddrT));
  ApplicationContainer sink = sinkHelper.Install(dest);
  sink.Start(Seconds(timeStartSec));
  sink.Stop(Seconds(timeStopSec));
}

} // namespace WLan
