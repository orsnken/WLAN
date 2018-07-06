#ifndef CONSTANT_PARAM_H__
#define CONSTANT_PARAM_H__

#include "ns3.h"

#include <string>

namespace WLan {

struct EdcaAc {
  const std::string str;
  const unsigned int id;
  EdcaAc(std::string s, unsigned int n) : str(s), id(n) {}
};

extern const double kDistanceWLans;
extern const double kDistanceApSta;
extern const double kSimulationTimeSec;
extern const double kApplicationBeginSec;
extern const double kApplicationEndSec;

extern const EdcaAc kEdcaStaAc;
extern const int    kEdcaStaCwMin;
extern const int    kEdcaStaCwMax;
extern const int    kEdcaStaAifsn;
extern const int    kEdcaStaTxopMs;

extern const EdcaAc kEdcaApAc;
extern const int    kEdcaApCwMin;
extern const int    kEdcaApCwMax;
extern const int    kEdcaApAifsn;
extern const int    kEdcaApTxopMs;

extern const std::string kAppDataRate;
extern const unsigned int kAppPacketSizeByte;

} // namespace WLan

#endif  // CONSTANT_PARAM_H__