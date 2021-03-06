/*****************************************************************************************
* Software License Agreement (BSD License)
*
* Copyright (c) 2014, Christian Gehring, Péter Fankhauser, C. Dario Bellicoso, Stelian Coros
* All rights reserved.
*
*  Redistribution and use in source and binary forms, with or without
*  modification, are permitted provided that the following conditions
*  are met:
*
*   * Redistributions of source code must retain the above copyright
*     notice, this list of conditions and the following disclaimer.
*   * Redistributions in binary form must reproduce the above
*     copyright notice, this list of conditions and the following
*     disclaimer in the documentation and/or other materials provided
*     with the distribution.
*   * Neither the name of Autonomous Systems Lab nor ETH Zurich
*     nor the names of its contributors may be used to endorse or
*     promote products derived from this software without specific
*     prior written permission.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
*  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
*  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
*  FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
*  COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
*  INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
*  BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
*  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
*  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
*  LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
*  ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
*  POSSIBILITY OF SUCH DAMAGE.
*/
/*
 * VisualizerSC.cpp
 *
 *  Created on: Mar 14, 2014
 *      Author: gech
 */

#include "loco/visualizer/sc/VisualizerSC.hpp"
#include "AppGUI/Globals.h"

namespace loco {

VisualizerSC::VisualizerSC(int* drawCharacter) :
    VisualizerBase(),
 gaitPatternWindow_(nullptr),
 gaitPatternFlightPhasesWindow_(nullptr),
 isSimulationRunning_(Globals::animationRunning),
 desiredFrameRate_(Globals::desiredFrameRate),
 drawCharacter_(drawCharacter)
{

  gaitPatternWindow_ = new GaitPatternAPSPreview(0, 0, 450, 150);
  gaitPatternFlightPhasesWindow_ = new GaitPatternFlightPhasesPreview(0, 0, 450, 150);

  /* initialize foot trajectories */
  const double windowSize = 10.0; 2.0;
  const double dt = 1.0/desiredFrameRate_;
  for (int iLeg=0; iLeg<4; iLeg++) {
    for (double t=0; t<windowSize; t=t+dt) {
      footTrajectories_[iLeg].addKnot(t, loco::Position());
      desiredFootTrajectories_[iLeg].addKnot(t, loco::Position());
      predictedFootHoldTrajectories_[iLeg].addKnot(t, loco::Position());
      predictedFootHoldInvertedPendulumTrajectories_[iLeg].addKnot(t, loco::Position());
      predictedDefaultFootHoldTrajectories_[iLeg].addKnot(t, loco::Position());
    }
  }
  for (double t=0; t<windowSize; t=t+dt) {
    baseTrajectory_.addKnot(t, loco::Position());
  }


  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glEnable(GL_BLEND);

}
VisualizerSC::~VisualizerSC() {
  delete gaitPatternWindow_;
  delete gaitPatternFlightPhasesWindow_;
}

void VisualizerSC::addParameters() {

}

void VisualizerSC::drawHistoryOfFootPositions(loco::LegGroup* legs) {
  const double dt = 1.0/desiredFrameRate_;
  for (int iLeg=0; iLeg<4; iLeg++) {
    if (isSimulationRunning_) {
      const loco::LegBase* leg = legs->getLeg(iLeg);
      footTrajectories_[iLeg].removeKnot(0);
      footTrajectories_[iLeg].addKnot(footTrajectories_[iLeg].getKnotPosition(footTrajectories_[iLeg].getKnotCount()-1)+dt, leg->getPositionWorldToFootInWorldFrame());
    }
    drawTrajectoryCatMullRomPosition(footTrajectories_[iLeg], dt, 2.0);
  }
}

void VisualizerSC::drawHistoryOfDesiredFootPositions(loco::LegGroup* legs) {
  const double dt = 1.0/desiredFrameRate_;
  for (int iLeg=0; iLeg<4; iLeg++) {
    if (isSimulationRunning_) {
      const loco::LegBase* leg = legs->getLeg(iLeg);
      desiredFootTrajectories_[iLeg].removeKnot(0);
      desiredFootTrajectories_[iLeg].addKnot(desiredFootTrajectories_[iLeg].getKnotPosition(desiredFootTrajectories_[iLeg].getKnotCount()-1)+dt, leg->getDesiredWorldToFootPositionInWorldFrame());
    }
    drawTrajectoryCatMullRomPosition(desiredFootTrajectories_[iLeg], dt, 2.0);
  }
}

void VisualizerSC::drawdrawHistoryOfPredictedFootHolds(loco::FootPlacementStrategyInvertedPendulum* strategy) {
  const double dt = 1.0/desiredFrameRate_;
  GLUtilsKindr::glLColor(0.0, 153.0/255.0, 0.0, 1.0); // green

  for (int iLeg=0; iLeg<4; iLeg++) {
    if (isSimulationRunning_) {
      predictedFootHoldTrajectories_[iLeg].removeKnot(0);
      predictedFootHoldTrajectories_[iLeg].addKnot(predictedFootHoldTrajectories_[iLeg].getKnotPosition(predictedFootHoldTrajectories_[iLeg].getKnotCount()-1)+dt, strategy->positionWorldToFootHoldInWorldFrame_[iLeg]);
    }
    drawTrajectoryCatMullRomPosition(predictedFootHoldTrajectories_[iLeg], dt, 2.0);
  }

  GLUtilsKindr::glLColor(0.0, 0.0,  0.0, 1.0); // black
  for (int iLeg=0; iLeg<4; iLeg++) {
    if (isSimulationRunning_) {
      predictedFootHoldInvertedPendulumTrajectories_[iLeg].removeKnot(0);
      predictedFootHoldInvertedPendulumTrajectories_[iLeg].addKnot(predictedFootHoldInvertedPendulumTrajectories_[iLeg].getKnotPosition(predictedFootHoldInvertedPendulumTrajectories_[iLeg].getKnotCount()-1)+dt, strategy->positionWorldToFootHoldInvertedPendulumInWorldFrame_[iLeg]);
    }
    drawTrajectoryCatMullRomPosition(predictedFootHoldInvertedPendulumTrajectories_[iLeg], dt, 2.0);
  }

  GLUtilsKindr::glLColor(1.0, 128.0/255.0,  0.0, 1.0); // orange
  for (int iLeg=0; iLeg<4; iLeg++) {
    if (isSimulationRunning_) {
      predictedDefaultFootHoldTrajectories_[iLeg].removeKnot(0);
      predictedDefaultFootHoldTrajectories_[iLeg].addKnot(predictedDefaultFootHoldTrajectories_[iLeg].getKnotPosition(predictedDefaultFootHoldTrajectories_[iLeg].getKnotCount()-1)+dt, strategy->positionWorldToDefaultFootHoldInWorldFrame_[iLeg]);
    }
    drawTrajectoryCatMullRomPosition(predictedDefaultFootHoldTrajectories_[iLeg], dt, 2.0);
  }


}

void VisualizerSC::drawHistoryOfBasePosition(loco::TorsoBase* torso) {
  const double dt = 1.0/desiredFrameRate_;
  if (isSimulationRunning_) {
    baseTrajectory_.removeKnot(0);
    baseTrajectory_.addKnot(baseTrajectory_.getKnotPosition(baseTrajectory_.getKnotCount()-1)+dt, torso->getMeasuredState().getPositionWorldToBaseInWorldFrame());
  }
  drawTrajectoryCatMullRomPosition(baseTrajectory_, dt, 2.0);

}


void VisualizerSC::drawGaitPatternAPS(loco::GaitPatternAPS* gaitPattern, double stridePhase) {

    if (gaitPatternWindow_ != nullptr) {
      gaitPatternWindow_->gp = gaitPattern;
      gaitPatternWindow_->cursorPosition = stridePhase;
      gaitPatternWindow_->draw();
    }
}

void VisualizerSC::drawContactForces(AbstractRBEngine* world) {
  std::vector<ContactForce>* cfs = world->getContactForces();
  GLUtils::glLColor(1.0, 0, 0);
//  glColor4d(1.0, 0.0, 0.0, 1.0);
  for (uint i=0; i<cfs->size();i++) {
    GLUtils::drawArrow(-cfs->at(i).f/-500.0, cfs->at(i).cp, 0.005);
  }
}

void VisualizerSC::drawDesiredPose(Character* character, AbstractRBEngine* world, loco::TorsoBase* torso, loco::LegGroup* legs) {

  loco::RotationQuaternion  orientationWorldToBaseInWorldFrame;
  loco::Position positionWorldToBaseInWorldFrame;
  orientationWorldToBaseInWorldFrame = torso->getDesiredState().getOrientationControlToBase()*torso->getMeasuredState().getOrientationWorldToControl();
  positionWorldToBaseInWorldFrame = torso->getMeasuredState().getPositionWorldToControlInWorldFrame()
                                    + torso->getDesiredState().getPositionControlToBaseInControlFrame();

  VectorQj desJointPositions;
  int iLeg =0;
  for (auto leg : *legs) {
    desJointPositions.block<3,1>(iLeg*3,0) = leg->getDesiredJointPositions();
    iLeg++;
  }
  drawPose(character, world, positionWorldToBaseInWorldFrame, orientationWorldToBaseInWorldFrame, desJointPositions, SHOW_ABSTRACT_VIEW_DESIRED);
}

void VisualizerSC::drawMeasuredPose(Character* character, AbstractRBEngine* world,  loco::TorsoBase* torso, loco::LegGroup* legs) {

  loco::RotationQuaternion  orientationWorldToBaseInWorldFrame;
  loco::Position positionWorldToBaseInWorldFrame;
  orientationWorldToBaseInWorldFrame = torso->getMeasuredState().getOrientationWorldToBase();
  positionWorldToBaseInWorldFrame = torso->getMeasuredState().getPositionWorldToBaseInWorldFrame();
  VectorQj desJointPositions;
  int iLeg =0;
  for (auto leg : *legs) {
    desJointPositions.block<3,1>(iLeg*3,0) = leg->getMeasuredJointPositions();
    iLeg++;
  }

  drawPose(character, world, positionWorldToBaseInWorldFrame, orientationWorldToBaseInWorldFrame, desJointPositions, SHOW_ABSTRACT_VIEW_MEASURED);
}

void VisualizerSC::drawPose(Character* character, AbstractRBEngine* world, const loco::Position& positionWorldToBaseInWorldFrame, const loco::RotationQuaternion& orientationWorldToBaseInWorldFrame,  const VectorQj& desJointPositions, int drawFlags) {

  ReducedCharacterState desiredPose(character->getStateDimension());
  Quaternion quat(orientationWorldToBaseInWorldFrame.w(), orientationWorldToBaseInWorldFrame.x(), orientationWorldToBaseInWorldFrame.y(), orientationWorldToBaseInWorldFrame.z());
  //quat = Quaternion(0,0,0,1);
  desiredPose.setOrientation(quat);
  //desiredPose.setPosition(Point3d(0.2,0.2,0.2));


  const VectorQj desJointVelocities = VectorQj::Zero();
  setCharacterJointState(desiredPose, desJointPositions,desJointVelocities);
  desiredPose.setPosition(Point3d(positionWorldToBaseInWorldFrame.x(),positionWorldToBaseInWorldFrame.y(),positionWorldToBaseInWorldFrame.z()));
  drawPose(character, world, &desiredPose, drawFlags);
}

void VisualizerSC::drawDesiredVirtualForces(loco::TorsoBase* torso, loco::LegGroup* legs, loco::VirtualModelController* virtualModelController) {
  const Force forceInBaseFrame = virtualModelController->getDesiredVirtualForceInBaseFrame();
  const Torque torqueInBaseFrame  = virtualModelController->getDesiredVirtualTorqueInBaseFrame();
  glPushMatrix();
  GLUtilsKindr::glLColor(1.0, 0.0, 0.0, 1.0);
  drawForceAndTorqueInBaseFrame(forceInBaseFrame, torqueInBaseFrame, torso, legs);
  glPopMatrix();
}


void VisualizerSC::drawDistributedVirtualForces(loco::TorsoBase* torso, loco::LegGroup* legs, loco::VirtualModelController* virtualModelController) {
  Force netForceInBaseFrame;
  Torque netTorqueInBaseFrame;

  virtualModelController->getDistributedVirtualForceAndTorqueInBaseFrame(netForceInBaseFrame, netTorqueInBaseFrame);

  glPushMatrix();
  GLUtilsKindr::glLColor(0.0, 0.0, 1.0, 1.0);
  drawForceAndTorqueInBaseFrame(netForceInBaseFrame, netTorqueInBaseFrame, torso, legs);
  glPopMatrix();
}

void VisualizerSC::drawForceAndTorqueInBaseFrame(const Force& forceInBaseFrame, const Torque& torqueInBaseFrame, loco::TorsoBase* torso, loco::LegGroup* legs) {

  double forceScale = 1.0/100.0;

  const Force virtualForceInBaseFrame = forceInBaseFrame;
  const Torque virtualTorqueInBaseFrame = torqueInBaseFrame;
  const Position positionWorldToBaseInWorldFrame = torso->getMeasuredState().getPositionWorldToBaseInWorldFrame();

  const Force virtualForceInWorldFrame = forceScale*Force(torso->getMeasuredState().getOrientationWorldToBase().inverseRotate(virtualForceInBaseFrame.toImplementation()));

  // force

  GLUtilsKindr::drawArrow(GLUtilsKindr::Vector(virtualForceInWorldFrame), positionWorldToBaseInWorldFrame, 0.01, false);

  double lengthHipToHip = legs->getLeftForeLeg()->getPositionBaseToHipInBaseFrame().x()-legs->getLeftHindLeg()->getPositionBaseToHipInBaseFrame().x();
  const Force forceYawInBaseFrame(0.0, lengthHipToHip*virtualTorqueInBaseFrame.z(), 0.0);
  const Force forceYawInWorldFrame = forceScale*Force(torso->getMeasuredState().getOrientationWorldToBase().inverseRotate(forceYawInBaseFrame.toImplementation()));

  const Force forcePitchInBaseFrame(0.0, 0.0 , lengthHipToHip*virtualTorqueInBaseFrame.y());
  const Force forcePitchInWorldFrame = forceScale*Force(torso->getMeasuredState().getOrientationWorldToBase().inverseRotate(forcePitchInBaseFrame.toImplementation()));

  const Force forceRollInWorldBase(0.0, 0.0 , lengthHipToHip*virtualTorqueInBaseFrame.x());
  const Force forceRollInWorldFrame = forceScale*Force(torso->getMeasuredState().getOrientationWorldToBase().inverseRotate(forceRollInWorldBase.toImplementation()));

  const Position foreMidHipInWorldFrame =  Position((legs->getLeftForeLeg()->getPositionWorldToHipInWorldFrame()+ legs->getRightForeLeg()->getPositionWorldToHipInWorldFrame()).toImplementation()*0.5);
  const Position hindMidHipInWorldFrame =  Position((legs->getLeftHindLeg()->getPositionWorldToHipInWorldFrame()+ legs->getRightHindLeg()->getPositionWorldToHipInWorldFrame()).toImplementation()*0.5);
  const Position leftMidHipInWorldFrame =  Position( positionWorldToBaseInWorldFrame.x(), ((legs->getLeftHindLeg()->getPositionWorldToHipInWorldFrame()+ legs->getLeftForeLeg()->getPositionWorldToHipInWorldFrame()).toImplementation()).y()*0.5, positionWorldToBaseInWorldFrame.z());
  const Position rightMidHipInWorldFrame = Position( positionWorldToBaseInWorldFrame.x(), ((legs->getRightHindLeg()->getPositionWorldToHipInWorldFrame()+ legs->getRightForeLeg()->getPositionWorldToHipInWorldFrame()).toImplementation()).y()*0.5, positionWorldToBaseInWorldFrame.z());

  // yaw
  GLUtilsKindr::drawArrow(GLUtilsKindr::Vector(forceYawInWorldFrame), foreMidHipInWorldFrame, 0.01, false);
  GLUtilsKindr::drawArrow(GLUtilsKindr::Vector(-forceYawInWorldFrame), hindMidHipInWorldFrame, 0.01, false);
  // pitch
  GLUtilsKindr::drawArrow(GLUtilsKindr::Vector(-forcePitchInWorldFrame), foreMidHipInWorldFrame, 0.01, false);
  GLUtilsKindr::drawArrow(GLUtilsKindr::Vector(forcePitchInWorldFrame), hindMidHipInWorldFrame, 0.01, false);
  // roll
  GLUtilsKindr::drawArrow(GLUtilsKindr::Vector(forceRollInWorldFrame), leftMidHipInWorldFrame, 0.01, false);
  GLUtilsKindr::drawArrow(GLUtilsKindr::Vector(-forceRollInWorldFrame), rightMidHipInWorldFrame, 0.01, false);

}




void VisualizerSC::drawSupportPolygon(loco::LegGroup* legs, double lineWidth) {
  glLineWidth(lineWidth);

  if (legs->getLeftForeLeg()->isGrounded() && legs->getLeftHindLeg()->isGrounded()) {
    const loco::Position start = legs->getLeftForeLeg()->getPositionWorldToFootInWorldFrame();
    const loco::Position end = legs->getLeftHindLeg()->getPositionWorldToFootInWorldFrame();
    GLUtilsKindr::drawLine(start, end);
  }
  if (legs->getLeftForeLeg()->isGrounded() && legs->getRightHindLeg()->isGrounded()) {
    const loco::Position start = legs->getLeftForeLeg()->getPositionWorldToFootInWorldFrame();
    const loco::Position end = legs->getRightHindLeg()->getPositionWorldToFootInWorldFrame();
    GLUtilsKindr::drawLine(start, end);
  }
  if (legs->getLeftForeLeg()->isGrounded() && legs->getRightForeLeg()->isGrounded()) {
    const loco::Position start = legs->getLeftForeLeg()->getPositionWorldToFootInWorldFrame();
    const loco::Position end = legs->getRightForeLeg()->getPositionWorldToFootInWorldFrame();
    GLUtilsKindr::drawLine(start, end);
  }
  if (legs->getRightForeLeg()->isGrounded() && legs->getRightHindLeg()->isGrounded()) {
    const loco::Position start = legs->getRightForeLeg()->getPositionWorldToFootInWorldFrame();
    const loco::Position end = legs->getRightHindLeg()->getPositionWorldToFootInWorldFrame();
    GLUtilsKindr::drawLine(start, end);
  }
  if (legs->getRightForeLeg()->isGrounded() && legs->getLeftHindLeg()->isGrounded()) {
    const loco::Position start = legs->getRightForeLeg()->getPositionWorldToFootInWorldFrame();
    const loco::Position end = legs->getLeftHindLeg()->getPositionWorldToFootInWorldFrame();
    GLUtilsKindr::drawLine(start, end);
  }
  if (legs->getRightHindLeg()->isGrounded() && legs->getLeftHindLeg()->isGrounded()) {
    const loco::Position start = legs->getRightHindLeg()->getPositionWorldToFootInWorldFrame();
    const loco::Position end = legs->getLeftHindLeg()->getPositionWorldToFootInWorldFrame();
    GLUtilsKindr::drawLine(start, end);
  }
}




void VisualizerSC::drawPose(Character* character, AbstractRBEngine* world, ReducedCharacterState* desiredPose, int drawFlags) {
  glEnable(GL_LIGHTING);

  std::vector<double> worldState;
  worldState.clear();
  world->getState(&worldState);
  ReducedCharacterState rs(character->getStateDimension());
  character->populateState(&rs);
  rs.setPosition(desiredPose->getPosition() + Vector3d(0, 0.0, 0));
  rs.setOrientation(desiredPose->getOrientation());
  glColor3d(0.0,0.0,0.0);


  copyOrientation(&rs, desiredPose, character->getJointIndex("rfHipAA"));
  copyOrientation(&rs, desiredPose, character->getJointIndex("lfHipAA"));
  copyOrientation(&rs, desiredPose, character->getJointIndex("rrHipAA"));
  copyOrientation(&rs, desiredPose, character->getJointIndex("lrHipAA"));

  copyOrientation(&rs, desiredPose, character->getJointIndex("rfHipFE"));
  copyOrientation(&rs, desiredPose, character->getJointIndex("lfHipFE"));
  copyOrientation(&rs, desiredPose, character->getJointIndex("rrHipFE"));
  copyOrientation(&rs, desiredPose, character->getJointIndex("lrHipFE"));

  copyOrientation(&rs, desiredPose, character->getJointIndex("rfKneeFE"));
  copyOrientation(&rs, desiredPose, character->getJointIndex("lfKneeFE"));
  copyOrientation(&rs, desiredPose, character->getJointIndex("rrKneeFE"));
  copyOrientation(&rs, desiredPose, character->getJointIndex("lrKneeFE"));

  character->setState(&rs);
//  world->drawRBs(drawFlags);

  bool shadowMode = false;
  bool drawMesh = false;
//  int flags = (drawMesh)?(SHOW_MESH | SHOW_EYE_BLINK):(SHOW_ABSTRACT_VIEW | drawFlags /*| SHOW_BODY_FRAME | SHOW_ABSTRACT_VIEW*/);
  int flags = drawFlags;
    flags |= SHOW_CHARACTER;
//  flags = SHOW_ABSTRACT_VIEW;
  //if we are drawing shadows, we don't need to enable textures or lighting, since we only want the projection anyway
  glEnable(GL_LIGHTING);
  if (shadowMode == false){
    flags |= SHOW_CPS_IF_NO_MESH;
    flags |= SHOW_COLOURS/* | SHOW_JOINTS | SHOW_BODY_FRAME*/;
    glEnable(GL_LIGHTING);
    glDisable(GL_TEXTURE_2D);
    glEnable(GL_LIGHTING);

  }else
    glDisable(GL_LIGHTING);

  if (character == NULL)
    return;

  character->getAF()->root->draw(flags);
  for (int i=0; i<character->getJointCount(); i++) {
      character->getJoint(i)->getChild()->draw(flags);
  }

  glDisable(GL_TEXTURE_2D);
  glDisable(GL_LIGHTING);


  //glColor3d(0,0,0);






  world->setState(&worldState);

//  glDisable(GL_LIGHTING);

//  glEnable(GL_LIGHTING); // todo
}

void VisualizerSC::setCharacterJointState(ReducedCharacterState& newState, const VectorQj& Qj, const VectorQj& dQj) {
  const int jointMapping_[12] = {0, 4, 8, 1, 5, 9, 2, 6, 10, 3, 7, 11};

  Vector3d localRotAxis;
  Quaternion tmpRotation;
  for (int i=0;i<Qj.size();i++){
    int jointIndex = jointMapping_[i];
  //      logPrint("joint %d (their) maps to %d(mine): (%s)\n", i, jointIndex, starlETH->getJoint(jointIndex)->name);
    getLocalCoordsRotationAxisForJoint(jointIndex, localRotAxis);
  //      tprintf("local rot axis %d %f %f %f \n",i, localRotAxis.x, localRotAxis.y, localRotAxis.z);
    newState.setJointRelativeAngVelocity(localRotAxis * dQj(i), jointIndex);
    tmpRotation.setToRotationQuaternion(Qj(i), localRotAxis);
    newState.setJointRelativeOrientation(tmpRotation, jointIndex);
  }
}

void  VisualizerSC::getLocalCoordsRotationAxisForJoint(int jointIndex, Vector3d& rotationAxis){
  //assume all legs have the same rotation axis...
  const Vector3d legAAAxis = Vector3d(1, 0, 0);
  const Vector3d legFEAxis = Vector3d(0, 1, 0);


  if (isHipAA(jointIndex))
    rotationAxis = legAAAxis;
  else if (isHipFE(jointIndex))
    rotationAxis = legFEAxis;
  else if (isKneeFE(jointIndex))
    rotationAxis = legFEAxis;
  else
    throwError("remove torque component: the joint seems not to be part of any leg!!!\n");
}

bool VisualizerSC::isHipAA(int jIndex){
  return jIndex == 0 || jIndex ==  1 || jIndex ==  2 || jIndex == 3;
}

bool VisualizerSC::isHipFE(int jIndex){
  return jIndex == 4 || jIndex == 5 || jIndex == 6 || jIndex == 7;
}

bool VisualizerSC::isKneeFE(int jIndex){
  return jIndex == 8 || jIndex == 9 || jIndex == 10 || jIndex == 11;
}

void VisualizerSC::drawGaitPatternFlightPhases(loco::GaitPatternFlightPhases* gaitPattern)
{
  if (gaitPatternFlightPhasesWindow_ != nullptr) {
    gaitPatternFlightPhasesWindow_->setGaitPattern(gaitPattern);
//    gaitPatternFlightPhasesWindow_->cursorPosition = stridePhase;
    gaitPatternFlightPhasesWindow_->draw();
  }
}

void VisualizerSC::drawTrajectoryCatMullRomPosition(TrajectoryPosition &trajectory, double dt, double lineWidth) {


  const int knotCount = trajectory.getKnotCount();

  if (knotCount == 0) {
    return;
  }

  glLineWidth(lineWidth);
   GLfloat prevLineWidth;
   glGetFloatv(GL_LINE_WIDTH, &prevLineWidth);

  glBegin(GL_LINES);
    double trajLength = trajectory.getKnotPosition(trajectory.getKnotCount()-1);
    Position pt = trajectory.evaluate_catmull_rom(0.0);
    for (double t=dt; t<trajLength-dt/2; t+=dt)
    {
      Position nextPt = trajectory.evaluate_catmull_rom(t);
      glVertex3d(pt.x(), pt.y(), pt.z());
      glVertex3d(nextPt.x(), nextPt.y(), nextPt.z());
      pt = nextPt;
    }
    Position nextPt = trajectory.evaluate_catmull_rom(trajLength);
    glVertex3d(pt.x(), pt.y(), pt.z());
    glVertex3d(nextPt.x(), nextPt.y(), nextPt.z());
  glEnd();

  glLineWidth(prevLineWidth);

}



void VisualizerSC::drawTrajectoryLinearPosition(TrajectoryPosition &trajectory, double dt, double lineWidth) {

  const int knotCount = trajectory.getKnotCount();

  if (knotCount == 0) {
    return;
  }

  glLineWidth(lineWidth);
  GLfloat prevLineWidth;
  glGetFloatv(GL_LINE_WIDTH, &prevLineWidth);

  glBegin(GL_LINES);
    double trajLength = trajectory.getKnotPosition(knotCount-1);
    Position pt = trajectory.evaluate_linear(0.0);
    for (double t=dt; t<trajLength-dt/2; t+=dt)
    {
      Position nextPt = trajectory.evaluate_linear(t);
      glVertex3d(pt.x(), pt.y(), pt.z());
      glVertex3d(nextPt.x(), nextPt.y(), nextPt.z());
      pt = nextPt;
    }
    Position nextPt = trajectory.evaluate_linear(trajLength);
    glVertex3d(pt.x(), pt.y(), pt.z());
    glVertex3d(nextPt.x(), nextPt.y(), nextPt.z());
  glEnd();

  glLineWidth(prevLineWidth);

}

void VisualizerSC::drawTrajectoryLinearPositionKnots(TrajectoryPosition &trajectory, double lineWidth) {

  const int knotCount = trajectory.getKnotCount();

  if (knotCount == 0) {
    return;
  }

  glLineWidth(lineWidth);
  GLfloat prevLineWidth;
  glGetFloatv(GL_LINE_WIDTH, &prevLineWidth);

  glBegin(GL_LINES);
    Position pt = trajectory.getKnotValue(0);
    for (int i=0; i<knotCount; i++)
    {
      Position nextPt = trajectory.getKnotValue(i);
      glVertex3d(pt.x(), pt.y(), pt.z());
      glVertex3d(nextPt.x(), nextPt.y(), nextPt.z());
      pt = nextPt;
    }
    Position nextPt = trajectory.getKnotValue(knotCount-1);
    glVertex3d(pt.x(), pt.y(), pt.z());
    glVertex3d(nextPt.x(), nextPt.y(), nextPt.z());
  glEnd();

  glLineWidth(prevLineWidth);

}



void VisualizerSC::drawFrictionPyramidOfContactForceDistribution(loco::LegGroup* legs, loco::ContactForceDistribution* contactForceDistribution, double heightOfFrictionPyramid) {

  glPushMatrix();

  for (auto leg : *legs) {
    // Only draw if leg is grounded:
    if (leg->isGrounded()) {

      // Get Friction coefficient:
      const double frictionCoefficient = contactForceDistribution->getFrictionCoefficient(leg);

      const double widthOfFrictionPyramid1 = frictionCoefficient*heightOfFrictionPyramid;
      const double widthOfFrictionPyramid2 = frictionCoefficient*heightOfFrictionPyramid;

      const Vector normaltoGround = -contactForceDistribution->getNormalDirectionOfFrictionPyramidInWorldFrame(leg).normalized()*heightOfFrictionPyramid;
      const loco::Vector tangential1 = contactForceDistribution->getFirstDirectionOfFrictionPyramidInWorldFrame(leg);
      const loco::Vector tangential2 = contactForceDistribution->getSecondDirectionOfFrictionPyramidInWorldFrame(leg);


      const Position originOfFrictionPyramid = leg->getPositionWorldToFootInWorldFrame()-loco::Position(normaltoGround);


      // set color from outside of the method
//      GLUtilsKindr::glLColor(1.0,0.0,0,0.7);
      glDepthMask(GL_FALSE);
      GLUtilsKindr::drawPyramid(widthOfFrictionPyramid1, widthOfFrictionPyramid2, tangential1, tangential2, normaltoGround, originOfFrictionPyramid);
      glDepthMask(GL_TRUE);


    }

  }
  glPopMatrix();

}


} /* namespace loco */
