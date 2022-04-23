cmake_minimum_required(VERSION 3.17)
project(AI_particle_filter)

set(CMAKE_CXX_STANDARD 14)

add_executable(AI_particle_filter main.cpp Robot.h Particle.h Helpers.h Sensor.cpp Sensor.h)