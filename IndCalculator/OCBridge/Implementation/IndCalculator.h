//
//  Ind.h
//  Ind
//
//  Created by ryan on 13/04/2017.
//  Copyright © 2017 ryan. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "IIndCalculator.h"

@protocol IIndCandleStick;
@protocol IIndCalculator;
@class IndDataSet;

@interface IndCalculator: NSObject <IIndCalculator>

+ (instancetype)calculatorWithIndName:(NSString *)name;

@property (nonatomic, strong) NSArray<NSNumber *> *params;
@property (nonatomic, readonly) NSString *indName;

- (NSArray<IndDataSet *> *)calc:(NSArray<IIndCandleStick> *)items;

@end

