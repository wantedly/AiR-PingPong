//
//  table.h
//  pinpong
//
//  Created by Naoyoshi Aikawa on 6/21/15.
//
//

#ifndef __pinpong__table__
#define __pinpong__table__

#include <stdio.h>

#include "ball.h"


class Table {
private:
    Ball ball;
public:
    const int TABLE_WIDTH = 274;  // 2.74m
    const int TABLE_HEIGHT = 152;  // 1.525m

    Table();
    void update();
    void draw(const ofMatrix4x4& mat);
};


#endif /* defined(__pinpong__table__) */
