#pragma once
#include <rack.hpp>


using namespace rack;

// Declare the Plugin, defined in plugin.cpp
extern Plugin* pluginInstance;

// Declare each Model, defined in each module source file
extern Model* modelComparatorStepper;
extern Model* modelEnvelopeArray;
extern Model* modelPentaSequencer;
extern Model* modelWavePropagation;

extern Model* modelSignals;
