//
//  SKAppDelegate.m
//  IndCalculator
//
//  Created by aelam on 04/14/2017.
//  Copyright (c) 2017 aelam. All rights reserved.
//

@import IndCalculator;
#import "SKAppDelegate.h"
#import "SKCandleDataBuilder.h"

@implementation SKAppDelegate

+ (void)load {
    IndCalculator *ind = [IndCalculator calculatorWithIndName:@"SAR"];
    ind.params = @[@10,@2,@20];
    
    NSArray <IIndCandleStick>*candles = [SKCandleDataBuilder candles];
    
    NSArray *result = [ind calc:candles];
    
    NSLog(@"%@",result);
    IndDataSet *dataSet = result.firstObject;
    
    for (NSInteger i = 0; i < dataSet.values.count; i++) {
        NSLog(@"date: %f value: %@, color: %@",[candles[i] datetime], dataSet.values[i], dataSet.colors[i]);
    }
    
}

@end
