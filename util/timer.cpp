/* Copyright (c) 2017-2019 Hans-Kristian Arntzen
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#include "timer.hpp"
#include <chrono>

using namespace std;

namespace Util
{
FrameTimer::FrameTimer()
{
	reset();
}

void FrameTimer::reset()
{
	start = get_time();
	last = start;
	last_period = 0;
}

void FrameTimer::enter_idle()
{
	idle_start = get_time();
}

void FrameTimer::leave_idle()
{
	auto idle_end = get_time();
	idle_time += idle_end - idle_start;
}

double FrameTimer::get_frame_time() const
{
	return double(last_period) * 1e-9;
}

double FrameTimer::frame()
{
	auto new_time = get_time() - idle_time;
	last_period = new_time - last;
	last = new_time;
	return double(last_period) * 1e-9;
}

double FrameTimer::frame(double frame_time)
{
	last_period = int64_t(frame_time * 1e9);
	last += last_period;
	return frame_time;
}

double FrameTimer::get_elapsed() const
{
	return double(last - start) * 1e-9;
}

int64_t FrameTimer::get_time()
{
	return get_current_time_nsecs();
}

int64_t get_current_time_nsecs()
{
	auto current = chrono::steady_clock::now().time_since_epoch();
	auto nsecs = chrono::duration_cast<chrono::nanoseconds>(current);
	return nsecs.count();
}

void Timer::start()
{
	t = get_current_time_nsecs();
}

double Timer::end()
{
	auto nt = get_current_time_nsecs();
	return double(nt - t) * 1e-9;
}
}