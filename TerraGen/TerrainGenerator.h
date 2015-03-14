//
//  TerrainGenerator.h
//  TerraGen
//
//  Created by James Lennon on 3/12/15.
//  Copyright (c) 2015 James Lennon. All rights reserved.
//

#ifndef __TerraGen__TerrainGenerator__
#define __TerraGen__TerrainGenerator__

#include <stdio.h>
#include <noise/noise.h>
#include <set>
#include <vector>
#include "noise/noiseutils.h"

namespace TG {
    
    enum BiomeType{
        Forest, Trop_desert, Dry_desert, Woods, Rocky, Tundra, Snow_forest, Grassland
    };
    
    enum TileType{
        Water, ShallowWater, Sand, Dirt, Dark, Grass, Lava, Stone, Tile, Snow
    };
    
    enum ExtraType{
        None, ForestTree, DesertTree, SnowTree, DarkTree, Rock, Cow, Werewolf, RockMonster
    };
    
    class Tile{
        
    public:
        Tile();
        TileType type;
        ExtraType extra;
        
    private:
        
    };
    
    class Map{
        
    public:
        Map(int w, int h);
        int getWidth();
        int getHeight();
        void setTileType(TileType type, int x, int y);
        class Tile& getTile(int x, int y);
        /*
         void save(string fileName);
         void load(string fileName);
         */
        
    private:
        int _w,_h;
        std::vector<class Tile> grid;
        
        void setWidth(int w);
        void setHeight(int h);
        bool isInBounds(int x, int y);
    };
    
    void print(Map& m);
    
    class TerrainGenerator {
        
    public:
        TerrainGenerator();
        void generateMap(int w, int h);
        Map* getResult();
        
    private:
        Map *_result;
        int _w, _h;
        float _h1, _h2, _h3, _m1, _m2;
        float _waterline, _deepwaterline;
        
        void fillNoiseMap(utils::NoiseMap &map, module::Perlin& perlin);
        void smooth(utils::NoiseMap& map);
        std::set<float> generateSet(utils::NoiseMap& map);
        BiomeType chooseBiome(float h, float m);
        void handleBiome(BiomeType bt, float detail, class Tile& tile);
    };
    
}


#endif /* defined(__TerraGen__TerrainGenerator__) */
