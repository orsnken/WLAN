#include "ns3.h"

#include "ConstantParam.h"
#include "Domain.h"

namespace WLan {

using namespace ns3;

Domain::Domain(std::string ssid, int ch, int nnodes) : ch_(ch){
  ssid_ = Ssid(ssid);
  apNodes_.Create(1);
  staNodes_.Create(nnodes);
  ConfigureDataLinkLayer();
}

void Domain::ConfigureDataLinkLayer() {
  WifiHelper wifi;
  wifi.SetStandard(WIFI_PHY_STANDARD_80211g);
  wifi.SetRemoteStationManager(
    "ns3::ConstantRateWifiManager",
    "DataMode"   , StringValue("ErpOfdmRate54Mbps"),
    "ControlMode", StringValue("ErpOfdmRate6Mbps")
  );
  YansWifiChannelHelper channel;
  channel.SetPropagationDelay("ns3::ConstantSpeedPropagationDelayModel");
  channel.AddPropagationLoss(
    "ns3::LogDistancePropagationLossModel",
    "Exponent"         , DoubleValue(3.0),
    "ReferenceDistance", DoubleValue(1.0),
    "ReferenceLoss"    , DoubleValue(76.645997)
  );
  YansWifiPhyHelper phy = YansWifiPhyHelper::Default();
  phy.SetPcapDataLinkType(YansWifiPhyHelper::DLT_IEEE802_11_RADIO);
  phy.SetChannel(channel.Create());
  phy.Set("EnergyDetectionThreshold", DoubleValue(-96));
  phy.Set("CcaMode1Threshold"       , DoubleValue(-99));
  phy.Set("TxPowerEnd"              , DoubleValue(10.0206));
  phy.Set("TxPowerStart"            , DoubleValue(10.0206));

  WifiMacHelper mac;
  phy.Set("ChannelNumber", UintegerValue(ch_));
  mac.SetType(
    "ns3::StaWifiMac",
    "ActiveProbing", BooleanValue(false),
    "QosSupported" , BooleanValue(true),
    "Ssid"         , SsidValue(ssid_)
  );
  wifi.Install(phy, mac, staNodes_);
  mac.SetType(
    "ns3::ApWifiMac",
    "Ssid", SsidValue(ssid_)
  );
  wifi.Install(phy, mac, apNodes_);

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

} // namespace WLan
