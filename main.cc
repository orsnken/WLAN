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
  cmd.AddValue ("distanceWLANs", "The distance in meters between WLANs (Default:5.0)", WLan::Global::distanceWLans);
  cmd.Parse(argc, argv);
  std::random_device rnd;
  std::mt19937 mt(rnd());
  RngSeedManager::SetSeed(mt() + 1);
  RngSeedManager::SetRun(mt() + 1);
  Config::SetDefault("ns3::WifiRemoteStationManager::RtsCtsThreshold", UintegerValue(100000));
  WLan::Domain::Init();
  WLan::Simulation().Run();
}
