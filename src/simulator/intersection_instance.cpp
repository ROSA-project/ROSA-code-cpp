#include "intersection_instance.hpp"
#include <iostream>
#include <cmath>  
#include <algorithm>
#include <vector>
#include <functional>
#include <utility>
#include "sphere.hpp"
#define _USE_MATH_DEFINES

namespace rosa {

IntersectionInstance::IntersectionInstance(const Object& obj1, const Object& obj2)
    : obj1_(obj1)
    , obj2_(obj2) {

    // TODO: to silence the "unused variables" warnings.
    // (void)obj1_;
    // (void)obj2_;

    intersect();
}

double IntersectionInstance::intersect() {
    // TODO a temporary implementation for pairs of supported geometrical shapes
    if( (obj1_.getShape().getType() ) == "Sphere" && ( obj2_.getShape().getType() ) == "Sphere") {
        const Sphere& sphere1 = static_cast<const Sphere&>(obj1_.getShape());
        const Sphere& sphere2 = static_cast<const Sphere&>(obj2_.getShape());

        double d = sqrt(pow(obj2_.getPosition().x - obj1_.getPosition().x, 2) + 
                        pow(obj2_.getPosition().y - obj1_.getPosition().y, 2) + 
                        pow(obj2_.getPosition().z - obj1_.getPosition().z, 2));   

        if ( (sphere1.getRadius() + sphere2.getRadius()) <= d) {
            return 0.0;
            }

        // Check if one sphere is completely inside the other
        double v_s1 = (4.0 / 3.0) * M_PI * (pow(sphere1.getRadius() , 3));
        double v_s2 = (4.0 / 3.0) * M_PI * (pow(sphere2.getRadius() , 3));

        if (d <= std::fabs(sphere1.getRadius() - sphere2.getRadius())) {
            doesIntersect_ = true;
            return std::min(v_s1, v_s2);
            } 

        double h1 = std::fabs(sphere1.getRadius() - std::fabs((pow(sphere1.getRadius(), 2) + pow(d, 2) - pow(sphere2.getRadius(), 2)) / (2.0 * d)));
        double h2 = std::fabs(sphere2.getRadius() - std::fabs((pow(sphere2.getRadius(), 2) + pow(d, 2) - pow(sphere1.getRadius(), 2)) / (2.0 * d)));   

        double v_cap1 = (M_PI / 3.0) * (pow(h1, 2) * (3.0 * sphere1.getRadius() - h1));
        double v_cap2 = (M_PI / 3.0) * (pow(h2, 2) * (3.0 * sphere2.getRadius() - h2));  

        double threshold = std::min(0.015 * v_s1 , 0.015 * v_s2);

        if((v_cap1 + v_cap2) < threshold) {return 0.0;}

        doesIntersect_ = true;
        isInfinitesimal_= true;
        return (v_cap1 + v_cap2 );      
    }

    return 0.0;
}

std::pair<std::vector<double>, std::vector<double>> IntersectionInstance::reversion(){
    std::vector<double> revert_Vec_1(3 , 0);
    std::vector<double> revert_Vec_2(3 , 0);
 if( (obj1_.getShape().getType() ) == "Sphere" && ( obj2_.getShape().getType() ) == "Sphere") {
    const Sphere& sphere1 = static_cast<const Sphere&>(obj1_.getShape());
    const Sphere& sphere2 = static_cast<const Sphere&>(obj2_.getShape());
    double d = sqrt(pow(obj2_.getPosition().x - obj1_.getPosition().x, 2) + 
                    pow(obj2_.getPosition().y - obj1_.getPosition().y, 2) + 
                    pow(obj2_.getPosition().z - obj1_.getPosition().z, 2));

    std::vector<double> d_vec_12 = {
        obj2_.getPosition().x - obj1_.getPosition().x
        ,obj2_.getPosition().y - obj1_.getPosition().y
        ,obj2_.getPosition().z - obj1_.getPosition().z };

    std::vector<double> direction_12(3, 0);
    //find the (point of intersection) (not reverted yet) between the 2 spheres.
    double coefficient_of_d = 0.5 + ((sphere1.getRadius()*sphere1.getRadius() - sphere2.getRadius()*sphere2.getRadius())/(2*(d*d)));
    //ci represents the center of intersection when the objects have a shared infinitesimal volume.
    double c1_ci = coefficient_of_d * d;
    double revert_1 = sphere1.getRadius() - c1_ci ; 

    double c2_ci = d - c1_ci;
    double revert_2 = sphere2.getRadius() - c2_ci ;

    for (int i=0 ; i < 3 ; i++) {
    direction_12[i] = d_vec_12[i] / d; //unit direction vector -> points in n

    // the first sphere always wants to move in the opposite dierction of n 
    // the second sphere always wants to move in the direction on n 
    revert_Vec_1[i] = revert_1 * -1 * direction_12[i];
    revert_Vec_2[i] = revert_2 * direction_12[i];
    };

    revert_Vec_1[3]=obj1_.getObjectId();
    revert_Vec_2[3]=obj2_.getObjectId();
    }
    return {revert_Vec_1, revert_Vec_2};
}


}// namespace rosa