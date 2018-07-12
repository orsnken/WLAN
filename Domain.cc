#include "ns3.h"

#include "ConstantParam.h"
#include "Domain.h"

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
    "Exponent"         , DoubleValue(3.71),
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
  sPhy.Set("TxPowerEnd"              , DoubleValue(10.0206));
  sPhy.Set("TxPowerStart"            , DoubleValue(10.0206));
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
  int nnodes
) : ch_(ch), naddr_(naddr), smask_(smask) {
  ssid_ = Ssid(ssid);
  apNodes_.Create(1);
  staNodes_.Create(nnodes);
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
  edca->SetMinCw(kEdcaApCwMin);
  edca->SetMaxCw(kEdcaApCwMax);
  edca->SetAifsn(kEdcaApAifsn);
  edca->SetTxopLimit(MilliSeconds(kEdcaApTxopMs));

  wifiMac->GetAttribute(kEdcaStaAc.str, ptr);
  edca = ptr.Get<EdcaTxopN>();
  edca->SetMinCw(kEdcaStaCwMin);
  edca->SetMaxCw(kEdcaStaCwMax);
  edca->SetAifsn(kEdcaStaAifsn);
  edca->SetTxopLimit(MilliSeconds(kEdcaStaTxopMs));
}

void Domain::ConfigureNetworkLayer() {
  InternetStackHelper stack;
  stack.Install(apNodes_);
  stack.Install(staNodes_);

  Ipv4AddressHelper ipv4Network;
  ipv4Network.SetBase(naddr_.c_str(), smask_.c_str());
  ipv4Network.Assign(apDevs_);
  ipv4Network.Assign(staDevs_);
}

} // namespace WLan
