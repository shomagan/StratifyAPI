

#include "dsp/SignalData.hpp"
#include "dsp/Transform.hpp"
#include "dsp/Filter.hpp"

using namespace dsp;

#define IS_FLOAT 0
#define HAS_SHIFT 1
#define LOCAL_INT_MAX INT16_MAX
#define unsigned_native_type u16
#define native_type q15_t
#define arm_dsp_api_function() api_q15()
#define SignalDataType SignalDataQ15
#define SignalType SignalQ15
#define SignalComplexType SignalComplexQ15
#define FftRealType FftRealQ15
#define FftComplexType FftComplexQ15
#define BiquadFilterType BiquadFilterQ15
#define FirFilterType FirFilterQ15
#define big_type q63_t

#include "SignalDataGeneric.h"
