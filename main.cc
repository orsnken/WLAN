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
  WLan::Domain::Init();
  WLan::Simulation().Run();
}

void WLan::Simulation::Run() {
  std::cout << "Run Now!!" << std::endl;
  WLan::Domain network1("Network 1", 1, "192.168.1.0", "255.255.255.0", 3);
  WLan::Domain network2("Network 2", 1, "192.168.2.0", "255.255.255.0", 3);

  std::vector<Vector3D> sp1;
  sp1.push_back(Vector3D( 0, 0, 0.5));
  sp1.push_back(Vector3D(-0.5,0, 0));
  sp1.push_back(Vector3D( 0, 0, -0.5));
  network1.ConfigureMobility(Vector3D( 0, 0, 0), sp1);
  
  std::vector<Vector3D> sp2;
  sp2.push_back(Vector3D( 0, 0, 0.5));
  sp2.push_back(Vector3D( 0.5, 0, 0));
  sp2.push_back(Vector3D( 0, 0, -0.5));
  network2.ConfigureMobility(Vector3D(5, 0, 0), sp2);

  SetUdpOnOffApplication(network1.GetStaNode(0), network1. GetApNode(0), kEdcaStaAc, kEdcaApAc , 1005, 0.1, 10.0);
  SetUdpOnOffApplication(network1. GetApNode(0), network1.GetStaNode(0), kEdcaApAc , kEdcaStaAc, 1006, 0.1, 10.0);

  SetUdpOnOffApplication(network1.GetStaNode(1), network1. GetApNode(0), kEdcaStaAc, kEdcaApAc , 1007, 0.1, 10.0);
  SetUdpOnOffApplication(network1. GetApNode(0), network1.GetStaNode(1), kEdcaApAc , kEdcaStaAc, 1008, 0.1, 10.0);


  SetUdpOnOffApplication(network1.GetStaNode(2), network1. GetApNode(0), kEdcaStaAc, kEdcaApAc , 1009, 0.1, 10.0);
  SetUdpOnOffApplication(network1. GetApNode(0), network1.GetStaNode(2), kEdcaApAc , kEdcaStaAc, 1010, 0.1, 10.0);

  SetUdpOnOffApplication(network2.GetStaNode(0), network2. GetApNode(0), kEdcaStaAc, kEdcaApAc , 5005, 0.1, 10.0);
  SetUdpOnOffApplication(network2. GetApNode(0), network2.GetStaNode(0), kEdcaApAc , kEdcaStaAc, 5006, 0.1, 10.0);

  SetUdpOnOffApplication(network2.GetStaNode(1), network2. GetApNode(0), kEdcaStaAc, kEdcaApAc , 5007, 0.1, 10.0);
  SetUdpOnOffApplication(network2. GetApNode(0), network2.GetStaNode(1), kEdcaApAc , kEdcaStaAc, 5008, 0.1, 10.0);

  SetUdpOnOffApplication(network2.GetStaNode(2), network2. GetApNode(0), kEdcaStaAc, kEdcaApAc , 5009, 0.1, 10.0);
  SetUdpOnOffApplication(network2. GetApNode(0), network2.GetStaNode(2), kEdcaApAc , kEdcaStaAc, 5010, 0.1, 10.0);

  FlowMonitorHelper flowMonitor;
  Ptr<FlowMonitor> fm = flowMonitor.InstallAll();

  Simulator::Stop(Seconds(3));
  Simulator::Run();
  Simulator::Destroy();

  fm->CheckForLostPackets ();
  Ptr<Ipv4FlowClassifier> classifier = DynamicCast<Ipv4FlowClassifier>(flowMonitor.GetClassifier());
  std::map<FlowId, FlowMonitor::FlowStats> stats = fm->GetFlowStats();
  std::cout << "--------------------------------------" << std::endl;
  for (std::map<FlowId, FlowMonitor::FlowStats>::const_iterator iter = stats.begin(); iter != stats.end(); iter++) {
    Ipv4FlowClassifier::FiveTuple t = classifier->FindFlow (iter->first);
    NS_LOG_UNCOND("Flow ID: " << iter->first << " src addr " << t.sourceAddress << " dest addr " << t.destinationAddress);
    NS_LOG_UNCOND("" << iter->second.rxBytes * 8.0 / (iter->second.timeLastRxPacket.GetSeconds() - iter->second.timeFirstTxPacket.GetSeconds()) / 1024 << ",");
  }
}
