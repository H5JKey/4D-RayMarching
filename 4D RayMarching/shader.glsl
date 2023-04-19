

uniform vec2 u_resolution;
uniform vec2 u_mouse;
uniform vec3 ro;
uniform vec3 lookAt;
uniform float u_time;
uniform int count;
uniform vec2 size;
uniform float posX;
uniform float posY;
uniform float posZ;
uniform float posW;
uniform float rotateXY;
uniform float rotateXZ;
uniform float rotateXW;
uniform float rotateYZ;
uniform float rotateYW;
uniform float rotateZW;
uniform float id;
uniform vec3 color;




#define MAX_STEPS 1000
#define MAX_DIST 1000.
#define SURF_DIST .01

float ID=0.0;




#define MAX_STEPS 1000
#define MAX_DIST 1000.
#define SURF_DIST .01
#define planeID 2.0
#define sphereID 1.0
#define cubeID 3.0


void pR(inout vec2 p, float a) {
    p=cos(a)*p+sin(a)*vec2(p.y, -p.x);
}


float duoCylinderDist(vec4 p)
{
  vec4 pos=vec4(posX,posY,posZ,posW);
  p-=pos;
  vec2 d = abs(vec2(length(p.xz),length(p.yw)))-size;
  return min(max(d.x,d.y),0.0)+length(max(d,0.));
 }

float cubeDist(vec4 p) {
   vec4 pos=vec4(posX,posY,posZ,posW);
   return max(abs(p.x-pos.x), max(abs(p.y-pos.y), max(abs(p.z-pos.z),abs(p.w-pos.w))))-size.x/2.0;
}

float torusDist(vec4 p)
{

  vec4 pos=vec4(posX,posY,posZ,posW);
  p-=pos;
  return length(vec3(abs(length(p.xy)-size.x),p.zw))-size.y;
}


float sphereDist(vec4 p)
{

  vec4 pos=vec4(posX,posY,posZ,posW);
  return length(pos-p)-size.x;
}



float GetDist(vec3 p) {
    float dist;
    vec4 p4=vec4(p,0.0);
    pR(p4.xy,rotateXY);
    pR(p4.xz,rotateXZ);
    pR(p4.xw,rotateXW);
    pR(p4.yz,rotateYZ);
    pR(p4.yw,rotateYW);
    pR(p4.zw,rotateZW);
    if (id==1.0)
         dist=sphereDist(p4);
    else if (id==0.0)
         dist=cubeDist(p4);
    else if (id==2.0)
        dist=duoCylinderDist(p4);
    else if (id==3.0)
        dist=torusDist(p4);
         
    return dist;
}

float RayMarch(vec3 ro, vec3 rd) {
	float dO=0.;
    
    for(int i=0; i<MAX_STEPS; i++) {
    	vec3 p = ro + rd*dO;
        float dS = GetDist(p);
        dO += dS;
        if(dO>MAX_DIST || dS<SURF_DIST) break;
    }
    
    return dO;
}

vec3 GetNormal(vec3 p) {
	float d = GetDist(p);
    vec2 e = vec2(.01, 0);
    
    vec3 n = d - vec3(
        GetDist(p-e.xyy),
        GetDist(p-e.yxy),
        GetDist(p-e.yyx));
    
    return normalize(n);
}


float getAmbientOcclusion(vec3 p, vec3 normal) {
   float occ=0.0;
   float weight=1.0;
   for (int i=0; i<8; i++) {
      float len=0.01+0.02*float(i*i);
      float dist=GetDist(p);
      occ+=(len-dist)*weight;
      weight*=0.85;
    }
    return 1.0-clamp(0.6*occ, 0.0, 1.0);
}


vec3 GetLight(vec3 p, vec3 rd, vec3 color) {
    vec3 lightPos;
    lightPos.xyz= vec3(3.0, 7.0, 3.0);//Координаты источника света
    vec3 l = normalize(lightPos-p);
    vec3 n = GetNormal(p);
    vec3 r= reflect(-l,n);
    vec3 v=-rd;
    
    vec3 dif = 0.9*color*clamp(dot(n, l), 0.0, 1.0);
    vec3 specular=vec3(0.6, 0.5, 0.4)*pow(clamp(dot(r,v),0.0, 1.0), 10.0);
    vec3 diffuse=0.9*color*clamp(dot(l,n),0.0,1.0);
    vec3 ambient =0.05*color;
    vec3 fresnel=0.15*color*pow(1.0+dot(rd,n),3.0);
    float d = RayMarch(p+n*SURF_DIST*2., l);
    if(d<length(lightPos-p)) return color*0.05;
    float occ=getAmbientOcclusion(p,n);
    
    return dif+ambient*occ+fresnel*occ+specular*occ;
    
}


mat3 getCam (vec3 ro, vec3 lookAt) {
    vec3 camF = normalize(vec3(lookAt-ro));
    vec3 camR = normalize (cross(vec3(0, 1, 0), camF));
    vec3 camU = cross ( camF, camR);
    return mat3(camR, camU, camF);
}




void mouse(inout vec3 ro) {
    vec2 m = u_mouse.xy / u_resolution.xy;
    pR(ro.yz, m.y*3.14*0.5 - 0.5);
    pR(ro.xz, m.x * 2.0 * 3.14);
}



void main() {
	vec2 uv = (2.0*gl_FragCoord.xy-u_resolution.xy)/u_resolution.y;

    vec3 col = vec3(0.0);
    
    
    vec3 background=vec3(0.5, 0.8, 0.9);

    vec3 rd = getCam(ro, lookAt)*normalize(vec3(uv, 1.0));

    float d = RayMarch(ro, rd);
    if (d<MAX_DIST) {
        vec3 p = ro + rd * d;
    
    
        vec3 dif;
        dif = GetLight(p,rd,color);
        col = vec3(dif);
        col= mix(col, background, 1.0-exp(-0.0005*d*d));
     } else {
        col=vec3(background);
     }
    
    col = pow(col, vec3(0.4545));	// gamma correction
    
    gl_FragColor = vec4(col,1.0);
}