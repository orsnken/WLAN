#include "ns3.h"

#include <fstream>
#include <iomanip>
#include <iostream>
#include <random>
#include <sstream>
#include <string>

#include "ConstantParam.h"
#include "Domain.h"
#include "Simulation.h"

using namespace ns3;

namespace {

const std::string kFilenameDir     = "scratch/WLAN/dat";
const std::string kFilenamePrefix  = "SUG_";
const std::string kFilenamePostfix = ".dat";

} // namesapce 

namespace {

void output(FlowMonitorHelper& flowMonitor, Ptr<FlowMonitor> fm, const std::string& filename) {
  std::ofstream ofs(filename);
  fm->CheckForLostPackets ();
  Ptr<Ipv4FlowClassifier> classifier = DynamicCast<Ipv4FlowClassifier>(flowMonitor.GetClassifier());
  std::map<FlowId, FlowMonitor::FlowStats> stats = fm->GetFlowStats();
  std::cout << "--------------------------------------" << std::endl;
  double avr = 0.0;
  for (std::map<FlowId, FlowMonitor::FlowStats>::const_iterator iter = stats.begin(); iter != stats.end(); iter++) {
    Ipv4FlowClassifier::FiveTuple t = classifier->FindFlow (iter->first);
    double throughputKbps = iter->second.rxBytes * 8.0 / (iter->second.timeLastRxPacket.GetSeconds() - iter->second.timeFirstTxPacket.GetSeconds()) / 1024;
    NS_LOG_UNCOND("Flow ID: " << iter->first << " src addr " << t.sourceAddress << " dest addr " << t.destinationAddress);
    NS_LOG_UNCOND("" << throughputKbps << ",");
    avr +=  throughputKbps;
    ofs << throughputKbps << std::endl;
  }
  NS_LOG_UNCOND("AVR.: " << avr);
  ofs.close();
}

std::string filename() {
  using namespace WLan;
  std::string filename = kFilenameDir;
  filename += "/";
  filename += kFilenamePrefix;
  filename += "D";
  std::ostringstream sout;
  sout << std::setfill('0') << std::setw(3) << static_cast<int>(kDistanceWLans);
  filename += sout.str();
  filename += "_ACMIN";
  filename += std::to_string(kEdcaApCwMin);
  filename += "_ACMAX";
  filename += std::to_string(kEdcaApCwMax);
  filename += "_AAIFS";
  filename += std::to_string(kEdcaApAifsn);
  filename += "_SCMIN";
  filename += std::to_string(kEdcaStaCwMin);
  filename += "_SCMAX";
  filename += std::to_string(kEdcaStaCwMax);
  filename += "_SAIFS";
  filename += std::to_string(kEdcaStaAifsn);
  filename += kFilenamePostfix;
  return filename;
}

} // namespace

void WLan::Simulation::Run() {
  std::cout << "Run Now!!" << std::endl;
  WLan::Domain network1("Network 1", 1, "192.168.1.0", "255.255.255.0", 3);
  WLan::Domain network2("Network 2", 1, "192.168.2.0", "255.255.255.0", 3);

  std::vector<Vector3D> sp1;
  sp1.push_back(Vector3D( 0.0, 0.0, kDistanceApSta));
  sp1.push_back(Vector3D(-kDistanceApSta, 0.0, 0.0));
  sp1.push_back(Vector3D( 0.0, 0.0,-kDistanceApSta));
  network1.ConfigureMobility(Vector3D( 0, 0, 0), sp1);
  
  std::vector<Vector3D> sp2;
  sp2.push_back(Vector3D( 0.0, 0.0, kDistanceApSta));
  sp2.push_back(Vector3D(-kDistanceApSta, 0.0, 0.0));
  sp2.push_back(Vector3D( 0.0, 0.0,-kDistanceApSta));
  network2.ConfigureMobility(Vector3D(kDistanceWLans, 0, 0), sp2);

  SetUdpOnOffApplication(network1.GetStaNode(0), network1. GetApNode(0), kEdcaStaAc, kEdcaApAc , 1005, kApplicationBeginSec, kApplicationEndSec);
  SetUdpOnOffApplication(network1. GetApNode(0), network1.GetStaNode(0), kEdcaApAc , kEdcaStaAc, 1006, kApplicationBeginSec, kApplicationEndSec);

  // SetUdpOnOffApplication(network1.GetStaNode(1), network1. GetApNode(0), kEdcaStaAc, kEdcaApAc , 1007, 0.1, 10.0);
  // SetUdpOnOffApplication(network1. GetApNode(0), network1.GetStaNode(1), kEdcaApAc , kEdcaStaAc, 1008, 0.1, 10.0);

  // SetUdpOnOffApplication(network1.GetStaNode(2), network1. GetApNode(0), kEdcaStaAc, kEdcaApAc , 1009, 0.1, 10.0);
  // SetUdpOnOffApplication(network1. GetApNode(0), network1.GetStaNode(2), kEdcaApAc , kEdcaStaAc, 1010, 0.1, 10.0);

  SetUdpOnOffApplication(network2.GetStaNode(0), network2. GetApNode(0), kEdcaStaAc, kEdcaApAc , 5005, kApplicationBeginSec, kApplicationEndSec);
  SetUdpOnOffApplication(network2. GetApNode(0), network2.GetStaNode(0), kEdcaApAc , kEdcaStaAc, 5006, kApplicationBeginSec, kApplicationEndSec);

  // SetUdpOnOffApplication(network2.GetStaNode(1), network2. GetApNode(0), kEdcaStaAc, kEdcaApAc , 5007, 0.1, 10.0);
  // SetUdpOnOffApplication(network2. GetApNode(0), network2.GetStaNode(1), kEdcaApAc , kEdcaStaAc, 5008, 0.1, 10.0);

  // SetUdpOnOffApplication(network2.GetStaNode(2), network2. GetApNode(0), kEdcaStaAc, kEdcaApAc , 5009, 0.1, 10.0);
  // SetUdpOnOffApplication(network2. GetApNode(0), network2.GetStaNode(2), kEdcaApAc , kEdcaStaAc, 5010, 0.1, 10.0);

  FlowMonitorHelper flowMonitor;
  Ptr<FlowMonitor> fm = flowMonitor.InstallAll();

  Simulator::Stop(Seconds(kSimulationTimeSec));
  Simulator::Run();
  Simulator::Destroy();

  output(flowMonitor, fm, filename());

}

