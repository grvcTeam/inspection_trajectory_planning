#pragma once

#include <math.h>
#include <Eigen/Dense>
#include <Eigen/StdVector>
#include <acado/acado_optimal_control.hpp>
#include <acado_toolkit.hpp>
#include <map>
// #include "log.h"
#include "mission_planner_types.hpp"
#include <trajectory_planner.hpp>

typedef trajectory_planner::state state;

//! MissionPlannerInspection class
/*!
 * Abstract base class for mission planner. It cannot be instantiated
 */

class MissionPlannerInspection : public trajectory_planner::TrajectoryPlanner{
 public:

  /**
   * @brief constructor of the class
   */
  MissionPlannerInspection(const trajectory_planner::parameters _param, const inspection_params _inspection_params);

  /**
   * @brief destructor of the class
   */
  virtual ~MissionPlannerInspection();

  /**
   * @brief changes the desired point to inspect
   *
   * @param _point point to inspect
   */
  void setPointToInspect(const Eigen::Vector3d &_point) {
    point_to_inspect_ = std::move(_point);
  }

  /**
   * @brief gives the desired point to inspect
   *
   * @return point to inspect
   */
  Eigen::Vector3d getPointToInspect() { return point_to_inspect_; }

  /**
   * @brief changes the desired distance to the inspection point
   *
   * @param _distance distance to the inspection point
   */
  void setDistanceToInspect(const float &_distance) {
    distance_to_inspect_point_ = _distance;
  }

  /**
   * @brief increases/decreases the desired distance to the inspection point
   *
   * @param _distance true if increase, false if decrease
   */
  void incDistanceToInspect(const bool &_distance) {
    if (_distance)  setDistanceToInspect(distance_to_inspect_point_ + inspection_params_.inc_distance);
    else                    setDistanceToInspect(distance_to_inspect_point_ - inspection_params_.inc_distance);
  }

  /**
   * @brief gives the distance to the inspection point
   *
   * @return distance to the inspection point
   */
  float getDistanceToInspect() { return distance_to_inspect_point_; }

  /**
   * @brief changes the desired relative angle of the drones
   *
   * @param _angle angle
   */
  void setRelativeAngle(const float &_angle) { relative_angle_ = _angle; }

  /**
   * @brief gives the relative angle
   *
   * @return relative angle
   */
  float getRelativeAngle() { return relative_angle_; }

  /**
   * @brief increases/decreases the relative angle of the drones
   *
   * @param _distance true if increase, false if decrease
   */
  void incRelativeAngle(const bool &_angle) {
    if (_angle)  setRelativeAngle(relative_angle_ + inspection_params_.inc_angle);
    else                 setRelativeAngle(relative_angle_ - inspection_params_.inc_angle);
  }

  /**
   * @brief fits a given point to the cylinder/circle where the drones are
   * moving around
   *
   * @param point desired point to fit on the cylinder/circle
   * @return point on the cylinder/circle
   */
  Eigen::Vector3d pointOnCircle(const Eigen::Vector3d point);

 protected:
  Eigen::Vector3d point_to_inspect_ = Eigen::Vector3d::Zero();
  float distance_to_inspect_point_ = 3;
  float relative_angle_ = 0.7;
  inspection_params inspection_params_;

  /**
   * @brief refreshes the value of the goal points
   */
  void refreshGoals() {
    for (auto &goal : goals_) {
      goal.pos = pointOnCircle(goal.pos);
    }
  }

 private:

  const float TOL_INSPECTION_ZONE = 1;
  /**
   * @brief returns an initial trajectory to inspect for the drone according to
   * the initial pose
   *
   * @param initial_pose initial pose of the drone
   * @return vector of states of the trajectory
   */
  virtual std::vector<state> initialTrajectory(
      const state &initial_pose) = 0;

  /**
   * @brief virtual function that makes the following checks
   *
   * @return true if all checks are passed
   * @return false if any ot the check is not passed
   */
  virtual bool checks() {
    std::cout << "check mission planner abstract" << std::endl;
  }

  /**
   * @brief gives an initial orientation according to a trajectory given
   *
   * @param traj trajectory
   */
  virtual void initialOrientation(std::vector<state> &traj);


  /**
   * @brief Utility function that checks if the drone is near the inspection
   * zone
   *
   * @param drone_pose drone's current pose
   */
  bool isInspectionZone(const Eigen::Vector3d &drone_pose);



};