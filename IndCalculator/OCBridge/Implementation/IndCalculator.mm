//
//  Ind.m
//  Ind
//
//  Created by ryan on 13/04/2017.
//  Copyright © 2017 ryan. All rights reserved.
//

#import "IndCalculator.h"
#import "IIndCalculator.h"
#import "ind.h"
#import "Factory.h"
#import "Registrar.h"
#import "IndDataSet.h"

@interface IndCalculator () {
    CInd     *_ind;
    NSString *_name;
}

@end

@implementation IndCalculator

+ (instancetype)calculatorWithIndName:(NSString *)name {
    return [[self alloc] initWithIndName: name];
}

- (instancetype)initWithIndName:(NSString *)name {
    CInd *newInd = Factory::produce([name UTF8String]);
    if (newInd == nil) {
        return nil;
    }
    
    if (self = [super init]) {
        _ind = newInd;
        _name = name;
    }
    return self;

}

- (void)setParams:(NSArray<NSNumber *> *)params {
    if (_params != params) {
        _params = params;
        
        if (params.count > 6) {
            NSLog(@"参数最多能设置6个, 其他会被忽略");
        }
        _ind->m_cParamSize = _params.count;
        for (NSInteger i = 0; i < params.count; i++) {
            _ind->m_psParam[i] = [_params[i] shortValue];
        }
    }
}

- (NSString *)indName {
    return _name;
}

- (NSArray<IIndDataSet> *)calc:(NSArray<IIndCandleStick> *)items {
    NSInteger itemCount = items.count;
    CFDayMobile *cppItems = [self sticksFromIIndCandleSticks:items];
    _ind->Calc(cppItems, (int)itemCount);
    
    NSMutableArray<IIndDataSet> *results = (NSMutableArray<IIndDataSet> *)[[NSMutableArray alloc] initWithCapacity:_ind->m_cExpSize];
    for (NSInteger dataGroupIndex = 0; dataGroupIndex < _ind->m_cExpSize; dataGroupIndex++) {
        NSMutableArray *values = [NSMutableArray arrayWithCapacity:itemCount];
        NSMutableArray *colors = [NSMutableArray arrayWithCapacity:itemCount];
        
        NSInteger startIndex = _ind->m_pnFirst[dataGroupIndex];
        for (NSInteger dataIndex = 0; dataIndex < itemCount; dataIndex++) {
            CFDayMobile cppItem = cppItems[dataIndex];
            double value = 0;
            if ([_name isEqualToString:@"MA"]) {
                value = cppItem.m_pfMA[dataGroupIndex];
                [values addObject:@(value)];
            } else if ([_name isEqualToString:@"VOL"]) {
                value = cppItem.m_pfVMA[dataGroupIndex];
                [values addObject:@(value)];
            } else if ([_name isEqualToString:@"SAR"]) {
                value = cppItem.m_pfInd[dataGroupIndex];
                if (dataGroupIndex == 0) {
                    [values addObject:@(value)];
                } else {
                    [colors addObject:@(value)];
                }
            } else {
                value = cppItem.m_pfInd[dataGroupIndex];
                [values addObject:@(value)];
            }
        }
        
        IndDataSet *indDataSet = [[IndDataSet alloc] init];
        indDataSet.beginIndex = startIndex;
        indDataSet.values = values;
        indDataSet.colors = colors;

        [results addObject:indDataSet];
    }
    
    return results;
}

- (CFDayMobile *)sticksFromIIndCandleSticks:(NSArray<IIndCandleStick> *)items {
    NSInteger count = items.count;
    if (count == 0) {
        return nil;
    }
    CFDayMobile *cppItems = (CFDayMobile *)malloc(sizeof(CFDayMobile) * count);
    
    for (NSInteger i = 0; i < count; i++) {
        id<IIndCandleStick> item = items[i];
        cppItems[i].m_fHigh = item.high;
        cppItems[i].m_fOpen = item.open;
        cppItems[i].m_fLow = item.low;
        cppItems[i].m_fClose = item.close;
        cppItems[i].m_fVolume = item.volume;
    }
    
    return cppItems;
}

- (void)dealloc {
    if (_ind) {
        delete _ind;
    }
}

@end


