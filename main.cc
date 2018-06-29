#include "ns3.h"

#include <iostream>

#include "ConstantParam.h"
#include "Domain.h"
#include "Simulation.h"

using namespace ns3;

int main(int argc, char *argv[]) {
  CommandLine cmd;
  cmd.Parse(argc, argv);
  
  Config::SetDefault("ns3::WifiRemoteStationManager::RtsCtsThreshold", UintegerValue(100000));

  WLan::Simulation().Run();
}

void WLan::Simulation::TraceThroughput(Ptr<Application> app, uint32_t& oldTotalBytes, int n) {
  uint32_t newTotalBytes;

  Ptr <PacketSink> pktSink = DynamicCast <PacketSink> (app);
  newTotalBytes = pktSink->GetTotalRx ();
  double th = (newTotalBytes - oldTotalBytes)*8.0/1/1024;

  std::cout << "[" << n << "]" << Simulator::Now ().GetSeconds () << " \t " << th << std::endl;
  // throughput = (throughput+th)/2.0;
  oldTotalBytes = newTotalBytes;
  Simulator::Schedule (Seconds(1), &Simulation::TraceThroughput, this, app, oldTotalBytes, n);
}

void WLan::Simulation::Run() {
  std::cout << "Run Now!!" << std::endl;
  WLan::Domain network1("Network 1", 1, "192.168.1.0", "255.255.255.0", 3);
  WLan::Domain network2("Network 2", 1, "192.168.2.0", "255.255.255.0", 3);

  std::vector<Vector3D> sp1;
  sp1.push_back(Vector3D( 0, 0, 0.5));
  sp1.push_back(Vector3D(-0.5,0, 0));
  sp1.push_back(Vector3D( 0, 0, -0.5));
  network1.ConfigureMobility(Vector3D(20, 0, 0), sp1);
  
  std::vector<Vector3D> sp2;
  sp2.push_back(Vector3D( 0, 0, 0.5));
  sp2.push_back(Vector3D( 0.5, 0, 0));
  sp2.push_back(Vector3D( 0, 0, -0.5));
  network2.ConfigureMobility(Vector3D(20, 0, 0), sp2);

  uint32_t tb10 = 0;
  ApplicationContainer a10 = SetUdpOnOffApplication(network1.GetStaNode(0), network1.GetApNode(0), kEdcaStaAc, kEdcaApAc, 1005, 0.1, 10.0);
  Simulator::Schedule(Seconds(1), &Simulation::TraceThroughput, this, a10.Get(1), tb10, 10);
 
  uint32_t tb11 = 0;
  ApplicationContainer a11 = SetUdpOnOffApplication(network1.GetApNode(0), network1.GetStaNode(0), kEdcaApAc, kEdcaStaAc, 1006, 0.1, 10.0);
  Simulator::Schedule(Seconds(1), &Simulation::TraceThroughput, this, a11.Get(1), tb11, 11);

  uint32_t tb20 = 0;
  ApplicationContainer a20 = SetUdpOnOffApplication(network1.GetStaNode(1), network1.GetApNode(0), kEdcaStaAc, kEdcaApAc, 1007, 0.1, 10.0);
  Simulator::Schedule(Seconds(1), &Simulation::TraceThroughput, this, a20.Get(1), tb20, 20);
 
  uint32_t tb21 = 0;
  ApplicationContainer a21 = SetUdpOnOffApplication(network1.GetApNode(0), network1.GetStaNode(1), kEdcaApAc, kEdcaStaAc, 1008, 0.1, 10.0);
  Simulator::Schedule(Seconds(1), &Simulation::TraceThroughput, this, a21.Get(1), tb21, 21);

  uint32_t tb50 = 0;
  ApplicationContainer a50 = SetUdpOnOffApplication(network2.GetStaNode(0), network2.GetApNode(0), kEdcaStaAc, kEdcaApAc, 5005, 0.1, 10.0);
  Simulator::Schedule(Seconds(1), &Simulation::TraceThroughput, this, a50.Get(1), tb50, 50);

  uint32_t tb51 = 0;
  ApplicationContainer a51 = SetUdpOnOffApplication(network2.GetApNode(0), network2.GetStaNode(0), kEdcaApAc, kEdcaStaAc, 5006, 0.1, 10.0);
  Simulator::Schedule(Seconds(1), &Simulation::TraceThroughput, this, a51.Get(1), tb51, 51);


  uint32_t tb60 = 0;
  ApplicationContainer a60 = SetUdpOnOffApplication(network2.GetStaNode(1), network2.GetApNode(0), kEdcaStaAc, kEdcaApAc, 5007, 0.1, 10.0);
  Simulator::Schedule(Seconds(1), &Simulation::TraceThroughput, this, a60.Get(1), tb60, 60);

  uint32_t tb61 = 0;
  ApplicationContainer a61 = SetUdpOnOffApplication(network2.GetApNode(0), network2.GetStaNode(1), kEdcaApAc, kEdcaStaAc, 5008, 0.1, 10.0);
  Simulator::Schedule(Seconds(1), &Simulation::TraceThroughput, this, a61.Get(1), tb61, 61);

  // std::string animFile = "anim.xml";
  // AnimationInterface anim(animFile);
  // anim.EnablePacketMetadata();
  // anim.EnableIpv4L3ProtocolCounters (Seconds(0), Seconds(1));

  Simulator::Stop(Seconds(10));
  Simulator::Run();
  Simulator::Destroy();
}
