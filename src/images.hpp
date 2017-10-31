//
//  images.hpp
//  Project2
//
//  Created by Bohan Chen  on 10/24/17.
//

#ifndef images_hpp
#define images_hpp

#include <ofMain.h>

class Images{
public:

//ofVec2f scale;
    ofColor color;
    ofVec2f scale;
    ofVec2f position;


    Images();

    Images(ofColor c,
           ofVec2f s,
           ofVec2f p);

//void update();
void draw();

};

#endif /* images_hpp */
