//
//  TerrainGenerator.cpp
//  TerraGen
//
//  Created by James Lennon on 3/12/15.
//  Copyright (c) 2015 James Lennon. All rights reserved.
//

#include "TerrainGenerator.h"
#include <iostream>
#include "noise/interp.h"

int dx[] = {-1,0,1,0}, dy[] = {0,0,1,-1};

using namespace TG;
using namespace noise;
using namespace std;

Map::Map(int w, int h){
    _w = w;
    _h = h;
    grid = (TileType*)malloc(_w * _h * sizeof(TileType));
}

void Map::setHeight(int h){
    _h = h;
}

void Map::setWidth(int w){
    _w = w;
}

int Map::getWidth(){
    return _w;
}

int Map::getHeight(){
    return _h;
}

bool Map::isInBounds(int x, int y){
    return x>=0 && x<_w && y>=0 && y<_h;
}

TileType& Map::getTile(int x, int y){
    return grid[(y*_w + x%_w)];
}

void Map::setTile(TG::TileType type, int x, int y){
    getTile(x, y) = type;
}

void render(utils::NoiseMap& heightMap, std::string name){
    utils::RendererImage renderer;
    utils::Image image;
    renderer.SetSourceNoiseMap(heightMap);
    renderer.SetDestImage(image);
//    renderer.EnableLight();
//    renderer.SetLightContrast(3.0);
//    renderer.SetLightBrightness(2.0);
    renderer.Render();
    
    utils::WriterBMP writer;
    writer.SetSourceImage(image);
    writer.SetDestFilename(name);
    writer.WriteDestFile();
}

void generate(module::Perlin& alt, int w, int h){
    utils::NoiseMap heightMap;
    utils::NoiseMapBuilderPlane heightMapBuilder;
    heightMapBuilder.SetSourceModule(alt);
    heightMapBuilder.SetDestNoiseMap(heightMap);
    heightMapBuilder.SetDestSize(w, h);
    heightMapBuilder.SetBounds(0.0, w, 0.0, h);
    heightMapBuilder.Build();
    
    utils::RendererImage renderer;
    utils::Image image;
    renderer.ClearGradient();
    renderer.AddGradientPoint(-1, utils::Color(255,0,0,255));
    renderer.AddGradientPoint(1, utils::Color(0,0,255,255));
    renderer.SetSourceNoiseMap(heightMap);
    renderer.SetDestImage(image);
    renderer.EnableLight();
    renderer.SetLightContrast(3.0);
    renderer.SetLightBrightness(2.0);
    renderer.Render();
    
    utils::WriterBMP writer;
    writer.SetSourceImage(image);
    writer.SetDestFilename ("tutorial.bmp");
    writer.WriteDestFile();
}

void print(Map& m){
    for (int j=0; j<m.getHeight(); j++) {
        for (int i=0; i<m.getWidth(); i++) {
            TileType t = m.getTile(i, j);
            char c = '0';
            switch (t) {
                case Grass:
                    c = 'G';
                    break;
                case Water:
                    c = '-';
                    break;
                case ShallowWater:
                    c = '~';
                    break;
                case Sand:
                    c = 'S';
                    break;
                case Dirt:
                    c = 'D';
                    break;
                case Dark:
                    c = '%';
                    break;
                case Lava:
                    c = 'L';
                    break;
                case Stone:
                    c = 'T';
                    break;
                case Snow:
                    c = 'M';
                    break;
                    
                default:
                    c = '.';
                    break;
            }
            printf("%c ", c);
        }
        printf("\n");
    }
}

TerrainGenerator::TerrainGenerator(){
    _result = nullptr;
}

void TerrainGenerator::fillNoiseMap(utils::NoiseMap &map, module::Perlin& perlin){
    utils::NoiseMapBuilderPlane heightMapBuilder;
    heightMapBuilder.SetSourceModule(perlin);
    heightMapBuilder.SetDestNoiseMap(map);
    heightMapBuilder.SetDestSize(_w, _h);
    heightMapBuilder.SetBounds(5.0, _w+5, 5.0, _h+5);
    heightMapBuilder.Build();
}

set<float> TerrainGenerator::generateSet(utils::NoiseMap& map){
    set<float> st;
    
    for (int j=0; j<_h; j++) {
        for (int i=0; i<_w; i++) {
            float val = map.GetValue(i, j);
            st.insert(val);
        }
    }
    
    return st;
}

