#include "../includes/chunk.h"

void CHUNK::initChunk(int seed, TEXTURE *texture){
    BLOCKID block;
    BLOCK cube;
    float3 pos_;
    chunkBoundingBox();
        for(int i = 0; i < static_cast<int>(chunkSize.z); i++){
            for(int j = 0; j < static_cast<int>(chunkSize.x); j++){
                for(int k = 0; k < static_cast<int>(chunkSize.y); k++){
                    float noise = fractalPerlin2D(j,i,0.01f,5,0.5f,seed);
                    int height = static_cast<int>((noise +1.0f) * 14);
                    if(k == height) block = GRASS;
                    //else if(k >= 2) block = GRASS;
                    else if(k > height) block = AIR;
                    else if(k < height) {
                        block = DIRT;
                    }
                    if(block != AIR){
                        pos_ = {static_cast<float>(j),static_cast<float>(k),static_cast<float>(i)};
                        cube.position = pos_;
                        cube.World = XMMatrixTranslation(pos_.x + (position.u *16.0f),pos_.y ,pos_.z + (position.v *16.0f));
                        blocks[j][k][i] = block;
                        cube.boxMin = pos_;
                        cube.boxMax = {pos_.x +1.0f, pos_.y +1.0f, pos_.z + 1.0f};
                        cube.id = block;
                        cubes.push_back(cube);
                        size += 1;
                    }else{
                        blocks[j][k][i] = AIR;
                    }
                }
            }
        }
        chunkVisibleBlocks(texture);
        chunkUpdated = true;
}

void CHUNK::updateChunk(INPUT_ *input, CAMERA *camera, UINT width, UINT height, TEXTURE *texture){
    POINT point = input->mouse_position(NULL);
    MOUSEINPUT_ mInput = input->mouse_input();
    //if()
    if(mInput == LEFT_UP){
        //variables
        float2 mouse_point;
        XMFLOAT4X4 projMat,invViewMat;
        XMMATRIX IVM,IWM;
        XMFLOAT3 camDir,rayDirection,rayOrigin,boxMin,boxMax;
        XMVECTOR dir,rayDir,rayO;
        float thit = 0.0f;
        int selectedIndex = -1;
        float closestThit = FLT_MAX;
        //mouse position adjusted in -1.0f to +1.0f
        mouse_point.u = ((2.0f *(float)point.x)/(float)width) - 1.0f;
        mouse_point.v = (((2.0f *(float)point.y)/(float)height) - 1.0f) * -1.0f;
        //adjusts the point
        XMStoreFloat4x4(&projMat,camera->Projection);
        mouse_point.u = mouse_point.u / projMat._11;
        mouse_point.v = mouse_point.v / projMat._22;
        //the inverse view matrix
        IVM = XMMatrixInverse(NULL,camera->View);
        XMStoreFloat4x4(&invViewMat,IVM);
        //direction of the ray
        camDir.x = (mouse_point.u * invViewMat._11) + (mouse_point.v * invViewMat._21) + invViewMat._31;
        camDir.y = (mouse_point.u * invViewMat._12) + (mouse_point.v * invViewMat._23) + invViewMat._32;
        camDir.z = (mouse_point.u * invViewMat._13) + (mouse_point.v * invViewMat._24) + invViewMat._33;
        dir = XMLoadFloat3(&camDir);
        for(int i = 0; i < cubes.size(); i++){
            //inverse object position
            IWM = XMMatrixInverse(NULL,cubes[i].World);
            //calculate origin and direction of the ray
            rayO = XMVector3TransformCoord(camera->position,IWM);
            rayDir = XMVector3TransformNormal(dir,IWM);
            //normalize the ray direction
            rayDir = XMVector3Normalize(rayDir);
            //convert vector to float3 directx
            XMStoreFloat3(&rayOrigin, rayO);
            XMStoreFloat3(&rayDirection, rayDir);
            boxMin.x = cubes[i].boxMin.x;
            boxMin.y = cubes[i].boxMin.y;
            boxMin.z = cubes[i].boxMin.z;
            boxMax.x = cubes[i].boxMax.x;
            boxMax.y = cubes[i].boxMax.y;
            boxMax.z = cubes[i].boxMax.z;
            //intersect
           if(rayIntersectCube(rayOrigin, rayDirection, boxMin, boxMax, thit)) {
              if(thit < closestThit && thit >= 0.0f) {
                closestThit = thit;
                selectedIndex = i;
              }
           }
       }

       if(selectedIndex != -1) {
        printf("Cube %d selected at t = %f\n", selectedIndex, closestThit);
        cubes[selectedIndex].id = AIR;
       }
      
       chunkVisibleBlocks(texture);
       chunkUpdated = true;
    }
    
}

void CHUNK::chunkBoundingBox(){
    float3 world = {static_cast<float>(position.u*chunkSize.x),0.0f,static_cast<float>(position.v*chunkSize.z)};

    XMFLOAT3 extent(world.x + chunkSize.x,world.y + chunkSize.y, world.z + chunkSize.z);

    XMFLOAT3 center(world.x*0.5f,0,world.z*0.5f);

    box.Center = center;
    box.Extents = extent;
}

