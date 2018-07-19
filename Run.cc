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
#include "SimulationSettings.h"

using namespace ns3;

namespace {

const std::string kFilenameDir     = "scratch/WLAN/dat";
const std::string kFilenamePrefix  = "";
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
    ofs << throughputKbps << " ";
  }
  NS_LOG_UNCOND("AVR.: " << avr);
  ofs.close();
}

std::string filename() {
  using namespace WLan;
  std::string filename = kFilenameDir;
  filename += "/";
  filename += kFilenamePrefix;
  filename += Global::trafficType;
  filename += "_";
  filename += "D";
  std::ostringstream sout;
  sout << std::setfill('0') << std::setw(3) << static_cast<int>(Global::distanceWLans);
  filename += sout.str();
  filename += "_A-";
  filename += std::to_string(Global::apCwMin);
  filename += "-";
  filename += std::to_string(Global::apCwMax);
  filename += "-";
  filename += std::to_string(Global::apAifsn);
  filename += "_S-";
  filename += std::to_string(Global::staCwMin);
  filename += "-";
  filename += std::to_string(Global::staCwMax);
  filename += "-";
  filename += std::to_string(Global::staAifsn);
  filename += kFilenamePostfix;
  return filename;
}

} // namespace

void WLan::Simulation::Run() {
  std::cout << "Run Now!![" << Global::trafficType << ", " << Global::distanceWLans << "m]" << std::endl;
  std::cout << "AP : (CWmin, CWmax, AIFSN) > (" << Global::apCwMin << ", " << Global::apCwMax << ", " << Global::apAifsn << ")" << std::endl;
  std::cout << "STA: (CWmin, CWmax, AIFSN) > (" << Global::staCwMin << ", " << Global::staCwMax << ", " << Global::staAifsn << ")" << std::endl;

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
  network2.ConfigureMobility(Vector3D(Global::distanceWLans, 0, 0), sp2);

  if (Global::trafficType == "UDP") {
    SetUdpOnOffApplication(network1.GetStaNode(0), network1. GetApNode(0), kEdcaStaAc, kEdcaApAc , 1005, kApplicationBeginSec, kApplicationEndSec);
    SetUdpOnOffApplication(network1. GetApNode(0), network1.GetStaNode(0), kEdcaApAc , kEdcaStaAc, 1006, kApplicationBeginSec, kApplicationEndSec);

    SetUdpOnOffApplication(network1.GetStaNode(1), network1. GetApNode(0), kEdcaStaAc, kEdcaApAc , 1007, kApplicationBeginSec, kApplicationEndSec);
    SetUdpOnOffApplication(network1. GetApNode(0), network1.GetStaNode(1), kEdcaApAc , kEdcaStaAc, 1008, kApplicationBeginSec, kApplicationEndSec);

    SetUdpOnOffApplication(network1.GetStaNode(2), network1. GetApNode(0), kEdcaStaAc, kEdcaApAc , 1009, kApplicationBeginSec, kApplicationEndSec);
    SetUdpOnOffApplication(network1. GetApNode(0), network1.GetStaNode(2), kEdcaApAc , kEdcaStaAc, 1010, kApplicationBeginSec, kApplicationEndSec);

    SetUdpOnOffApplication(network2.GetStaNode(0), network2. GetApNode(0), kEdcaStaAc, kEdcaApAc , 5005, kApplicationBeginSec, kApplicationEndSec);
    SetUdpOnOffApplication(network2. GetApNode(0), network2.GetStaNode(0), kEdcaApAc , kEdcaStaAc, 5006, kApplicationBeginSec, kApplicationEndSec);

    SetUdpOnOffApplication(network2.GetStaNode(1), network2. GetApNode(0), kEdcaStaAc, kEdcaApAc , 5007, kApplicationBeginSec, kApplicationEndSec);
    SetUdpOnOffApplication(network2. GetApNode(0), network2.GetStaNode(1), kEdcaApAc , kEdcaStaAc, 5008, kApplicationBeginSec, kApplicationEndSec);

    SetUdpOnOffApplication(network2.GetStaNode(2), network2. GetApNode(0), kEdcaStaAc, kEdcaApAc , 5009, kApplicationBeginSec, kApplicationEndSec);
    SetUdpOnOffApplication(network2. GetApNode(0), network2.GetStaNode(2), kEdcaApAc , kEdcaStaAc, 5010, kApplicationBeginSec, kApplicationEndSec);
  } else if (Global::trafficType == "TCP") {
    SetTcpOnOffApplication(network1.GetStaNode(0), network1. GetApNode(0), kEdcaStaAc, kEdcaApAc , 1005, kApplicationBeginSec, kApplicationEndSec);
    SetTcpOnOffApplication(network1. GetApNode(0), network1.GetStaNode(0), kEdcaApAc , kEdcaStaAc, 1006, kApplicationBeginSec, kApplicationEndSec);

    SetTcpOnOffApplication(network1.GetStaNode(1), network1. GetApNode(0), kEdcaStaAc, kEdcaApAc , 1007, kApplicationBeginSec, kApplicationEndSec);
    SetTcpOnOffApplication(network1. GetApNode(0), network1.GetStaNode(1), kEdcaApAc , kEdcaStaAc, 1008, kApplicationBeginSec, kApplicationEndSec);

    SetTcpOnOffApplication(network1.GetStaNode(2), network1. GetApNode(0), kEdcaStaAc, kEdcaApAc , 1009, kApplicationBeginSec, kApplicationEndSec);
    SetTcpOnOffApplication(network1. GetApNode(0), network1.GetStaNode(2), kEdcaApAc , kEdcaStaAc, 1010, kApplicationBeginSec, kApplicationEndSec);

    SetTcpOnOffApplication(network2.GetStaNode(0), network2. GetApNode(0), kEdcaStaAc, kEdcaApAc , 5005, kApplicationBeginSec, kApplicationEndSec);
    SetTcpOnOffApplication(network2. GetApNode(0), network2.GetStaNode(0), kEdcaApAc , kEdcaStaAc, 5006, kApplicationBeginSec, kApplicationEndSec);

    SetTcpOnOffApplication(network2.GetStaNode(1), network2. GetApNode(0), kEdcaStaAc, kEdcaApAc , 5007, kApplicationBeginSec, kApplicationEndSec);
    SetTcpOnOffApplication(network2. GetApNode(0), network2.GetStaNode(1), kEdcaApAc , kEdcaStaAc, 5008, kApplicationBeginSec, kApplicationEndSec);

    SetTcpOnOffApplication(network2.GetStaNode(2), network2. GetApNode(0), kEdcaStaAc, kEdcaApAc , 5009, kApplicationBeginSec, kApplicationEndSec);
    SetTcpOnOffApplication(network2. GetApNode(0), network2.GetStaNode(2), kEdcaApAc , kEdcaStaAc, 5010, kApplicationBeginSec, kApplicationEndSec);
  } else {
    std::cout << "Unknown traffic type [" << Global::trafficType << "]" << std::endl;
    return;
  }

  FlowMonitorHelper flowMonitor;
  Ptr<FlowMonitor> fm = flowMonitor.InstallAll();

  Simulator::Stop(Seconds(kSimulationTimeSec));
  Simulator::Run();
  Simulator::Destroy();

  output(flowMonitor, fm, filename());

}

