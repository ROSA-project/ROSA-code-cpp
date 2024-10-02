#include <gtest/gtest.h>
#include "world.hpp"
#include "position.hpp"
#include <cmath>

// Demonstrate some basic assertions.
TEST(FreeSpaceMotionSingleBall, NoTorqueAxisAligned) {
  rosa::WorldConfig w_config;
  w_config.simDurationMsec = 1000; 
  w_config.visFrameIntervalMsec = 250;  // 25 msec
  rosa::World world("../sample_maps/single_ball.json", "e2e-test-freespace-ball", w_config);
  world.run();
  rosa::ObjectId objID = world.getTesterID(0);
  rosa::Position pos = world.getObjectPosition(objID);  
  rosa::Position targetPoint(1,0,0,0,0,0,0);
  std::cout << pos.x << ", " << pos.y << ", " << pos.z << std::endl;
  EXPECT_LE(pos.distance(targetPoint), 0.001);
  

  // Expect two strings not to be equal.
  
  EXPECT_STRNE("hello", "world");
  // Expect equality.
  EXPECT_EQ(7 * 6, 42);
}   