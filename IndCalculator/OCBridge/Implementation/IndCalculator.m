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

@interface IndCalculator () {
    CInd *_ind;
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

- (NSArray *)calc:(NSArray<IIndCandleStick> *)items {
    NSAssert(false, @"子类实现");
    return nil;
}

- (void)dealloc {
}

@end


