#pragma once
#include "geometry.hpp"

class Sphere: public Geometry {
public:
    double R;
    Sphere(){};
    Sphere(Vector c, double r,Vector rho,Motion m,bool _mirror=false,double n=-100.,bool _light=false){ 
        this->C = c;
        this->albedo = rho * (1./255.);
        this->refract_ind = n; // -100 means no refraction
        this->mirror = _mirror;
        this->light = _light;
        this->motion = m;
        this->R = r;
        };
    Intersection intersect(Ray r){
        //treat motion
        Vector moved_C = C+ r.get_time() * motion.speed; //we can do more complex stuff here
        //to put like before just replace by C
        Vector u = r.getu();
        Vector O = r.getO();
        double t = dot(u,moved_C-O);
        double delta = square(t) - ( (O-moved_C).norm_squared() - square(R));

        Intersection inter;
        
        if (delta<0){
            inter.intersects = false;
            return inter;
        }
        inter.P = O;
        if(delta==0){
            inter.length = t;
            inter.P +=t*u;}

        if(delta>0){
            double t1 = t - sqrt(delta);
            double t2 = t + sqrt(delta);
            if(t2<0){
                inter.intersects = false;
                return inter;
                }
            else{
                if (t1>=0){ 
                    inter.length = t1;
                    inter.P  += t1*u;}
                else     { 
                    inter.length = t2;
                    inter.P  += t2*u;};
                }
        }
        inter.intersects = true;
        inter.N = inter.P - moved_C;
        inter.N.normalize();
        inter.albedo = albedo; // initialize albedo
        inter.id = id;
        inter.incoming_direction = u;
        return inter;
    };
};