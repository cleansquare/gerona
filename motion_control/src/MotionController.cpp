#include <geometry_msgs/Point32.h>
#include "MotionController.h"


bool MotionController::getSlamPose( Vector3d &pose ) const
{
    tf::StampedTransform transform;
    geometry_msgs::TransformStamped msg;

    std::string source_frame("base_link");
    std::string target_frame("map");
    try {
      pose_listener_.lookupTransform(target_frame, source_frame, ros::Time(0), transform);
    } catch (tf::TransformException& ex) {
      ROS_ERROR("error with transform robot pose: %s", ex.what());
      return false;
    }
    tf::transformStampedTFToMsg(transform, msg);

    pose.x() = msg.transform.translation.x;
    pose.y() = msg.transform.translation.y;
    pose(2) = tf::getYaw(msg.transform.rotation);
    return true;
}

