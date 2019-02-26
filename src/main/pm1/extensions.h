//
// Created by ydrml on 2019/2/23.
//

#ifndef PM1_SDK_EXTENSIONS_H
#define PM1_SDK_EXTENSIONS_H

#include <iostream>
#include <sstream>
#include <functional>
#include <thread>

namespace mechdancer {
	namespace common {
		/**
		 * 连接为字符串
		 *
		 * @tparam T 类型通配符
		 * @param splitter 间隔符
		 * @param items 连接对象
		 * @return 字符串
		 */
		template<class...T>
		inline std::string join_to_string(std::string splitter, T...items) {
			std::stringstream builder;
			
			const auto ___ = {(builder << items << splitter, 0)...};
			const auto str = builder.str();
			return str.substr(0, str.length() - splitter.length());
		}
		
		/**
		 * 线程睡眠
		 *
		 * @param ms 时间（毫秒）
		 */
		inline void sleep(uint32_t ms) {
			std::this_thread::sleep_for(std::chrono::milliseconds(ms));
		}
		
		/**
		 * 打印到控制台
		 *
		 * @tparam T 类型通配符
		 * @param data 数据
		 */
		template<class T>
		inline void print(T data) {
			std::cout << data;
		}
		
		/**
		 * 打印一行到控制台
		 *
		 * @tparam T 类型通配符
		 * @param data 数据
		 */
		template<class T>
		inline void println(T data) {
			std::cout << data << std::endl;
		}
	}
}

#endif //PM1_SDK_EXTENSIONS_H