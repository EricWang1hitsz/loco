/*!
* @file     BaseControlTest.cpp
* @author   Christian Gehring
* @date     Jun, 2013
* @version  1.0
* @ingroup
* @brief
*/


#include <gtest/gtest.h>

#include "loco/locomotion_controller/LocomotionControllerDynamicGait.hpp"
#include "loco/gait_pattern/GaitPatternAPS.hpp"
#include "loco/limb_coordinator/LimbCoordinatorDynamicGait.hpp"
#include "loco/foot_placement_strategy/FootPlacementStrategyInvertedPendulum.hpp"
#include "loco/torso_control/TorsoControlDynamicGait.hpp"

#include "loco/common/LegStarlETH.hpp"
#include "loco/common/TorsoStarlETH.hpp"

#include "RobotModel.hpp"
#include "TerrainPlane.hpp"


TEST(LocomotionControllerTest, test) {
  double dt = 0.0025;


  robotTerrain::TerrainPlane terrain;
  robotModel::RobotModel robotModel;
  robotModel.init();
  robotModel.update();



  loco::LegGroup legs;
  loco::LegStarlETH leftForeLeg("leftFore");
  loco::LegStarlETH rightForeLeg("rightFore");
  loco::LegStarlETH leftHindLeg("leftHind");
  loco::LegStarlETH rightHindLeg("rightHind");
  legs.addLeg(&leftForeLeg);
  legs.addLeg(&rightForeLeg);
  legs.addLeg(&leftHindLeg);
  legs.addLeg(&rightHindLeg);

  loco::TorsoStarlETH torso;


  loco::APS aps(0.8, 0.8, 0.5, 0.5, 0.5, 0.5, 0.5);
  loco::GaitPatternAPS gaitPatternAPS;
  gaitPatternAPS.initialize(aps, dt);

  loco::LimbCoordinatorDynamicGait limbCoordinator(&legs, &gaitPatternAPS);
  loco::FootPlacementStrategyInvertedPendulum footPlacementStrategy(&legs, &torso, &robotModel, &terrain);
  loco::BaseControlDynamicGait baseControl;

  loco::LocomotionControllerDynamicGait locomotionController(&legs, &torso, &robotModel, &terrain, &limbCoordinator, &footPlacementStrategy, &baseControl);
  locomotionController.advance(dt);

}
