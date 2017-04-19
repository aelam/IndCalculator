//
//  IIndCandleStick.h
//  Ind
//
//  Created by ryan on 14/04/2017.
//  Copyright © 2017 ryan. All rights reserved.
//

#import <Foundation/Foundation.h>

@protocol IIndCandleStick <NSObject>

@property (nonatomic, assign) double high;
@property (nonatomic, assign) double open;
@property (nonatomic, assign) double close;
@property (nonatomic, assign) double low;
@property (nonatomic, assign) double volume;
@property (nonatomic, assign) double datetime;

@end
