
__constant float4 bg_clr = (float4)(1.0f,1.0f,1.0f,1.0f);

float Get_Angle(float3 A, float3 B){
    if(length(A) == 0 || length(B) == 0){return 0.0f;}
    float cth = dot(A,B) / (length(A)*length(B));
    return acos(cth);
}


float3 Set_Angles(float3 N_hat){
    float3 ang;
    float3 z_hat = (float3)(0.0f, 0.0f, 1.0f);
    float3 y_hat = (float3)(0.0f, 1.0f, 0.0f);
    float3 xz = normalize((float3)(N_hat.x, 0.0f, N_hat.z));
    
    
    float phi = Get_Angle(xz, z_hat);
    if(N_hat.x < 0.0f){ang.y = -phi;}
    else{ang.y = phi;}
    ang.x = Get_Angle(N_hat, y_hat);
    ang.z = 0.0f;
    return ang;
    
}

float3 Rotate(float3 ang, float3 dir){
    float a = ang.x;
    float b = ang.y;
    float c = ang.z;
    

    
    //float3 r0 = (float3)(cos(c)*cos(b),cos(c)*sin(b)*sin(a) - sin(c)*cos(a) , cos(c)*sin(b)*cos(a)+sin(c)*sin(a));
    //float3 r1 = (float3)(sin(c)*cos(b), sin(c)*sin(b)*sin(a) + cos(c)*cos(a), sin(c)*sin(b)*cos(a)-cos(c)*sin(a));
    //float3 r2 = (float3)(-sin(b), cos(b)*sin(a), cos(b)*cos(a));
    
    float3 r0 = (float3)(cos(b),sin(b)*sin(a), sin(b)*cos(a));
    float3 r1 = (float3)(sin(c),cos(a), -sin(a));
    float3 r2 = (float3)(-sin(b), cos(b)*sin(a), cos(b)*cos(a));
    
    float x = dot(r0,dir);
    float y = dot(r1,dir);
    float z = dot(r2,dir);
    return (float3)(x,y,z);
}



struct sphere {
    float rad;
    float3 pos;
    
}__attribute__((aligned));

struct Ray {
    bool has_hit;
    bool comp_amb;
    int num_bounces;
    float t;
    float3 pos;
    float3 dir;
    float4 hit_clr;
    float3 reflect;
    float3 normal;
    float3 HP;
};

struct Ray Init_Ray(float3 _pos, float3 _dir, float4 bg){
   struct Ray ret;
   ret.has_hit = false;
   ret.comp_amb = false;
   ret.num_bounces = 0;
   ret.t = 1000000.0f;
   ret.pos = _pos;
   ret.dir = normalize(_dir);
   ret.hit_clr = bg;
   return ret;
}


struct Hemisphere {
    int num_vecs;
    float3 vecs[65];
};


struct Vertex{
    float3 pos;
    float3 norm;
};

struct Hemisphere Gen_Hemi(float3 norm){
    struct Hemisphere ret;
    ret.num_vecs = 65;
    float rad = 1.0f;
    int num_theta = 4;
    int num_phi = 16;
    
    float phi = 0.0f;
    float x, y, z;
    float dth = 3.14159f/(2.0f*num_theta);
    float theta = dth;
    int count = 0;
    
    //rotation
    float3 tmp_vec;
    float3 ang = Set_Angles(norm);
    tmp_vec = (float3)(0.0f,rad,0.0f);
    ret.vecs[count] = Rotate(ang, tmp_vec);
    count++;
    for(int i = 0; i<num_theta; i++){
        for (int j = 0; j<num_phi; j++){
            phi = j*dth;
            x = -rad*cos(phi)*sin(theta);
            z = rad*sin(phi)*sin(theta);
            y = rad*cos(theta);
            tmp_vec = (float3)(x,y,z);
            ret.vecs[count] = Rotate(ang, tmp_vec);
            //printf("x = %.2f y = %.2f z = %.2f\n", x, z, y);
            count++;
        }
        
        theta+=0.90f*dth;
            }
    return ret;
}








bool Hit_Sphere(struct Ray* ray, struct sphere sp){
    float3 V1 = normalize(ray->dir);
    float3 V2 = sp.pos - ray->pos;
    float b = dot(V1,V2);
    if(b<0.0f){return false;}
    float c = dot(V2,V2) - sp.rad*sp.rad;
    float disc = b*b - c;
    float dist;
    if(disc > 0.0f){
        dist = b - sqrt(disc);
        if(dist > ray->t ){return false;}
        else{
            ray->has_hit = true;
            ray->comp_amb = true;
            ray->num_bounces +=1;
            ray->t = dist;
            return true;
            }
    }
    else{
        return false;
    }
}


int Trace_Sphere_Only(struct Ray* ray, __global struct sphere* sps, int num_sps){
    
    bool hit = false;
    for(int i = 0; i < num_sps; i++){
            hit = Hit_Sphere(ray,sps[i]);
        }
    return 0;
}


float Compute_AO(struct Ray* ray, __global struct sphere* sps, int num_sps){
    
    
    float3 hp = ray->pos + 0.15f*ray->dir;
    struct Ray new_ray = Init_Ray(hp, ray->dir, bg_clr);
    float mod = 1.0f;
    float3 new_dir;
    float delta = 0.0f;
    float tot = 0.0f;
    struct Hemisphere hemi = Gen_Hemi(ray->dir);
    int num_hs =  hemi.num_vecs;
    float Hit_Max = 7.5f;
    for(int i = 0; i<num_hs;i++){
        new_dir =  hemi.vecs[i];
        new_ray.dir = normalize(new_dir);
        new_ray.t = 100000.0f;
        Trace_Sphere_Only(&new_ray, sps, num_sps);
        delta = (new_ray.t > Hit_Max) ? Hit_Max : new_ray.t;
        tot += delta;
        
        
    }
    float max_val = Hit_Max * num_hs;
    mod =  tot / max_val;
    return mod;
    
}

__kernel void Comp_Ambient_Occlusion(__global struct sphere* sps,const int num_sps,__global float* output, __global struct Vertex* verts){
    
    
    int i = get_global_id(0);
    struct Ray ray = Init_Ray(verts[i].pos, verts[i].norm, bg_clr);
    float amb = Compute_AO(&ray, sps, 67);
    output[i] = amb;
    
        
        
    
}







