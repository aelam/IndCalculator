//
//  IndDataSet.h
//  Ind
//
//  Created by ryan on 14/04/2017.
//  Copyright © 2017 ryan. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "IIndDataSet.h"

@protocol IIndCandleStick;
@protocol IIndCalculator;
@protocol IIndDataSet;

@interface IndDataSet : NSObject <IIndDataSet>

@property (nonatomic, assign) NSInteger beginIndex;
@property (nonatomic, strong) NSArray<NSNumber *> *values;
@property (nonatomic, strong) NSArray<NSNumber *> *colors;

@end
