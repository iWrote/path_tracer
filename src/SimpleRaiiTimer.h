#pragma once
#ifndef SIMPLERAIITIMER_H
#define SIMPLERAIITIMER_H


#include <iostream>
#include <chrono>


//http://www.cplusplus.com/reference/chrono/
//simple timing tool i saw on TheChernoProject

struct Timer
{
	Timer(const char* l)
		:label{l}
	{
		start = std::chrono::high_resolution_clock::now();
	}
	~Timer()
	{
		end = std::chrono::high_resolution_clock::now();
		duration = end - start;

		double ms = duration.count() * 1000.0f;
		std::cout << "Timer[" << label << "]: " << ms << "ms\n";
	}
private:
	std::chrono::time_point<std::chrono::steady_clock> start, end;
	std::chrono::duration<double> duration;

	const char* label;
};

#endif // !SIMPLERAIITIMER_H
