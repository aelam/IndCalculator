//
//  IndCalculator.h
//  Ind
//
//  Created by ryan on 14/04/2017.
//  Copyright © 2017 ryan. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "IIndCandleStick.h"

@protocol IIndCalculator;
@protocol IIndDataSet;
@class IndDataSet;

@protocol IIndCalculator <NSObject>

@property (nonatomic, strong) NSArray<NSNumber *> *params;

- (NSArray<IndDataSet *> *)calc:(NSArray<IIndCandleStick> *)items;

@end
