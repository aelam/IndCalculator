//
//  IndDataSet.m
//  Ind
//
//  Created by ryan on 14/04/2017.
//  Copyright © 2017 ryan. All rights reserved.
//

#import "IndDataSet.h"

@implementation IndDataSet

- (NSString *)description {
    return [NSString stringWithFormat:@"startIndex:%zd, values: %@ colors: %@", _beginIndex, _values, _colors];
}

@end
