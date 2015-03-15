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
    gen.generateMap(67, 67);
    print(*gen.getResult());
    
    return 0;
}
