#pragma once

#include <glew.h>
#include <glut.h>
#include <cmath>

GLuint CreateNoise3D();
void SetNoiseFrequency(int frequency);
double noise1(double arg);
double noise2(double vec[2]);
double noise3(double vec[3]);
void normalize2(double v[2]);
void normalize3(double v[3]);
void initNoise();
double PerlinNoise1D(double x, double alpha, double beta, int n);
double PerlinNoise2D(double x, double y, double alpha, double beta, int n);
double PerlinNoise3D(double x, double y, double z, double alpha, double beta, int n);
void make3DNoiseTexture();
GLuint init3DNoiseTexture(int texSize, GLubyte* texPtr);
