#pragma once
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <stdint.h>
#include <utility>
#include <vector>
#include <cmath>
#include <ctime>
#include <algorithm>


class Genetic
{
private:
	struct pixel { uint8_t R, G, B; };
	
	// variables
	bool setup_done = false;
	int W = 160, H = 240, POPULATION_SIZE = 120, NUMBER_OF_BEST_ELEMENTS = 10;
	int mutation_chance = 45;
	int SZ = W * H;
	const char* name_write;
	const char* name_read;
	std::vector<pixel> goal_picture, child;
	std::vector<std::pair<std::vector<pixel>, double>> elements;

	// functions
	int readRGB();
	int saveData();
	void mutate();
	void rank();
	void make_love();

public:
	Genetic();
	virtual ~Genetic();

	void setup(int w, int h, int pop_size, int nof_best, int mut_per, const char* n_in, const char* n_out);
	void run();
};

