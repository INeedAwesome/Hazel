#pragma once

#include <iostream>
#include <chrono>
#include <string>

namespace Hazel {

	class Timer
	{
	public:
		Timer(std::string name = "")
		{
			this->name = name;
			start = std::chrono::high_resolution_clock::now();
		}
		~Timer() = default;

		float Stop()
		{
			end = std::chrono::high_resolution_clock::now();
			std::chrono::duration<float> duration = (end - start) * 1000;
			return duration.count();
		}

		void StopAndPrintTime()
		{
			std::cout << Stop() << "ms" << ": " << name << " " << std::endl;
		}
	private:
		std::string name = "";
		std::chrono::time_point<std::chrono::steady_clock> start, end;
	};

}
