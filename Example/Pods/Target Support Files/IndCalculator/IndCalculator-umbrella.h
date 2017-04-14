#ifdef __OBJC__
#import <UIKit/UIKit.h>
#else
#ifndef FOUNDATION_EXPORT
#if defined(__cplusplus)
#define FOUNDATION_EXPORT extern "C"
#else
#define FOUNDATION_EXPORT extern
#endif
#endif
#endif

#import "IndCalculator.h"
#import "IIndCalculator.h"
#import "IIndCandleStick.h"
#import "IIndDataSet.h"
#import "IndDataSet.h"

FOUNDATION_EXPORT double IndCalculatorVersionNumber;
FOUNDATION_EXPORT const unsigned char IndCalculatorVersionString[];

