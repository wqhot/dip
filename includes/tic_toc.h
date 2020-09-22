#pragma once

#include <ctime>
#include <cstdlib>
#include <chrono>
#include <iostream>
#include <string>

class TicToc
{
public:
  TicToc()
  {
    tic();
  }

  void tic()
  {
    start = std::chrono::system_clock::now();
  }

  double toc()
  {
    end = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_seconds = end - start;
    return elapsed_seconds.count() * 1000;
  }

  double restart_toc()
  {
    end = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_seconds = end - start;
    start = std::chrono::system_clock::now();
    return elapsed_seconds.count() * 1000;
  }

  void toc_print(std::string str)
  {
    end = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_seconds = end - start;
    std::cout << str << " spend: " << elapsed_seconds.count() * 1000 << " ms" << std::endl;
  }

  void toc_print_restart(std::string str)
  {
    end = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_seconds = end - start;
    std::cout << str << " spend: " << elapsed_seconds.count() * 1000 << " ms" << std::endl;
    start = std::chrono::system_clock::now();
  }

private:
  std::chrono::time_point<std::chrono::system_clock> start, end;
};
