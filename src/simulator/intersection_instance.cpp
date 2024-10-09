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

void IntersectionInstance::intersect() {
    // TODO a temporary implementation for pairs of supported geometrical shapes
    if( ((obj1_.getShape().getType() ) == "Sphere") && (( obj1_.getShape().getType() ) == "Sphere") ){
    

      double d = sqrt(pow(obj2_.getPosition().x - obj1_.getPosition().x, 2) + 
                  pow(obj2_.getPosition().y - obj1_.getPosition().y, 2) + 
                  pow(obj2_.getPosition().z - obj1_.getPosition().z, 2));

      double v_s1 = (4.0 / 3.0) * M_PI * pow(obj1_.getShape().getRadius(), 3);
      double v_s2 = (4.0 / 3.0) * M_PI * pow(obj2_.getShape().getRadius(), 3);
      
      // Check for no intersection
      if (obj1_.getShape().getRadius() + obj2_.getShape().getRadius() <= d) {return;}

      // // Check if one sphere is completely inside the other
      // if (d <= std::fabs(obj1_.getShape().getRadius() - obj2_.getShape().getRadius())) {
      //   std::min(v_s1, v_s2)
      //   return;
      // }

      // Calculate intersection
      // Heights of spherical caps
      double h1 = std::fabs(obj1_.getShape().getRadius() - std::fabs((pow(obj2_.getShape().getRadius(), 2) + pow(d, 2) - pow(obj2_.getShape().getRadius(), 2)) / (2.0 * d)));
      double h2 = std::fabs(obj2_.getShape().getRadius() - std::fabs((pow(obj2_.getShape().getRadius(), 2) + pow(d, 2) - pow(obj1_.getShape().getRadius(), 2)) / (2.0 * d)));

      // Volumes of spherical caps
      double v_cap1 = (M_PI / 3.0) * (pow(h1, 2) * (3.0 * obj1_.getShape().getRadius() - h1));
      double v_cap2 = (M_PI / 3.0) * (pow(h2, 2) * (3.0 * obj2_.getShape().getRadius() - h2));

      double threshold = std::min(0.01 * v_s1 , 0.01 * v_s2);
  
      if((v_cap1 + v_cap2) < threshold) {return;}
      // we need an upper threshold which helps us determine whether an intersection is infinitesimal or not -> LET's Talk about it
      // ( v_cap1 + v_cap2 ) : amount of intersection
      doesIntersect_ = true;
      isInfinitesimal_= true;

} 

}}// namespace rosa