void CHUNK::chunkVisibleBlocks(TEXTURE *texture){
    float3 neighbour;
    VERTEX vertex[4];
    float x_dir[6] = {0.0f,0.0f,0.0f,0.0f,-1.0f,1.0f};
    float y_dir[6] = {0.0f,0.0f,1.0f,-1.0f,0.0f,0.0f};
    float z_dir[6] = {-1.0f,1.0f,0.0f,0.0f,0.0f,0.0f};
    for(int i = 0; i < size; i++){
        for(int j = 0; j < 6; j++){
            neighbour.x = cubes[i].position.x + x_dir[j];
            neighbour.y = cubes[i].position.y + y_dir[j];
            neighbour.z = cubes[i].position.z + z_dir[j];

            if(/*(neighbour.x < 0 || neighbour.y < 0 || neighbour.z < 0 ||
              neighbour.x >= chunkSize.x || neighbour.y >= chunkSize.y || neighbour.z >= chunkSize.z)
              ||*/ (blocks[static_cast<int>(neighbour.x)][static_cast<int>(neighbour.y)][static_cast<int>(neighbour.z)] == AIR)){
                if(cubes[i].id == AIR) blocks[static_cast<int>(cubes[i].position.x)][static_cast<int>(cubes[i].position.y)][static_cast<int>(cubes[i].position.z)] = AIR;
                else{
                    WorldsToRender.push_back(cubes[i].World);
                  getFace(blocks[static_cast<int>(cubes[i].position.x)][static_cast<int>(cubes[i].position.y)][static_cast<int>(cubes[i].position.z)],texture,j,vertex);
                  for(int k = 0; k < 6; k++){
                    if(k < 4){
                        XMVECTOR vector = XMVectorSet(vertex[k].position.x,vertex[k].position.y,vertex[k].position.z,0.0f);
                        vector = XMVector3TransformCoord(vector,WorldsToRender.back());
                        vertex[k].position.x = XMVectorGetX(vector);
                        vertex[k].position.y = XMVectorGetY(vector);
                        vertex[k].position.z = XMVectorGetZ(vector);
                        faces.push_back(vertex[k]);
                    }
                   }
                }
                  
            }
        }
    }
    //printf("%i",size);
}

void CHUNKMANAGER::initChunks(TEXTURE *texture){
    CHUNK chunk;
    int x[9] = {0,1,-1,0,1,-1,0,1,-1};
    int z[9] = {0,0,0,1,1,1,1,1,1};

   for(int i = 0; i < 1; i++){
    chunk.position = {0.0f,static_cast<float>(i)};//{static_cast<float>(x[i]),static_cast<float>(z[i])};
    chunk.initChunk(1337+i,texture);
    chunks.push_back(chunk);
    toRender.push_back(chunk);
   }
}

void CHUNKMANAGER::frustumCulling(CAMERA *camera){
    BoundingFrustum frustum;
    BoundingFrustum::CreateFromMatrix(frustum,camera->Projection);

    XMMATRIX VP = camera->View * camera->Projection;
    XMMATRIX IView = XMMatrixInverse(NULL,camera->View);
    frustum.Transform(frustum,IView);

    toRender.clear();
    for(auto chunk: chunks){
        if(frustum.Intersects(chunk.box)){
            toRender.push_back(chunk);
        }
    }
}

void CHUNKMANAGER::distanceCulling(CAMERA *camera){
    printf("%i\n",toRender.size());
    toRender.clear();
    for(auto chunk: chunks){
       int x = static_cast<int>(floor(XMVectorGetX(camera->position)/16));
       int z = static_cast<int>(floor(XMVectorGetZ(camera->position)/16));
       int dx = chunk.position.u - x;
       int dz = chunk.position.v - z;

       printf("%i,%i,%i,%i,%i,%i\n",x,z,chunk.position.u,chunk.position.v,dx,dz);
       if(/*dx < 5 ||*/ dz < 2 && dz > 0){
        toRender.push_back(chunk);
       }
    }
    toRender[currentChunk].chunkUpdated = true;
}

int CHUNKMANAGER::toRenderChunks(){
    int indices_list[6] = {0,1,2,0,2,3};
    int offset = -4;

    if(!toRender[currentChunk].chunkUpdated) return 0;

    for(int i = 0; i < toRender.size(); i++){
        for(int k = 0; k < toRender[i].faces.size(); k++){
          faces.push_back(toRender[i].faces[k]);

        }
        for(int j = 0; j < (toRender[i].faces.size() / 4) * 6; j++){
           if(j%6 == 0) offset += 4;
           indices.push_back(indices_list[j%6] + offset);
        }
    }

    //for(int i = 0; i < 4; i++)printf("%f,%f,%f\n",faces[i].position.x,faces[i].position.y,faces[i].position.z);
    //for(int i = 0; i < 6; i++)printf("%i\n",indices[i]);
    printf("here");

    return 1;

    toRender[currentChunk].chunkUpdated = false;
}

bool rayIntersectCube(const XMFLOAT3 &rayO, const XMFLOAT3 &rayDir, const XMFLOAT3 &boxMin, const XMFLOAT3 &boxMax, float &tminOut){
    float tmin = 0.0f;
    float tmax = FLT_MAX;

    for (int i = 0; i < 3; ++i)
    {
        float origin = *((&rayO.x) + i);
        float dir = *((&rayDir.x) + i);
        float minB = *((&boxMin.x) + i);
        float maxB = *((&boxMax.x) + i);

        if (fabs(dir) < 1e-8f)
        {
            if (origin < minB || origin > maxB)
                return false;
        }
        else
        {
            float ood = 1.0f / dir;
            float t1 = (minB - origin) * ood;
            float t2 = (maxB - origin) * ood;
            if (t1 > t2) std::swap(t1, t2);
            tmin = std::max(tmin, t1);
            tmax = std::min(tmax, t2);
            if (tmin > tmax)
                return false;
        }
    }

    tminOut = tmin;
    return true;
}