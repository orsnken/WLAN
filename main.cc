#include "ns3.h"

#include <fstream>
#include <iostream>
#include <random>

#include "ConstantParam.h"
#include "Domain.h"
#include "Simulation.h"
#include "SimulationSettings.h"

using namespace ns3;

int main(int argc, char *argv[]) {
  CommandLine cmd;
  cmd.AddValue("distanceWLANs", "The distance in meters between WLANs (Default:5.0)", WLan::Global::distanceWLans);
  cmd.AddValue("apCwMin", "The CWmin for AP (Default:7)", WLan::Global::apCwMin);
  cmd.AddValue("apCwMax", "The CWmax for AP (Default:31)", WLan::Global::apCwMax);
  cmd.AddValue("apAifsn", "The AIFSN for AP (Default:2)", WLan::Global::apAifsn);
  cmd.AddValue("staCwMin", "The CWmin for STAs (Default:7)", WLan::Global::staCwMin);
  cmd.AddValue("staCwMax", "The CWmax for STAs (Default:31)", WLan::Global::staCwMax);
  cmd.AddValue("staAifsn", "The AIFSN for STAs (Default:2)", WLan::Global::staAifsn);
  cmd.AddValue("trafficType", "Transmittion protocol for communication, choose UDP or TCP (Default:UDP)", WLan::Global::trafficType);
  cmd.Parse(argc, argv);
  std::random_device rnd;
  std::mt19937 mt(rnd());
  RngSeedManager::SetSeed(mt() + 1);
  RngSeedManager::SetRun(mt() + 1);
  Config::SetDefault("ns3::WifiRemoteStationManager::RtsCtsThreshold", UintegerValue(100000));
  WLan::Domain::Init();
  WLan::Simulation().Run();
}
