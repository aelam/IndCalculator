//
//  SKCandleDataBuilder.m
//  IndCalculator
//
//  Created by Ryan Wang on 15/04/2017.
//  Copyright © 2017 aelam. All rights reserved.
//

#import "SKCandleDataBuilder.h"
#import "IIndCandleStick.h"

@implementation SKCandleDataBuilder

+ (NSArray <IIndCandleStick> *)candles {
    NSString * path = [[NSBundle mainBundle] pathForResource:@"candle" ofType:@"json"];
    NSData *data = [[NSData alloc] initWithContentsOfFile:path];
    NSArray *candleNumbers = [NSJSONSerialization JSONObjectWithData:data options:0 error:nil];
    NSMutableArray <IIndCandleStick> *candles = (NSMutableArray <IIndCandleStick> *)[NSMutableArray array];
    for (NSArray *numbers in candleNumbers) {
        IndCandleStick *stick = [[IndCandleStick alloc] init];
        stick.datetime = [numbers[0] doubleValue];
        stick.open = [numbers[1] doubleValue];
        stick.high = [numbers[2] doubleValue];
        stick.low = [numbers[3] doubleValue];
        stick.close = [numbers[4] doubleValue];
        
        [candles addObject:stick];
    }
    
    return candles;
}

@end
