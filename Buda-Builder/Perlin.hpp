#pragma once

#include <math.h>

class Perlin {
public:
	const float offX, offY, inc;

	Perlin(float offsetX, float offsetY, float increment)
		: offX{ offsetX }, offY{ offsetY }, inc{ increment } {};


	/* Function to linearly interpolate between a0 and a1
	 * Weight w should be in the range [0.0, 1.0]
	 */
	float interpolate(float a0, float a1, float w) {
		return (a1 - a0) * ((w * (w * 6.0 - 15.0) + 10.0) * w * w * w) + a0;
		
	}

	typedef struct {
		float x, y;
	} vector2;

	// Create pseudorandom direction vector
	vector2 randomGradient(int ix, int iy) {
		// No precomputed gradients mean this works for any number of grid coordinates
		const unsigned w = 8 * sizeof(unsigned);
		const unsigned s = w / 2; // rotation width
		unsigned a = ix, b = iy;
		a *= 3284157443; b ^= a << s | a >> w - s;
		b *= 1911520717; a ^= b << s | b >> w - s;
		a *= 2048419325;
		float random = a * (3.14159265 / ~(~0u >> 1)); // in [0, 2*Pi]
		vector2 v;
		v.x = cos(random); v.y = sin(random);
		return v;
	}

	// Computes the dot product of the distance and gradient vectors.
	float dotGridGradient(int ix, int iy, float x, float y) {
		// Get gradient from integer coordinates
		vector2 gradient = randomGradient(ix, iy);

		// Compute the distance vector
		float dx = x - (float)ix;
		float dy = y - (float)iy;

		// Compute the dot-product
		return (dx * gradient.x + dy * gradient.y);
	}

	// Compute Perlin noise at coordinates x, y
	float perlin(float x, float y) {
		// Determine grid cell coordinates
		int x0 = (int)floor(x);
		int x1 = x0 + 1;
		int y0 = (int)floor(y);
		int y1 = y0 + 1;

		// Determine interpolation weights
		// Could also use higher order polynomial/s-curve here
		float sx = x - (float)x0;
		float sy = y - (float)y0;

		// Interpolate between grid point gradients
		float n0, n1, ix0, ix1, value;

		n0 = dotGridGradient(x0, y0, x, y);
		n1 = dotGridGradient(x1, y0, x, y);
		ix0 = interpolate(n0, n1, sx);

		n0 = dotGridGradient(x0, y1, x, y);
		n1 = dotGridGradient(x1, y1, x, y);
		ix1 = interpolate(n0, n1, sx);

		// In range -1 to 1. To make it in range 0 to 1, multiply by 0.5 and add 0.5
		value = interpolate(ix0, ix1, sy);
		return value*0.5f+0.5f; 
	}

	// Generate 2D Perlin noise with given width and height
	std::vector<float> GetPerlin2D(unsigned width, unsigned height) {
		std::vector<float> noise2D;
		noise2D.resize(width * height);

		float x = offX;
		for (unsigned i = 0; i < width; i++) {
			float y = offY;
			for (unsigned j = 0; j < height; j++) {
				noise2D[i * height + j] = perlin(x,y);
				y += inc;
			}
			x += inc;
		}

		return noise2D;
	}

};
