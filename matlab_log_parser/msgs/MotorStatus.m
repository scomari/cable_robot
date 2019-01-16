classdef MotorStatus < handle
    properties
        op_mode
        motor_position
        motor_speed
        motor_torque
    end
    
    methods
        function set(obj, motor_status_packed)
            obj.op_mode = motor_status_packed(1);
            obj.motor_position = motor_status_packed(2);
            obj.motor_speed = motor_status_packed(3);
            obj.motor_torque = motor_status_packed(4);
        end
        
        function append(obj, motor_status_packed)
            obj.op_mode(end + 1) = motor_status_packed(1);
            obj.motor_position(end + 1) = motor_status_packed(2);
            obj.motor_speed(end + 1) = motor_status_packed(3);
            obj.motor_torque(end + 1) = motor_status_packed(4);
        end        
        
        function clear(obj)
            obj.op_mode = [];
            obj.motor_position = [];
            obj.motor_speed = [];
            obj.motor_torque = [];
        end
    end
end
