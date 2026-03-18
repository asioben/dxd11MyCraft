#include "../includes/block.h"

void getBlock(BLOCKID block, int *indexes){
    switch(block){
        case GRASS:{
            indexes[0] = 3;
            indexes[1] = 3;
            indexes[2] = 0;
            indexes[3] = 2;
            indexes[4] = 3;
            indexes[5] = 3;
        }break;
        case DIRT:{
            indexes[0] = 2;
            indexes[1] = 2;
            indexes[2] = 2;
            indexes[3] = 2;
            indexes[4] = 2;
            indexes[5] = 2;
        }break;
        case STONE:{
            indexes[0] = 1;
            indexes[1] = 1;
            indexes[2] = 1;
            indexes[3] = 1;
            indexes[4] = 1;
            indexes[5] = 1;
        }break;
    }
}

void getFace(BLOCKID block, TEXTURE *texture, int face, VERTEX *vertex){
    int indexes[6];
    SPRITE sprites[6];

    getBlock(block,indexes);
    for(int i = 0; i < 6; i++) sprites[i] = texture->getSprite(indexes[i],34,24);

    //Triangles
    VERTEX cube[24] = {
        // Front face
        {{0.0f, 0.0f, 0.0f},{sprites[0].u, sprites[0].v + sprites[0].height}},
        {{0.0f,  1.0f, 0.0f},{sprites[0].u, sprites[0].v}},
        {{ 1.0f,  1.0f, 0.0f},{sprites[0].u + sprites[0].width, sprites[0].v}},
        {{ 1.0f, 0.0f, 0.0f},{sprites[0].u + sprites[0].width, sprites[0].v + sprites[0].height}},
    
        // Back face
        {{0.0f, 0.0f, 1.0f},{sprites[1].u + sprites[1].width, sprites[1].v + sprites[1].height}},
        {{ 1.0f, 0.0f, 1.0f},{sprites[1].u, sprites[1].v + sprites[1].height}},
        {{ 1.0f,  1.0f, 1.0f},{sprites[1].u, sprites[1].v}},
        {{0.0f,  1.0f, 1.0f},{sprites[1].u + sprites[1].width, sprites[1].v}},
    
        // Top face
        {{0.0f, 1.0f, 0.0f},{sprites[2].u, sprites[2].v + sprites[2].height}},
        {{0.0f, 1.0f,  1.0f},{sprites[2].u, sprites[2].v}},
        {{ 1.0f, 1.0f,  1.0f},{sprites[2].u + sprites[2].width, sprites[2].v}},
        {{ 1.0f, 1.0f, 0.0f},{sprites[2].u + sprites[2].width, sprites[2].v + sprites[2].height}},
    
        // Bottom face
        {{0.0f, 0.0f, 0.0f},{sprites[3].u + sprites[3].width, sprites[3].v + sprites[3].height}},
        {{ 1.0f, 0.0f, 0.0f},{sprites[3].u, sprites[3].v + sprites[3].height}},
        {{ 1.0f, 0.0f,  1.0f},{sprites[3].u, sprites[3].v}},
        {{0.0f, 0.0f,  1.0f},{sprites[3].u + sprites[3].width, sprites[3].v}},
    
        // Left face
        {{0.0f, 0.0f,  1.0f},{sprites[4].u, sprites[4].v + sprites[4].height}},
        {{0.0f,  1.0f,  1.0f},{sprites[4].u, sprites[4].v}},
        {{0.0f,  1.0f, 0.0f},{sprites[4].u + sprites[4].width, sprites[4].v}},
        {{0.0f, 0.0f, 0.0f},{sprites[4].u + sprites[4].width, sprites[4].v + sprites[4].height}},
    
        // Right face
        {{1.0f, 0.0f, 0.0f},{sprites[5].u, sprites[5].v + sprites[5].height}},
        {{1.0f,  1.0f, 0.0f},{sprites[5].u, sprites[5].v}},
        {{1.0f,  1.0f,  1.0f},{sprites[5].u + sprites[5].width, sprites[5].v}},
        {{1.0f, 0.0f,  1.0f},{sprites[5].u + sprites[5].width, sprites[5].v + sprites[5].height}},
    };
    
    short indice[36] = {
        //front face
        0,1,2,
        0,2,3,
        //back face
        4,5,6,
        4,6,7,
        //top face
        8,9,10,
        8,10,11,
        //bottom face
        12,13,14,
        12,14,15,
        //left face
        16,17,18,
        16,18,19,
        //right face
        20,21,22,
        20,22,23
    };

    for(int j = 0; j < 6; j++){
        if(j < 4)vertex[j] = cube[face * 4 +j ];
    }
}