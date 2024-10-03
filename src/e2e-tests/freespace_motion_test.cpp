#include <gtest/gtest.h>
#include "world.hpp"
#include "position.hpp"
#include <cmath>

TEST(FreeSpaceMotionSingleBall, NoTorqueAxisAligned) {
  rosa::WorldConfig w_config;
  w_config.simDurationMsec = 1000; 
  w_config.visFrameIntervalMsec = 250;  // 25 msec

  rosa::World world("../sample_maps/single_ball.json", "e2e-test-freespace-single-ball-1", w_config);
  world.run();

  rosa::ObjectId objID = world.getTesterID(0);
  rosa::Position pos = world.getObjectPosition(objID);  
  rosa::Position targetPoint(1,0,0,0,0,0,0);
  // TODO replace the epsilon
  EXPECT_LE(pos.distance(targetPoint), 0.001);
}

TEST(FreeSpaceMotionTwoBalls, NoTorqueAxisAligned) {
  rosa::WorldConfig w_config;
  w_config.simDurationMsec = 1000; 
  w_config.visFrameIntervalMsec = 250;  // 25 msec
  
  rosa::World world("../sample_maps/two_balls.json", "e2e-test-freespace-two-balls-1", w_config);
  world.run();

  rosa::ObjectId objID1 = world.getTesterID(0);
  rosa::Position pos1 = world.getObjectPosition(objID1);  
  rosa::Position targetPoint1(1,0,0,0,0,0,0);

  EXPECT_LE(pos1.distance(targetPoint1), 0.001);

  rosa::ObjectId objID2 = world.getTesterID(1);
  rosa::Position pos2 = world.getObjectPosition(objID2);  
  rosa::Position targetPoint2(11,11,11,0,0,0,0);

  EXPECT_LE(pos2.distance(targetPoint2), 0.001);
}