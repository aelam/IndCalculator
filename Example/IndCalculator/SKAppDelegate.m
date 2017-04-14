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
    IndCalculator *ind = [IndCalculator calculatorWithIndName:@"MA"];
    ind.params = @[@5,@10,@15];
    
    NSArray <IIndCandleStick>*candles = [SKCandleDataBuilder candles];
    
    NSArray *result = [ind calc:candles];
    
    NSLog(@"%@",result);
}

@end
