//
//  main.cpp
//  TerraGen
//
//  Created by James Lennon on 3/12/15.
//  Copyright (c) 2015 James Lennon. All rights reserved.
//

#include <iostream>
#include "TerrainGenerator.h"

int main(int argc, const char * argv[]) {
    
    TG::TerrainGenerator gen;
    gen.generateMap(500, 500);
    
    return 0;
}
