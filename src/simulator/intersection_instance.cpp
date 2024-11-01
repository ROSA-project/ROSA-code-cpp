#include "intersection_instance.hpp"
#include <iostream>
#include <cmath>  
#include <algorithm>
#include "sphere.hpp"


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

        if ( (sphere1.getRadius() + sphere2.getRadius()) <= d) {return 0.0;}

        // Check if one sphere is completely inside the other
        double v_s1 = (4.0 / 3.0) * M_PI * pow(sphere1.getRadius() , 3);
        double v_s2 = (4.0 / 3.0) * M_PI * pow(sphere2.getRadius() , 3);

        if (d <= std::fabs(sphere1.getRadius() - sphere2.getRadius())) {
            doesIntersect_ = true;
            return std::min(v_s1, v_s2);
            } 

        double h1 = std::fabs(sphere1.getRadius() - std::fabs((pow(sphere1.getRadius(), 2) + pow(d, 2) - pow(sphere2.getRadius(), 2)) / (2.0 * d)));
        double h2 = std::fabs(sphere2.getRadius() - std::fabs((pow(sphere2.getRadius(), 2) + pow(d, 2) - pow(sphere1.getRadius(), 2)) / (2.0 * d)));   

        double v_cap1 = (M_PI / 3.0) * (pow(h1, 2) * (3.0 * sphere1.getRadius() - h1));
        double v_cap2 = (M_PI / 3.0) * (pow(h2, 2) * (3.0 * sphere2.getRadius() - h2));  

        double threshold = std::min(0.01 * v_s1 , 0.01 * v_s2);

        if((v_cap1 + v_cap2) < threshold) {return 0.0;}

        doesIntersect_ = true;
        isInfinitesimal_= true;
        return (v_cap1 + v_cap2 );      
    }

}



}// namespace rosa