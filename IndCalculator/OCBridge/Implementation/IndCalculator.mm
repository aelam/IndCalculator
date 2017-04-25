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
#include <string>

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
        
        if (params.count > 7) {
            NSLog(@"参数最多能设置7个, 其他会被忽略");
        }
        
        _ind->SetParamSize((char)_params.count);
        for (int i = 0; i < params.count; i++) {
            _ind->SetParam([_params[i] shortValue], i);
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

#if 0
    if (![_name isEqualToString:@"MA"]) {
        
        NSLog(@"param:%@\n", _params);
        printf("================\n");
        printf("BEGIN POSITION: \n");
        for (int i = 0 ; i <= 6; i++) {
            printf("%d ", _ind->GetBeginIndex(i));
        }
        printf("\n================\n");
        
        for (int i = 0 ; i < itemCount; i++) {
            printf("date %d", cppItems[i].m_dwTime);
            for (int j = 0; j < _ind->GetOutlineCount(); j++) {
                printf(" r%d: %0.2f ", j ,cppItems[i].m_pfInd[j]);
            }
            printf("\n");
        }
        printf("\n================\n");
    }
#endif
    
    NSInteger expSize = _ind->GetOutlineCount();

    NSMutableArray<IIndDataSet> *results = (NSMutableArray<IIndDataSet> *)[[NSMutableArray alloc] initWithCapacity:expSize];
    for (NSInteger dataGroupIndex = 0; dataGroupIndex < expSize; dataGroupIndex++) {
        NSMutableArray *values = [NSMutableArray arrayWithCapacity:itemCount];
        NSMutableArray *colors = [NSMutableArray arrayWithCapacity:itemCount];
        
        NSInteger coloredIndIndex = _ind->GetColoredIndIndex();
        NSInteger startIndex = _ind->GetBeginIndex((int)dataGroupIndex);
        std::string lineName = _ind->GetOutlineName((int)dataGroupIndex);
        NSString *dataSetName = [NSString stringWithCString:lineName.c_str()
                                                   encoding:[NSString defaultCStringEncoding]];
        
        for (NSInteger dataIndex = 0; dataIndex < itemCount; dataIndex++) {
            CFDayMobile cppItem = cppItems[dataIndex];
            [values addObject:@(cppItem.m_pfInd[dataGroupIndex])];
            [colors addObject:@(cppItem.m_color)];
        }

        IndDataSet *indDataSet = [[IndDataSet alloc] init];
        indDataSet.beginIndex = startIndex;
        indDataSet.values = values;
        indDataSet.dataSetName = dataSetName;
        
        if (coloredIndIndex >= 0 && dataGroupIndex == coloredIndIndex) {
            indDataSet.colors = colors;
        }

        [results addObject:indDataSet];
    }
    
    delete [] cppItems;
    
    return results;
}

- (CFDayMobile *)sticksFromIIndCandleSticks:(NSArray<IIndCandleStick> *)items {
    NSInteger count = items.count;
    if (count == 0) {
        return nil;
    }
    CFDayMobile *cppItems = new CFDayMobile[count];
    
    for (NSInteger i = 0; i < count; i++) {
        id<IIndCandleStick> item = items[i];
        cppItems[i].m_fHigh = item.high;
        cppItems[i].m_fOpen = item.open;
        cppItems[i].m_fLow = item.low;
        cppItems[i].m_fClose = item.close;
        cppItems[i].m_fVolume = item.volume;
        cppItems[i].m_dwTime = item.datetime;
    }
    
    return cppItems;
}

- (void)dealloc {
    if (_ind) {
        delete _ind;
    }
}

@end