BiomeType TerrainGenerator::chooseBiome(float h, float m) {
    BiomeType t;
    if (h > _h1) {
        if (m > _m1) {
            t = BiomeType::Snow_forest;
        } else if (m > _m2) {
            t = BiomeType::Tundra;
        } else {
            t = BiomeType::Rocky;
        }
    } else if (h > _h2) {
        if (m > _m1) {
            t = BiomeType::Forest;
        } else if (m > _m2) {
            t = BiomeType::Woods;
        } else {
            t = BiomeType::Trop_desert;
        }
    } else if (h > _h3) {
        if (m > _m1) {
            t = BiomeType::Forest;
        } else if (m > _m2) {
            t = BiomeType::Grassland;
        } else {
            t = BiomeType::Trop_desert;
        }
    } else {
        if (m > _m1) {
            t = BiomeType::Forest;
        } else if (m > _m2) {
            t = BiomeType::Grassland;
        } else {
            t = BiomeType::Dry_desert;
        }
    }
    
    return t;
}

TileType TerrainGenerator::handleBiome(BiomeType bt, float dval){
    dval = fabs(dval);
    TileType type;
    switch (bt) {
        case Forest:
            type = TileType::Grass;
//            if (dval > 90) {
//                npc = new Cow(i, j);
//            } else if (dval > 15) {
//                npc = new Tree(i, j, type);
//            }
            break;
        case Dry_desert:
            type = TileType::Sand;
            break;
        case Trop_desert:
            type = TileType::Sand;
//            if (dval > 80)
//                npc = new Tree(i, j, type);
            break;
        case Woods:
            type = TileType::Dark;
//            if (dval > 100)
//                npc = new Werewolf(i, j);
//            else if (dval > 15)
//                npc = new Tree(i, j, Type.dark);
            break;
        case Rocky:
            type = TileType::Stone;
//            if (dval > 100)
//                npc = new RockMonster(i, j);
//            else if (dval > 25)
//                npc = new Rock(i, j);
            break;
        case Tundra:
            type = TileType::Snow;
            if (dval > 40)
                type = TileType::Dirt;
            break;
        case Snow_forest:
            type = TileType::Snow;
//            if (dval > 15)
//                npc = new Tree(i, j, Type.snow);
            break;
        case Grassland:
            type = TileType::Grass;
            if (dval > 20)
                type = TileType::Dirt;
            break;
        default:
            throw new ExceptionInvalidParam();
            break;
    }
    return type;
//    Tile t = new Tile(type, i, j);
//    gm.put(i, j, t);
//    if (npc != null)
//        npcm.add(npc);
}

void TerrainGenerator::generateMap(int w, int h){
    if(_result!=nullptr){
        delete _result;
        _result = nullptr;
    }
    
    srand((int)time(0));
    _w = w;
    _h = h;
    _result = new Map(w,h);
    module::Perlin alt, moist, details;
    alt.SetSeed(rand() * RAND_MAX);
    alt.SetOctaveCount(8);
    alt.SetFrequency(.02);
    
    moist.SetSeed((int)random());
    moist.SetOctaveCount(8);
    moist.SetFrequency(.05);
    
    details.SetSeed((int)random());
    details.SetOctaveCount(8);
    details.SetFrequency(4.9);
    details.SetPersistence(2.0);
    
    utils::NoiseMap aMap, mMap, dMap;
    fillNoiseMap(aMap, alt);
    fillNoiseMap(mMap, moist);
    fillNoiseMap(dMap, details);
    
    render(aMap, "alt.bmp");
    render(mMap, "moist.bmp");
    render(dMap, "details.bmp");
    
    set<float> aSet = generateSet(aMap), mSet = generateSet(mMap);
    float aLow = *aSet.begin(), aHigh = *(--aSet.end());
    float mLow = *mSet.begin(), mHigh = *(--mSet.end());
    
    _h3 = (float)LinearInterp(aLow,aHigh,.25);
    _h2 = (float)LinearInterp(aLow,aHigh,.5);
    _h1 = (float)LinearInterp(aLow,aHigh,.75);
    _m2 = (float)LinearInterp(mLow,mHigh,1/3.f);
    _m1 = (float)LinearInterp(mLow,mHigh,2/3.f);
    _waterline = (float)LinearInterp(aLow,aHigh,.15);
    _deepwaterline = (float)LinearInterp(aLow,aHigh,.1);
    
    for (int j=0; j<_h; j++) {
        for (int i=0; i<_w; i++) {
            float aval = aMap.GetValue(i, j), mval = mMap.GetValue(i, j);
            TileType type;
            if(aval>_waterline){
                BiomeType bt = chooseBiome(aMap.GetValue(i, j), mMap.GetValue(i, j));
                type = handleBiome(bt, dMap.GetValue(i, j));
            }else if(aval>_deepwaterline){
                type = TileType::ShallowWater;
            }else{
                type = TileType::Water;
            }
            _result->getTile(i, j) = type;
        }
    }
    print(*_result);
}
