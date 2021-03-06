#include <mission_planner_inspection_follower.hpp>

MissionPlannerInspectionFollower::MissionPlannerInspectionFollower(trajectory_planner::parameters _params, inspection_params _inspection_params)
    : MissionPlannerInspection(_params, _inspection_params) {}

MissionPlannerInspectionFollower::~MissionPlannerInspectionFollower() {}

bool MissionPlannerInspectionFollower::checks() {
  if (!hasPose()) {
    std::cout << "Mission Planner " << param_.drone_id
              << " does not have all poses" << std::endl;
    return false;
  }
  return true;
}

std::vector<state> MissionPlannerInspectionFollower::initialTrajectory(
    const state &initial_pose) {
  
  std::vector<state> trajectory_to_optimize;

  if (hasSolvedTrajectories()) {
    refreshGoals();
    state aux;
    trajectory_to_optimize.push_back(initial_pose);
    Eigen::Quaterniond rotation = trajectory_planner::eulerToQuat(0, 0, relative_angle_);
    Eigen::Matrix3d rotMat = rotation.toRotationMatrix();
    Eigen::Vector3d aux_point_to_inspect = point_to_inspect_;
    aux_point_to_inspect(2) = 0;
    for (int i = 1; i < param_.horizon_length; i++) {
      aux.pos = rotMat * (solved_trajectories_[inspection_params_.leader_id][i].pos -
                          aux_point_to_inspect) +
                aux_point_to_inspect;
      trajectory_to_optimize.push_back(std::move(aux));
    }

    return trajectory_to_optimize;
  }else{
    return trajectory_to_optimize;
  }
}
