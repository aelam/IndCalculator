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

typedef NS_ENUM(NSInteger, IndLineType) {
    IndLineTypeDots,      // 点 SAR
    IndLineTypeLine,      // 线
    IndLineTypeThinStick, // MACD
    IndLineTypeStick      // VOL
};

@interface IndDataSet : NSObject <IIndDataSet>

@property (nonatomic, assign) NSInteger beginIndex;
@property (nonatomic, assign) NSInteger coloredIndIndex;
@property (nonatomic,   copy) NSString  *dataSetName;
@property (nonatomic, strong) NSArray<NSNumber *> *values;
@property (nonatomic, strong) NSArray<NSNumber *> *colors;

@end
