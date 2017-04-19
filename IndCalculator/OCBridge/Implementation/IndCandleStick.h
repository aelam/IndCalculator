//
//  IndCandleStick.h
//  Pods
//
//  Created by Ryan Wang on 14/04/2017.
//
//

#import <Foundation/Foundation.h>
#import "IIndCandleStick.h"

@interface IndCandleStick : NSObject <IIndCandleStick>

@property (nonatomic, assign) double high;
@property (nonatomic, assign) double open;
@property (nonatomic, assign) double close;
@property (nonatomic, assign) double low;
@property (nonatomic, assign) double volume;
@property (nonatomic, assign) double datetime;

@end
