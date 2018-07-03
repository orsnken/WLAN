#include "ConstantParam.h"

namespace {

const WLan::EdcaAc kAcVo("VO_EdcaTxopN", 0xc0);
const WLan::EdcaAc kAcVi("VI_EdcaTxopN", 0xb8);
const WLan::EdcaAc kAcBk("BK_EdcaTxopN", 0x28);
const WLan::EdcaAc kAcBe("BE_EdcaTxopN", 0x70);

} // namespace

namespace WLan {

const double kDistanceWLans = 15.0;
const double kDistanceApSta = 0.3;
const double kSimulationTimeSec = 3.0;

const EdcaAc kEdcaStaAc     = kAcBk;
const int    kEdcaStaCwMin  = 10;
const int    kEdcaStaCwMax  = 1023;
const int    kEdcaStaAifsn  = 2;
const int    kEdcaStaTxopMs = 0;

const EdcaAc kEdcaApAc     = kAcVo;
const int    kEdcaApCwMin  = 0;
const int    kEdcaApCwMax  = 1;
const int    kEdcaApAifsn  = 4;
const int    kEdcaApTxopMs = 0;

const std::string kAppDataRate = "54Mbps";
const unsigned int kAppPacketSizeByte = 1472;

} // namespace WLan
