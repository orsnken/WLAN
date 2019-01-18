#include "ns3.h"

#include "ConstantParam.h"
#include "Domain.h"
#include "SimulationSettings.h"

namespace WLan {

using namespace ns3;

// ----------------------------------------------------------------
// static
// ----------------------------------------------------------------
ns3::WifiHelper Domain::sWifi;
ns3::YansWifiPhyHelper Domain::sPhy;
ns3::YansWifiChannelHelper Domain::sChannel;

void Domain::Init() {
  sWifi.SetRemoteStationManager(
    "ns3::ConstantRateWifiManager",
    "DataMode"   , StringValue("ErpOfdmRate54Mbps"),
    "ControlMode", StringValue("ErpOfdmRate54Mbps")
  );
  sWifi.SetStandard (WIFI_PHY_STANDARD_80211g);

  sChannel.SetPropagationDelay("ns3::ConstantSpeedPropagationDelayModel");
  sChannel.AddPropagationLoss(
    "ns3::LogDistancePropagationLossModel",
    "Exponent"         , DoubleValue(3.0), // 3.71
    "ReferenceDistance", DoubleValue(1.0),
    "ReferenceLoss"    , DoubleValue(40.045997)
  );
  sChannel.AddPropagationLoss(
    "ns3::NakagamiPropagationLossModel",
    "Distance1", DoubleValue(80),
    "Distance2", DoubleValue(200),
    "m0", DoubleValue(1.0),
    "m1", DoubleValue(1.0),
    "m2", DoubleValue(1.0)
  );

  sPhy = YansWifiPhyHelper::Default();
  sPhy.SetPcapDataLinkType(YansWifiPhyHelper::DLT_IEEE802_11_RADIO);
  sPhy.SetChannel(sChannel.Create());
  sPhy.Set("EnergyDetectionThreshold", DoubleValue(-82));
  sPhy.Set("CcaMode1Threshold"       , DoubleValue(-82));
  sPhy.Set("TxPowerEnd"              , DoubleValue(10));
  sPhy.Set("TxPowerStart"            , DoubleValue(10));
}

// ----------------------------------------------------------------



// ----------------------------------------------------------------
// public
// ----------------------------------------------------------------
Domain::Domain(
  std::string ssid,
  int ch,
  std::string naddr,
  std::string smask,
  std::string naddrApServer,
  std::string smaskApServer,
  int nnodes
) : ch_(ch), naddr_(naddr), smask_(smask),
    naddrApServer_(naddrApServer), smaskApServer_(smaskApServer) {
  ssid_ = Ssid(ssid);
  apNodes_.Create(1);
  staNodes_.Create(nnodes);
  serverNodes_.Create(nnodes);
  ConfigureDataLinkLayer();
  ConfigureNetworkLayer();
}

void Domain::ConfigureMobility(
  ns3::Vector3D base,
  std::vector<ns3::Vector3D> rpstas
) {
  Ptr<ListPositionAllocator> positionAp = CreateObject<ListPositionAllocator>();
  positionAp->Add(base);
  Ptr<ListPositionAllocator> positionStas = CreateObject<ListPositionAllocator>();
  for (Vector3D v: rpstas) {
    positionStas->Add(v + base);
  }
  MobilityHelper mobility;
  mobility.SetPositionAllocator(positionAp);
  mobility.Install(apNodes_);
  mobility.SetPositionAllocator(positionStas);
  mobility.Install(staNodes_);
}

// ----------------------------------------------------------------



// ----------------------------------------------------------------
// private
// ----------------------------------------------------------------
void Domain::ConfigureDataLinkLayer() {
  sPhy.Set("ChannelNumber", UintegerValue(ch_));

  WifiMacHelper macSta;
  macSta.SetType(
    "ns3::StaWifiMac",
    "ActiveProbing", BooleanValue(false),
    "QosSupported" , BooleanValue(true),
    "Ssid"         , SsidValue(ssid_)
  );
  staDevs_ = sWifi.Install(sPhy, macSta, staNodes_);
  WifiMacHelper macAp;
  macAp.SetType(
    "ns3::ApWifiMac",
    "Ssid", SsidValue(ssid_),
    "QosSupported" , BooleanValue(true)
  );
  apDevs_ = sWifi.Install(sPhy, macAp, apNodes_);

  Ptr<NetDevice> dev = apNodes_.Get(0)->GetDevice(0);
  Ptr<WifiNetDevice> wifiDev = DynamicCast<WifiNetDevice>(dev);
  Ptr<WifiMac> wifiMac = wifiDev->GetMac ();
  Ptr<EdcaTxopN> edca;
  PointerValue ptr;

  wifiMac->GetAttribute(kEdcaApAc.str, ptr);
  edca = ptr.Get<EdcaTxopN>();
  edca->SetMinCw(Global::apCwMin);
  edca->SetMaxCw(Global::apCwMax);
  edca->SetAifsn(Global::apAifsn);
  edca->SetTxopLimit(MilliSeconds(kEdcaApTxopMs));

  wifiMac->GetAttribute(kEdcaStaAc.str, ptr);
  edca = ptr.Get<EdcaTxopN>();
  edca->SetMinCw(Global::staCwMin);
  edca->SetMaxCw(Global::staCwMax);
  edca->SetAifsn(Global::staAifsn);
  edca->SetTxopLimit(MilliSeconds(kEdcaStaTxopMs));

  PointToPointHelper p2p;
  p2p.SetDeviceAttribute("DataRate", StringValue("54Mbps"));
  p2p.SetChannelAttribute("Delay", StringValue("5ms"));
  p2p.SetDeviceAttribute("Mtu", UintegerValue(1600));
  apServerDevs_ = p2p.Install(apNodes_.Get(0), serverNodes_.Get(0));
  apServerDevs_.Add(p2p.Install(apNodes_.Get(0), serverNodes_.Get(1)));
  apServerDevs_.Add(p2p.Install(apNodes_.Get(0), serverNodes_.Get(2)));
}

void Domain::ConfigureNetworkLayer() {
  InternetStackHelper stack;
  stack.Install(apNodes_);
  stack.Install(staNodes_);
  stack.Install(serverNodes_);

  Ipv4AddressHelper ipv4Network;
  ipv4Network.SetBase(naddr_.c_str(), smask_.c_str());
  ipv4Network.Assign(apDevs_);
  ipv4Network.Assign(staDevs_);

  ipv4Network.SetBase(naddrApServer_.c_str(), smaskApServer_.c_str());
  Ipv4InterfaceContainer apServer = ipv4Network.Assign(apServerDevs_);

  Ipv4StaticRoutingHelper staticRouting;
  Ptr<Ipv4StaticRouting> sr = staticRouting.GetStaticRouting(apNodes_.Get(0)->GetObject<Ipv4>());
  sr->AddHostRouteTo(apServer.GetAddress(1), 2);
  sr->AddHostRouteTo(apServer.GetAddress(3), 3);
  sr->AddHostRouteTo(apServer.GetAddress(5), 4);
  // sr->AddNetworkRouteTo(Ipv4Address(naddrApServer_.c_str()), Ipv4Mask(smaskApServer_.c_str()), 2);

  for (int i = 0; i < static_cast<int>(serverNodes_.GetN()); i++) {
    sr = staticRouting.GetStaticRouting(serverNodes_.Get(i)->GetObject<Ipv4>());
    sr->AddNetworkRouteTo(Ipv4Address(naddr_.c_str()), Ipv4Mask(smask_.c_str()), apServer.GetAddress(0), 1);
  }

  for (int i = 0; i < static_cast<int>(staNodes_.GetN()); i++) {
    Ipv4Address gateway(apNodes_.Get(0)->GetObject<Ipv4>()->GetAddress(1, 0).GetLocal());
    Ptr<Ipv4StaticRouting> wlan = staticRouting.GetStaticRouting(staNodes_.Get(i)->GetObject<Ipv4>());
    wlan->SetDefaultRoute(gateway, 1);
  }
}

} // namespace WLan
