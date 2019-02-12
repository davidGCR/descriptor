//
//  sample_cube.h
//  descriptor
//
//  Created by David Choqueluque Roman on 2/12/19.
//  Copyright © 2019 David Choqueluque Roman. All rights reserved.
//

#ifndef sample_cube_h
#define sample_cube_h
class Sample_Cube{
public:
    int x, y,t;
    int width;
    int height;
    int length;
    
    Sample_Cube();
    Sample_Cube(int x,int y, int z, int w, int h, int l);
};

Sample_Cube::Sample_Cube(int x,int y, int t, int w, int h, int l):x(x),y(y),t(t),width(w),height(h), length(l){
    
}
#endif /* sample_cube_h */
