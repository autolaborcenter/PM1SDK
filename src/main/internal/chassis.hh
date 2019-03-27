//
// Created by ydrml on 2019/2/25.
//

#ifndef PM1_SDK_CHASSIS_H
#define PM1_SDK_CHASSIS_H

#include <mutex>
#include <vector>
#include "serial/serial.h"
#include "can_define.h"
#include "odometry_t.hh"
#include "time_extensions.h"

extern "C" {
#include "control_model/model.h"
}

namespace autolabor {
	/** 电机信息 */
	template<class time_t = decltype(now())>
	struct motor_t {
		double position, speed;
		time_t time;
		
		void update(time_t _now, double value) {
			auto delta = value - position;
			
			autolabor::seconds_floating dt = _now - time;
			speed    = delta / dt.count();
			position = value;
			time     = _now;
		}
	};
	
	namespace pm1 {
		/** 底盘状态 */
		struct chassis_state_t {
			node_state_t _ecu0, _ecu1, _tcu, _vcu, _mcu;
			
			bool everyone_online() const;
		};
		
		/** 底盘 */
		class chassis final {
		public:
			explicit chassis(const std::string &port_name,
			                 const chassis_config_t & = default_config,
			                 float optimize_width = pi_f / 4,
			                 float acceleration = INFINITY);
			
			/** 析构 */
			~chassis();
			
			/** 不可复制 */
			chassis(const chassis &) = delete;
			
			/** 不可移动 */
			chassis(chassis &&) = delete;
			
			/** 左轮状态 */
			motor_t<> left() const;
			
			/** 右轮状态 */
			motor_t<> right() const;
			
			/** 舵轮状态 */
			motor_t<> rudder() const;
			
			/** 锁定 */
			void enable();
			
			/** 解锁 */
			void disable();
			
			/** 检查状态 */
			bool is_enabled() const;
			
			/** 检查状态 */
			chassis_state_t get_state() const;
			
			/** 设置目标控制量 */
			void set_target(const physical &);
			
			/** 读取里程计 */
			odometry_t odometry() const;
			
			/** 清零里程计 */
			void clear_odometry();
		
		private:
			/** 串口引用 */
			std::shared_ptr<serial::Serial> port;
			
			/** 节点状态 */
			chassis_state_t chassis_state{};
			
			/** 电机数据 */
			motor_t<> _left{},
			          _right{},
			          _rudder{};
			
			/** 底盘参数 */
			chassis_config_t parameters{};
			
			/** 优化参数 */
			float optimize_width,
			      acceleration;
			
			/** 里程计 */
			odometry_t _odometry;
			
			mutable std::mutex
					odometry_protector;
			
			std::shared_ptr<std::mutex>
					send_mutex,
					receive_mutex;
			
			/** 目标运动 */
			physical target{};
			
			/** 最后一次请求的时间 */
			mutable decltype(autolabor::now()) request_time;
		};
	}
}


#endif //PM1_SDK_CHASSIS_H
