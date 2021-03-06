/**
 * @file utils/types.h
 * @author Simone Comari
 * @date 11 Feb 2020
 * @brief File containing the implementation of a custom wrapper to log cable robot data
 * employing easylogging++ package.
 */

#ifndef CABLE_ROBOT_TYPES_H
#define CABLE_ROBOT_TYPES_H

#include <cmath>
#include <stdint.h>
#include <stdlib.h>
#include <vector>

#include "libgrabec/inc/slaves/goldsolowhistledrive.h"

/**
 * @brief StringBuf Convenient type for string buffer.
 */
typedef std::basic_stringbuf<char16_t> StringBuf;

template <typename T> using vect = std::vector<T>; /**< Alias for vector type. */
using vectD                      = vect<double>; /**< Alias for vector of double type. */
using vectI                      = vect<int>;    /**< Alias for vector of int type. */
using vectS                      = vect<short>;  /**< Alias for vector of short type. */

/**
 * @brief A structure including motor status information.
 */
struct MotorStatus
{
  /**
   * @brief MotorStatus default constructor.
   */
  MotorStatus() : id(0), motor_position(0), motor_speed(0), motor_torque(0), op_mode(0) {}
  /**
   * @brief MotorStatus full implicit constructor.
   * @param[in] _id Motor ID.
   * @param[in] drive_pdos GoldSoloWhistle drive input PDOs from which to extract motor
   * status information.
   */
  MotorStatus(const id_t _id, const grabec::GSWDriveInPdos& drive_pdos)
    : id(_id), motor_position(drive_pdos.pos_actual_value),
      motor_speed(drive_pdos.vel_actual_value),
      motor_torque(drive_pdos.torque_actual_value), op_mode(drive_pdos.display_op_mode)
  {}
  /**
   * @brief MotorStatus full explicit constructor.
   * @param[in] _id Motor ID.
   * @param[in] _op_mode Motor operational mode
   * @param[in] motor_pos Motor position in encoder counts.
   * @param[in] _motor_speed Motor velocity in encoder counts/second.
   * @param[in] _motor_torque Motor torque in per thousand nominal points.
   */
  MotorStatus(const id_t _id, const int8_t _op_mode, const int32_t motor_pos,
              const int32_t _motor_speed, const int16_t _motor_torque)
    : id(_id), motor_position(motor_pos), motor_speed(_motor_speed),
      motor_torque(_motor_torque), op_mode(_op_mode)
  {}

  id_t id;                /**< Motor ID. */
  int32_t motor_position; /**< Motor position in encoder counts.*/
  int32_t motor_speed;    /**< Motor velocity in encoder counts/second.*/
  int16_t motor_torque;   /**< Motor torque in per thousand nominal points.*/
  int8_t op_mode;         /**< Motor operational mode*/
};

/**
 * @brief A structure including winch status information.
 *
 * Winch status is an extension of motor status, with the additional information about
 * cable length.
 */
struct WinchStatus: MotorStatus
{
  /**
   * @brief WinchStatus default constructor.
   */
  WinchStatus() : aux_position(0), cable_length(0.0) {}
  /**
   * @brief WinchStatus full implicit constructor.
   * @param[in] _id Motor ID.
   * @param[in] drive_pdos GoldSoloWhistle drive input PDOs from which to extract motor
   * status information.
   * @param[in] cable_len Cable length in meters.
   */
  WinchStatus(const id_t _id, const grabec::GSWDriveInPdos& drive_pdos,
              const double cable_len = 0.0)
    : MotorStatus(_id, drive_pdos), aux_position(drive_pdos.aux_pos_actual_value),
      cable_length(cable_len)
  {}
  /**
   * @brief WinchStatus full explicit constructor.
   * @param[in] _id Motor ID.
   * @param[in] _op_mode Motor operational mode.
   * @param[in] motor_pos Motor position in encoder counts.
   * @param[in] _motor_speed Motor velocity in encoder counts/second.
   * @param[in] _motor_torque Motor torque in per thousand nominal points.
   * @param[in] cable_len Cable length in meters.
   * @param[in] aux_pos Auxiliary encoder position. This corresponds to swivel pulley
   * encoder value for our case.
   */
  WinchStatus(const id_t _id, const int8_t _op_mode, const int32_t motor_pos,
              const int32_t _motor_speed, const int16_t _motor_torque,
              const double cable_len, const int aux_pos)
    : MotorStatus(_id, _op_mode, motor_pos, _motor_speed, _motor_torque),
      aux_position(aux_pos), cable_length(cable_len)
  {}

