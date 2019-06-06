//
// This file is part of the course materials for CSE P 524 at the University of Washington,
// Autumn 2018
//
// Licensed under Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License
// https://creativecommons.org/licenses/by-nc-sa/4.0/
//
// Author: Andrew Lumsdaine
//

#if !defined(TIMER_HPP)
#define TIMER_HPP

#include <chrono>

class Timer {
private:
  typedef std::chrono::time_point<std::chrono::system_clock> time_t;

public:
  Timer() : startTime(), stopTime() {}

  time_t start() { return (startTime = std::chrono::system_clock::now()); }
  time_t stop() { return (stopTime = std::chrono::system_clock::now()); }
  double elapsed() { return std::chrono::duration_cast<std::chrono::milliseconds>(stopTime - startTime).count(); }

private:
  time_t startTime, stopTime;
};

#define DEF_TIMER(label)			\
  Timer t_##label

#define START_TIMER(label)			\
  do {						\
    t_##label.start();				\
  } while(0)

#define STOP_TIMER(label)					\
  do {								\
    t_##label.stop();						\
    std::cout << "# elapsed time ["<< #label <<"]: "		\
	      << t_##label.elapsed() << " ms" << std::endl;	\
  } while(0)

#define STOP_TIMER_QUIETLY(label)                               \
  { [ & ] (void) -> double { t_##label.stop(); return t_##label.elapsed(); }() }


#endif    // TIMER_HPP
