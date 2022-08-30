#pragma once

#include <memory>
#include <iostream>
#include <chrono>
#include <string>

#define	TIME_SCOPE() LifetimeTimer(__FUNCTION__)

namespace FR {
	// Reference count pointer handle
	template<typename T>
	using Ref = std::shared_ptr<T>;

	template<typename T, typename ... Args>
	constexpr Ref<T> createRef(Args&& ... args) {
		return std::make_shared<T>(std::forward<Args>(args)...);
	}

	class LifetimeTimer {
	public:
		LifetimeTimer(std::string _name = "Unnamed Timer", bool _outputEndTime = true) :
			name(_name),
			outputEndTime(_outputEndTime)
		{
			start = std::chrono::high_resolution_clock::now();
		}

		double currentTimeMilliseconds() {
			end = std::chrono::high_resolution_clock::now();
			double duration = std::chrono::duration<double, std::milli>(end - start).count();

			return duration;
		}

		~LifetimeTimer() {
			if (outputEndTime)
				std::cout << "\"" << name << "\" took: " << currentTimeMilliseconds() << "ms" << std::endl;
		}
	private:
		using TimePoint = std::chrono::time_point<std::chrono::high_resolution_clock>;

		TimePoint
			start,
			end;

		std::string name;
		bool outputEndTime;
	};
}