  int aux_position;    /**< Cable length in meters.*/
  double cable_length; /**< Auxiliary encoder position. */
};

/**
 * @brief A structure including actuator status information.
 *
 * Actuator status is an extension of winch status, with the additional information about
 * swivel pulleys system.
 */
struct ActuatorStatus: WinchStatus
{
  /**
   * @brief ActuatorStatus default constructor.
   */
  ActuatorStatus() : pulley_angle(0.0) {}
  /**
   * @brief ActuatorStatus full implicit constructor.
   * @param[in] _id Motor ID.
   * @param[in] drive_pdos GoldSoloWhistle drive input PDOs from which to extract motor
   * status information.
   * @param[in] cable_len Cable length in meters.
   * @param[in] pulley_ang Swivel pulley angle in radians.
   */
  ActuatorStatus(const id_t _id, const grabec::GSWDriveInPdos& drive_pdos,
                 const double cable_len = 0.0, const double pulley_ang = 0.0)
    : WinchStatus(_id, drive_pdos, cable_len), pulley_angle(pulley_ang)
  {}
  /**
   * @brief ActuatorStatus full explicit constructor.
   * @param[in] _id Motor ID.
   * @param[in] _op_mode Motor operational mode.
   * @param[in] motor_pos Motor position in encoder counts.
   * @param[in] _motor_speed Motor velocity in encoder counts/second.
   * @param[in] _motor_torque Motor torque in per thousand nominal points.
   * @param[in] cable_len Cable length in meters.
   * @param[in] aux_pos Auxiliary encoder position. This corresponds to swivel pulley
   * encoder value for our case.
   * @param[in] pulley_ang Swivel pulley angle in radians.
   */
  ActuatorStatus(const id_t _id, const int8_t _op_mode, const int32_t motor_pos,
                 const int32_t _motor_speed, const int16_t _motor_torque,
                 const double cable_len, const int aux_pos, const double pulley_ang)
    : WinchStatus(_id, _op_mode, motor_pos, _motor_speed, _motor_torque, cable_len,
                  aux_pos),
      pulley_angle(pulley_ang)
  {}

  uint8_t state;       /**< see Actuator::States */
  double pulley_angle; /**< [rad] */
};


template <typename T>
/**
 * @brief A convenient structure to describe a waypoint, that is a value with a timestamp.
 */
struct WayPoint
{
  double ts = -1.0; /**< .. */
  T value;          /**< .. */

  /**
   * @brief Default constructor.
   */
  WayPoint() {}
  /**
   * @brief Full constructor
   * @param[in] time Waypoint timestamps in seconds.
   * @param[in] _value Waypoint value.
   */
  WayPoint(const double time, const T& _value) : ts(time), value(_value) {}
};

using WayPointD = WayPoint<double>; /**< alias for a waypoint with a double value */
using WayPointI = WayPoint<int>;    /**< alias for a waypoint with an int value */
using WayPointS = WayPoint<short>;  /**< alias for a waypoint with a short value */

template <typename T>
/**
 * @brief A convenient structure to describe a trajectory, that is an array of scalar with
 * an array of timestamps of equal length.
 */
struct Trajectory
{
  id_t id = 0;      /**< The ID of the trajectory, for example of the relative motor. */
  vectD timestamps; /**< An array of timestamps in seconds. */
  vect<T> values;   /**< An array of generic scalar values. */

