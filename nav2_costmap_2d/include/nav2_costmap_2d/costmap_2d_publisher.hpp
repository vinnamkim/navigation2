/*********************************************************************
 *
 * Software License Agreement (BSD License)
 *
 *  Copyright (c) 2008, 2013, Willow Garage, Inc.
 *  All rights reserved.
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
 *   * Neither the name of Willow Garage, Inc. nor the names of its
 *     contributors may be used to endorse or promote products derived
 *     from this software without specific prior written permission.
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
 *
 * Author: Eitan Marder-Eppstein
 *         David V. Lu!!
 *********************************************************************/
#ifndef NAV2_COSTMAP_2D__COSTMAP_2D_PUBLISHER_HPP_
#define NAV2_COSTMAP_2D__COSTMAP_2D_PUBLISHER_HPP_

#include <algorithm>
#include <string>

#include "rclcpp_lifecycle/lifecycle_node.hpp"
#include "nav2_costmap_2d/costmap_2d.hpp"
#include "nav_msgs/msg/occupancy_grid.hpp"
#include "map_msgs/msg/occupancy_grid_update.hpp"
#include "tf2/transform_datatypes.h"
#include "nav2_util/lifecycle_node.hpp"

namespace nav2_costmap_2d
{
/**
 * @class Costmap2DPublisher
 * @brief A tool to periodically publish visualization data from a Costmap2D
 */
class Costmap2DPublisher
{
public:
  /**
   * @brief  Constructor for the Costmap2DPublisher
   */
  Costmap2DPublisher(
    nav2_util::LifecycleNode::SharedPtr ros_node,
    Costmap2D * costmap,
    std::string global_frame,
    std::string topic_name,
    bool always_send_full_costmap = false);

  /**
   * @brief  Destructor
   */
  ~Costmap2DPublisher();

  void on_configure() {}
  void on_activate()
  {
    costmap_pub_->on_activate();
    costmap_update_pub_->on_activate();
  }
  void on_deactivate()
  {
    costmap_pub_->on_deactivate();
    costmap_update_pub_->on_deactivate();
  }
  void on_cleanup() {}

  /** @brief Include the given bounds in the changed-rectangle. */
  void updateBounds(unsigned int x0, unsigned int xn, unsigned int y0, unsigned int yn)
  {
    x0_ = std::min(x0, x0_);
    xn_ = std::max(xn, xn_);
    y0_ = std::min(y0, y0_);
    yn_ = std::max(yn, yn_);
  }

  /**
   * @brief  Publishes the visualization data over ROS
   */
  void publishCostmap();

  /**
   * @brief Check if the publisher is active
   * @return True if the frequency for the publisher is non-zero, false otherwise
   */
  bool active()
  {
    return active_;
  }

private:
  /** @brief Prepare grid_ message for publication. */
  void prepareGrid();

  /** @brief Publish the latest full costmap to the new subscriber. */
  // void onNewSubscription(const ros::SingleSubscriberPublisher& pub);

  nav2_util::LifecycleNode::SharedPtr node_;
  Costmap2D * costmap_;
  std::string global_frame_;
  std::string topic_name_;
  unsigned int x0_, xn_, y0_, yn_;
  double saved_origin_x_;
  double saved_origin_y_;
  bool active_;
  bool always_send_full_costmap_;

  rclcpp_lifecycle::LifecyclePublisher<nav_msgs::msg::OccupancyGrid>::SharedPtr costmap_pub_;
  rclcpp_lifecycle::LifecyclePublisher<map_msgs::msg::OccupancyGridUpdate>::SharedPtr
    costmap_update_pub_;

  nav_msgs::msg::OccupancyGrid grid_;
  // Translate from 0-255 values in costmap to -1 to 100 values in message.
  static char * cost_translation_table_;
};
}  // namespace nav2_costmap_2d
#endif  // NAV2_COSTMAP_2D__COSTMAP_2D_PUBLISHER_HPP_
