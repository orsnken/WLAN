#include "ConstantParam.h"
#include "SimulationSettings.h"

namespace WLan {

namespace Global {

double distanceWLans = kDistanceWLans;

int apCwMin = kEdcaApCwMin;
int apCwMax = kEdcaApCwMax;
int apAifsn = kEdcaApAifsn;

int staCwMin = kEdcaStaCwMin;
int staCwMax = kEdcaStaCwMax;
int staAifsn = kEdcaStaAifsn;

std::string trafficType = "UDP";

} // namespace Global

} // namespace WLan