  /**
   * @brief Default constructor.
   *
   * By default, ID is 0 and trajectory is empty.
   */
  Trajectory() {}
  /**
   * @brief Partial constructor.
   *
   * This constructor assign an ID, but leaves the trajectory empty.
   * @param[in] _id Trajectory ID.
   */
  Trajectory(const id_t _id) : id(_id) {}
  /**
   * @brief Partial constructor.
   *
   * This constructor assign an ID, fills the trajectory values and set all timestamps to
   * -1.
   * @param[in] _id Trajectory ID.
   * @param[in] _values An array of scalar values.
   */
  Trajectory(const id_t _id, const vect<T>& _values) : id(_id), values(_values)
  {
    timestamps = vectD(values.size(), -1.0);
  }
  /**
   * @brief Full constructor
   * @param[in] _id Trajectory ID.
   * @param[in] _values An array of scalar values.
   * @param[in] times An array of timestamps in seconds.
   * @warning times and _values must have the same length. Error raised otherwise.
   */
  Trajectory(const id_t _id, const vect<T>& _values, const vectD& times)
    : id(_id), values(_values), timestamps(times)
  {
    assert(times.size() == _values.size());
  }

  /**
   * @brief Get a waypoint from given index.
   * @param[in] index The index of the desired waypoint.
   * @return The desired waypoint.
   * @warning The index must be less than trajectory size, otherwise an error is raised.
   * @see waypointFromAbsTime waypointFromRelTime
   */
  WayPoint<T> waypointFromIndex(const size_t index) const
  {
    assert(index < timestamps.size() && index < values.size());
    return WayPoint<T>(timestamps[index], values[index]);
  }

  /**
   * @brief Get a waypoint from given absolute time.
   * @param[in] time An absolute time in seconds.
   * @param[in] eps The tolerance used to avoid numerical issues.
   * @return The closes waypoint to given time.
   * @see waypointFromRelTime waypointFromIndex
   */
  WayPoint<T> waypointFromAbsTime(const double time, const double eps = 1e-6) const
  {
    assert(timestamps.front() >= 0.0);

    if (time <= timestamps.front())
      return WayPoint<T>(timestamps.front(), values.front());
    if (time >= timestamps.back())
      return WayPoint<T>(timestamps.back(), values.back());
    // Find nearest neighbor
    vectD::const_iterator low =
      std::lower_bound(timestamps.begin(), timestamps.end(), time);
    ulong lower_idx;
    if (low == timestamps.end())
      // no bigger value than val in vector --> this shouldn't happen
      return WayPoint<T>(timestamps.back(), values.back());
    if (low == timestamps.begin())
      lower_idx = 0;
    else
      lower_idx = low - timestamps.begin() - 1;
    const ulong upper_idx = lower_idx + 1;

    double dt_left  = time - timestamps[lower_idx];
    double dt_right = timestamps[upper_idx] - time;
    double value;
    if (std::min(dt_left, dt_right) <= eps)
    {
      if (dt_left < dt_right)
        value = values[lower_idx];
      else
        value = values[upper_idx];
    }
    else
    {
      // Interpolate
      const double slope = (values[upper_idx] - values[lower_idx]) /
                           (timestamps[upper_idx] - timestamps[lower_idx]);
      const double offset = values[upper_idx] - slope * timestamps[upper_idx];
      value               = slope * time + offset;
    }

    return WayPoint<T>(time, static_cast<T>(value));
  }

  /**
   * @brief Get a waypoint from given relative time.
   * @param[in] time A relative time in seconds.
   * @param[in] eps The tolerance used to avoid numerical issues.
   * @return The closes waypoint to given time.
   * @see waypointFromAbsTime waypointFromIndex
   */
  WayPoint<T> waypointFromRelTime(const double time, const double eps = 1e-6) const
  {
    assert(timestamps.front() >= 0.0);

    return waypointFromAbsTime(time + timestamps.front(), eps);
  }
};

using TrajectoryD = Trajectory<double>; /**< alias for trajectory of double values */
using TrajectoryI = Trajectory<int>;    /**< alias for trajectory of int values */
using TrajectoryS = Trajectory<short>;  /**< alias for trajectory of short values */

#endif // CABLE_ROBOT_TYPES_H
