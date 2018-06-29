#include "ConstantParam.h"

namespace {

const WLan::EdcaAc kAcVo("VO_EdcaTxopN", 0xc0);
const WLan::EdcaAc kAcVi("VI_EdcaTxopN", 0xb8);
const WLan::EdcaAc kAcBk("BK_EdcaTxopN", 0x28);
const WLan::EdcaAc kAcBe("BE_EdcaTxopN", 0x70);

} // namespace

namespace WLan {

const EdcaAc kEdcaStaAc     = kAcBk;
const int    kEdcaStaCwMin  = 30;
const int    kEdcaStaCwMax  = 1024;
const int    kEdcaStaAifsn  = 1;
const int    kEdcaStaTxopMs = 0;

const EdcaAc kEdcaApAc     = kAcBe;
const int    kEdcaApCwMin  = 0;
const int    kEdcaApCwMax  = 1;
const int    kEdcaApAifsn  = 4;
const int    kEdcaApTxopMs = 0;

const std::string kAppDataRate = "16Mbps";
const unsigned int kAppPacketSizeByte = 1472;

} // namespace WLan
