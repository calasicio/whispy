#pragma once

#include <random>

inline std::random_device rd;
inline std::mt19937 gen(rd());

namespace random
{
  inline float rangeFloat(float min, float max)
  {
    std::uniform_real_distribution<float> dis(min, max);

    return dis(gen);
  }

  inline float generateGaussianNoise(float mean, float stddev)
  {
    std::normal_distribution<float> dist(mean, stddev);

    return dist(gen);
  }
}