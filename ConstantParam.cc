#include "ConstantParam.h"

namespace {

const WLan::EdcaAc kAcVo("VO_EdcaTxopN", 0xc0);
const WLan::EdcaAc kAcVi("VI_EdcaTxopN", 0xb8);
const WLan::EdcaAc kAcBk("BK_EdcaTxopN", 0x28);
const WLan::EdcaAc kAcBe("BE_EdcaTxopN", 0x70);

} // namespace

namespace WLan {

const double kDistanceWLans = 5.0;
const double kDistanceApSta = 0.3;
const double kSimulationTimeSec = 20.0;
const double kApplicationBeginSec = 0.1;
const double kApplicationEndSec   = kSimulationTimeSec - 0.1;

const EdcaAc kEdcaStaAc     = kAcBk;
const int    kEdcaStaCwMin  = 7;
const int    kEdcaStaCwMax  = 31;
const int    kEdcaStaAifsn  = 2;
const int    kEdcaStaTxopMs = 0;

const EdcaAc kEdcaApAc     = kAcVo;
const int    kEdcaApCwMin  = 7;
const int    kEdcaApCwMax  = 31;
const int    kEdcaApAifsn  = 2;
const int    kEdcaApTxopMs = 0;

const std::string kAppDataRate = "54Mbps";
const unsigned int kAppPacketSizeByte = 1472;

} // namespace WLan
