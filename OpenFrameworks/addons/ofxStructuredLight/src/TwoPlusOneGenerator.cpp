#include "TwoPlusOneGenerator.h"

TwoPlusOneGenerator::TwoPlusOneGenerator():
	orientation(PHASE_VERTICAL),
	wavelength(0) {
}

void TwoPlusOneGenerator::setOrientation(phaseOrientation orientation) {
	this->orientation = orientation;
}

void TwoPlusOneGenerator::setWavelength(float wavelength) {
	this->wavelength = wavelength;
}

void TwoPlusOneGenerator::setMinBrightness(float minBrightness) {
    this->minBrightness = minBrightness;
}

void TwoPlusOneGenerator::generate() {
	allocateSequence(3);
	for(int k = 0; k < 3; k++) {
		unsigned char* pixels = sequence[k].getPixels();
		float normalize = TWO_PI / wavelength;
		int side = orientation == PHASE_VERTICAL ? width : height;
		unsigned char* single = new unsigned char[side * 3];
		int i = 0;
		for(int j = 0; j < side; j++) {
			float curPhase;
			switch (k){
			case 0:
				curPhase = (sinf(j * normalize)+1.)/2.;
			break;
			case 1:
				curPhase = (cosf(j * normalize)+1.)/2.;
			break;
			case 2:
				curPhase = 0.5;
			break;
			}
			curPhase *= (256-minBrightness);
			curPhase += minBrightness;
			if(curPhase >= 256)
				curPhase = 255;
			unsigned char value = (unsigned char) curPhase;
			single[i++] = value;
			single[i++] = value;
			single[i++] = value;
		}
		if(orientation == PHASE_VERTICAL) {
			for(int y = 0; y < height; y++) {
				memcpy(&pixels[y * width * 3], single, width * 3);
			}
		} else {
			int i = 0;
			int j = 0;
			for(int y = 0; y < height; y++) {
				for(int x = 0; x < width; x++) {
					memcpy(&pixels[i], &single[j], 3);
					i += 3;
				}
				j += 3;
			}
		}
		delete [] single;
		sequence[k].update();
	}
}
