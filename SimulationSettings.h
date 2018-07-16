#ifndef SIMULATION_SETTINGS_H__
#define SIMULATION_SETTINGS_H__

#include <string>

namespace WLan {

namespace Global {

extern double distanceWLans;

extern int apCwMin;
extern int apCwMax;
extern int apAifsn;

extern int staCwMin;
extern int staCwMax;
extern int staAifsn;

extern std::string trafficType;

} // namespace Global

} // namespace WLan

#endif  // SIMULATION_SETTINGS_H__
