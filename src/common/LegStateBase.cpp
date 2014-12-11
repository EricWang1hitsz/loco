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
 * LegStateBase.cpp
 *
 *  Created on: Feb 26, 2014
 *      Author: gech
 */

#include "loco/common/LegStateBase.hpp"

namespace loco {

  LegStateBase::LegStateBase() :
        isNow_(false),
        lastStateWasEarly_(false),
        lastStateWasLate_(false),
        stateChangedAtTime_(0.0),
        swingPhase_(0.0)
  {


  }


  LegStateBase::~LegStateBase() {

  }


  void LegStateBase::setIsNow(bool isNow) { isNow_= isNow; }
  bool LegStateBase::isNow() const { return isNow_; }

  void LegStateBase::setLastStateWasEarly(bool wasEarly) { lastStateWasEarly_ = wasEarly; }
  bool LegStateBase::lastStateWasEarly() const { return lastStateWasEarly_; }

  void LegStateBase::setLastStateWasLate(bool wasLate) { lastStateWasLate_ = wasLate; }
  bool LegStateBase::lastStateWasLate() const { return lastStateWasLate_; }

  void LegStateBase::setStateChangedAtTime(double time) { stateChangedAtTime_ = time; }
  double LegStateBase::StateChangedAtTime() const { return stateChangedAtTime_; }

  double LegStateBase::getSwingPhase() const {
    return swingPhase_;
  }

  void LegStateBase::setSwingPhase(double swingPhase) {
    swingPhase_ = swingPhase;
  }


} /* namespace loco */
