#include "plugin.hpp"

struct WavePropagation : Module {
	enum ParamId {
		TIME_PARAM,
		DECAY_PARAM,
		SPREAD_PARAM,
		ATT1_PARAM,  // Attenuverter for TIME_PARAM
        ATT2_PARAM,  // Attenuverter for DECAY_PARAM
        ATT3_PARAM,  // Attenuverter for SPREAD_PARAM
        TRIGGER_BUTTON,
		PARAMS_LEN
	};
	enum InputId {
		_00_INPUT,
		TIME_INPUT,
		DECAY_INPUT,
		SPREAD_INPUT,
		INPUTS_LEN
	};
	enum OutputId {
		_01_OUTPUT, _02_OUTPUT, _03_OUTPUT, _04_OUTPUT,
		_05_OUTPUT, _06_OUTPUT, _07_OUTPUT, _08_OUTPUT,
		_09_OUTPUT, _10_OUTPUT, _11_OUTPUT, _12_OUTPUT,
		_13_OUTPUT, _14_OUTPUT, _15_OUTPUT, _16_OUTPUT,
		_17_OUTPUT, _18_OUTPUT, _19_OUTPUT, _20_OUTPUT,
		_21_OUTPUT, _22_OUTPUT, _23_OUTPUT, _24_OUTPUT,
		OUTPUTS_LEN
	};
	enum LightId {
		_00A_LIGHT, _00B_LIGHT,
		_01A_LIGHT, _01B_LIGHT,
		_02A_LIGHT, _02B_LIGHT, _02C_LIGHT, _02D_LIGHT, _02E_LIGHT,
		_03A_LIGHT, _03B_LIGHT, _03C_LIGHT, _03D_LIGHT, _03E_LIGHT,
		_04A_LIGHT, _04B_LIGHT, _04C_LIGHT,
		_05A_LIGHT, _05B_LIGHT, _05C_LIGHT, _05D_LIGHT, _05E_LIGHT,
		_06A_LIGHT, _06B_LIGHT, _06C_LIGHT, _06D_LIGHT, _06E_LIGHT,
		_07A_LIGHT, _07B_LIGHT, _07C_LIGHT,
		_08A_LIGHT, _08B_LIGHT, _08C_LIGHT, _08D_LIGHT, _08E_LIGHT,
		_09A_LIGHT, _09B_LIGHT, _09C_LIGHT, _09D_LIGHT, _09E_LIGHT,
		_10A_LIGHT, _10B_LIGHT,
		_11A_LIGHT, _11B_LIGHT,
		_12A_LIGHT, _12B_LIGHT, _12C_LIGHT, _12D_LIGHT, _12E_LIGHT,
		_13A_LIGHT, _13B_LIGHT, _13C_LIGHT, _13D_LIGHT, _13E_LIGHT,
		_14A_LIGHT, _14B_LIGHT,
		_15A_LIGHT, _15B_LIGHT,
		_00OUT_LIGHT,
		_01OUT_LIGHT, _02OUT_LIGHT, _03OUT_LIGHT, _04OUT_LIGHT,
		_05OUT_LIGHT, _06OUT_LIGHT, _07OUT_LIGHT, _08OUT_LIGHT,
		_09OUT_LIGHT, _10OUT_LIGHT, _11OUT_LIGHT, _12OUT_LIGHT,
		_13OUT_LIGHT, _14OUT_LIGHT, _15OUT_LIGHT, _16OUT_LIGHT,
		_17OUT_LIGHT, _18OUT_LIGHT, _19OUT_LIGHT, _20OUT_LIGHT,
		_21OUT_LIGHT, _22OUT_LIGHT, _23OUT_LIGHT, _24OUT_LIGHT,
		LIGHTS_LEN
	};
	
	// Define an array to store time variables
    float time_x[24] = {0.0f}; // Time interval for each light group
    float groupElapsedTime[24] = {}; // Elapsed time since the last activation for each light group

	// Active nodes management
	std::set<int> activeNodes; // Holds all currently active nodes

	//Keep track of input states so that we can avoid retriggering on gates
    bool previousInputState = false; 

	//Keep track of the time the one input is above the threshold
	float inputAboveThresholdTime = 0.0f; // Time in seconds

	// Define groups of lights
	std::vector<std::vector<LightId>> lightGroups = {
		{_01OUT_LIGHT, _00A_LIGHT, _00B_LIGHT, _01A_LIGHT},
		{_02OUT_LIGHT, _01B_LIGHT, _02A_LIGHT, _02C_LIGHT, _02D_LIGHT},
		{_03OUT_LIGHT, _02B_LIGHT, _03A_LIGHT, _03C_LIGHT, _03D_LIGHT},	
		{_04OUT_LIGHT, _02E_LIGHT, _04A_LIGHT, _04B_LIGHT},	 
		{_05OUT_LIGHT, _03B_LIGHT, _05A_LIGHT, _05C_LIGHT, _05D_LIGHT},
		{_06OUT_LIGHT, _04C_LIGHT, _06A_LIGHT, _06C_LIGHT, _06D_LIGHT},		
		{_07OUT_LIGHT, _03E_LIGHT, _07A_LIGHT, _07B_LIGHT},		
		{_08OUT_LIGHT, _05B_LIGHT, _08A_LIGHT, _08C_LIGHT, _08D_LIGHT},		
		{_09OUT_LIGHT, _06B_LIGHT, _09A_LIGHT, _09C_LIGHT, _09D_LIGHT},
		{_10OUT_LIGHT, _07C_LIGHT, _10A_LIGHT},
		{_11OUT_LIGHT, _05E_LIGHT, _11A_LIGHT},
		{_12OUT_LIGHT, _08E_LIGHT, _12A_LIGHT, _12C_LIGHT, _12D_LIGHT},
		{_13OUT_LIGHT, _08B_LIGHT, _13A_LIGHT, _13C_LIGHT, _13D_LIGHT},
		{_14OUT_LIGHT, _09E_LIGHT, _14A_LIGHT}, 
		{_15OUT_LIGHT, _09B_LIGHT, _15A_LIGHT},
		{_16OUT_LIGHT, _11B_LIGHT},
		{_17OUT_LIGHT, _10B_LIGHT},
		{_18OUT_LIGHT, _13E_LIGHT},
		{_19OUT_LIGHT, _06E_LIGHT},
		{_20OUT_LIGHT, _12B_LIGHT},
		{_21OUT_LIGHT, _13B_LIGHT},
		{_22OUT_LIGHT, _12E_LIGHT},
		{_23OUT_LIGHT, _14B_LIGHT},
		{_24OUT_LIGHT, _15B_LIGHT},
	};

	//Define the node-connected graph structure		
	std::map<int, std::vector<int>> nodeConnections = {
		{0, {1}},
		{1, {2, 3}},
		{2, {4, 6}},
		{3, {5}},
		{4, {7, 10}},
		{5, {8, 18}},
		{6, {9}},
		{7, {11, 12}},
		{8, {13, 14}},
		{9, {16}},
		{10, {15}},
		{11, {19, 21}},
		{12, {17, 20}},
		{13, {22}},
		{14, {23}}
	};
	
	WavePropagation() {
		config(PARAMS_LEN, INPUTS_LEN, OUTPUTS_LEN, LIGHTS_LEN);
		configParam(TIME_PARAM, 0.0f, 1.0f, 0.5f, "");
		configParam(SPREAD_PARAM, -1.0f, 1.f, 0.5f, "");
		configParam(DECAY_PARAM, 0.0f, 1.0f, 0.5f, "");
		
		configParam(ATT1_PARAM, -1.0f, 1.0f, 1.0f, "Time Attenuverter");
		configParam(ATT2_PARAM, -1.0f, 1.0f, 1.0f, "Decay Attenuverter");
		configParam(ATT3_PARAM, -1.0f, 1.0f, 1.0f, "Spread Attenuverter");

		configInput(_00_INPUT, "");
		configInput(TIME_INPUT, "");
		configInput(SPREAD_INPUT, "");
		configInput(DECAY_INPUT, "");
		configOutput(_01_OUTPUT, ""); configOutput(_02_OUTPUT, "");
		configOutput(_03_OUTPUT, ""); configOutput(_04_OUTPUT, "");
		configOutput(_05_OUTPUT, ""); configOutput(_06_OUTPUT, "");
		configOutput(_07_OUTPUT, ""); configOutput(_08_OUTPUT, "");
		configOutput(_09_OUTPUT, ""); configOutput(_10_OUTPUT, "");
		configOutput(_11_OUTPUT, ""); configOutput(_12_OUTPUT, "");
		configOutput(_13_OUTPUT, ""); configOutput(_14_OUTPUT, "");
		configOutput(_15_OUTPUT, ""); configOutput(_16_OUTPUT, "");
		configOutput(_17_OUTPUT, ""); configOutput(_18_OUTPUT, "");
		configOutput(_19_OUTPUT, ""); configOutput(_20_OUTPUT, "");
		configOutput(_21_OUTPUT, ""); configOutput(_22_OUTPUT, "");
		configOutput(_23_OUTPUT, ""); configOutput(_24_OUTPUT, "");
	}

    void process(const ProcessArgs& args) override {

		//Process inputs to paramaters
		float decay = params[DECAY_PARAM].getValue();
		float spread = params[SPREAD_PARAM].getValue();
		 
		time_x[0] = params[TIME_PARAM].getValue(); //Time interval for the first generator

		if (inputs[DECAY_INPUT].isConnected())
			decay += inputs[DECAY_INPUT].getVoltage()*0.1*params[ATT2_PARAM].getValue();
		if (inputs[SPREAD_INPUT].isConnected())
			spread += inputs[SPREAD_INPUT].getVoltage()*0.1*params[ATT3_PARAM].getValue();
		if (inputs[TIME_INPUT].isConnected())
			time_x[0] += inputs[TIME_INPUT].getVoltage()*0.1*params[ATT1_PARAM].getValue();

		// Clamp the param values after adding voltages
		decay = clamp(decay, 0.00f, 1.0f); 
		spread = clamp(spread, -1.00f, 1.0f);
		time_x[0] = clamp(time_x[0], 0.0f, 1.0f);


		// Apply non-linear re-scaling to parameters to make them feel better
		time_x[0] = pow(time_x[0], 2); //
		spread = (spread >= 0 ? 1 : -1) * pow(abs(spread), 4);
		decay = 1 - pow(1 - decay, 2); 


		// Re-Clamp the param values after non-linear scaling
		decay = clamp(decay, 0.02f, .98f); 
		spread = clamp(spread, -1.00f, 1.0f);
		time_x[0] = clamp(time_x[0], 0.0f, 1.0f);


		// Compute time parameters for subsequent nodes
		for (int i = 1; i < 24; i++) {
			time_x[i] = (1 + spread) * time_x[i - 1];
		}

		// Initialize a set to keep track of nodes that will become active
		std::set<int> nodesToActivate;

		//Set detection threshold based on the spread input		
		float detect_thresh = 2.0f * (-spread + 1);
//
		// Detect a rising signal at 1.0f on _00_INPUT or manual trigger button press to potentially activate node 0
		bool manualTriggerPressed = params[TRIGGER_BUTTON].getValue() > 0.0f;  // Assuming TRIGGER_BUTTON is the ID for your manual trigger button
		bool currentInputState = (inputs[_00_INPUT].isConnected() && inputs[_00_INPUT].getVoltage() > 1.0f) || manualTriggerPressed;

		if (currentInputState && !previousInputState) {
			// Check if node 0 is considered inactive based on its output voltage
			if (outputs[_01_OUTPUT].getVoltage() < detect_thresh) {
				activeNodes.insert(0);  // Reactivate node 0
				groupElapsedTime[0] = 0.f;  // Reset elapsed time for node 0
				for (LightId light : lightGroups[0]) {
					lights[light].setBrightness(1.0f);  // Turn on all lights for node 0's group
				}
			}
		}
		previousInputState = currentInputState;  // Update previous input state

		// Reset the trigger button state after processing to ensure it is ready for the next press
		if (manualTriggerPressed) {
			params[TRIGGER_BUTTON].setValue(0.0f);
		}		
//

		// Temporary set for nodes to deactivate
		std::set<int> nodesToDeactivate;
		
		// Iterate over all active nodes to update their states
		for (int node : activeNodes) {
			groupElapsedTime[node] += args.sampleTime; // Increment elapsed time

			float brightness = lights[_01OUT_LIGHT + node].getBrightness(); // Get the brightness of the corresponding light
	
			// Check if it's time to deactivate the current node
			if (groupElapsedTime[node] >= (time_x[node]+spread) ) {
				if (brightness < detect_thresh ){
					nodesToDeactivate.insert(node); // Schedule for deactivation			
				}
			}
		}
		
		// Deactivate nodes and potentially activate their child nodes
		for (int node : nodesToDeactivate) {
			activeNodes.erase(node); // Deactivate the node
			groupElapsedTime[node] = 0.f; // Reset elapsed time
				
			// Check and activate child nodes if they are considered inactive
			auto children = nodeConnections.find(node);
			if (children != nodeConnections.end()) {
				for (int childNode : children->second) {
					// Check if the child node is considered inactive
					if (outputs[_01_OUTPUT + childNode].getVoltage() < detect_thresh  ) { 
						activeNodes.insert(childNode); // Activate the child node
						groupElapsedTime[childNode] = 0.f; // Reset elapsed time for the child node
						for (LightId light : lightGroups[childNode]) {
							lights[light].setBrightness(1.0f); // Turn on lights for the child node's group
						}
					}
				}
			}
		}

		// Map OUT light brightness to OUTPUT voltages
		for (int i = 0; i < 24; ++i) {
			// Directly map the light brightness to output voltage
			float brightness = lights[_01OUT_LIGHT + i].getBrightness(); // Get the brightness of the corresponding light
			outputs[_01_OUTPUT + i].setVoltage(brightness * 10.0f); // Set the output voltage based on the light brightness
		}
			
		// Dim lights slowly for each light group
		for (int groupIndex = 0; groupIndex < lightGroups.size(); ++groupIndex) {
			// Calculate the dimming factor for the current group
			float dimmingFactor = decay + ((1.0f - decay) * (spread * 0.7f) ) * (groupIndex / 23.f);
		
			// Apply the dimming factor to each light within the current group
			for (LightId lightId : lightGroups[groupIndex]) {
				float how_bright = lights[lightId].getBrightness();
				how_bright *= dimmingFactor; 
				lights[lightId].setSmoothBrightness(how_bright, args.sampleTime);
			}
		} 
  
		// Iterate over all possible nodes
		for (int node = 0; node < 24; ++node) { 
			// Check if the node is active
			if (activeNodes.find(node) != activeNodes.end()) {
				// Retrieve the voltage for the current node's output
				float voltage = outputs[_01_OUTPUT + node].getVoltage(); 

				// Check if the output voltage is low
				if (voltage < 0.001f) {
					// Deactivate the node by removing it from the active set
					activeNodes.erase(node);

				 }
			}
		}

		if (inputs[_00_INPUT].isConnected()){
			float brightness = inputs[_00_INPUT].getVoltage() / 10.0f;
			lights[_00OUT_LIGHT].setSmoothBrightness(brightness, args.sampleTime);         
		}		
				 									
	} // void process
}; //struct

struct WavePropagationWidget : ModuleWidget {
	WavePropagationWidget(WavePropagation* module) {
		setModule(module);

		setPanel(createPanel(
			asset::plugin(pluginInstance, "res/WavePropagation.svg"),
			asset::plugin(pluginInstance, "res/WavePropagation-dark.svg")
		));


		addChild(createWidget<ThemedScrew>(Vec(RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ThemedScrew>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ThemedScrew>(Vec(RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));
		addChild(createWidget<ThemedScrew>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));


        addParam(createParamCentered<TL1105>(mm2px(Vec(10.916, 65)), module, WavePropagation::TRIGGER_BUTTON));


		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(10.916, 72.73)), module, WavePropagation::_00_INPUT));

        // Attenuverter Knobs
        addParam(createParamCentered<Trimpot>(mm2px(Vec(11.064, 35.728)), module, WavePropagation::ATT1_PARAM));
        addParam(createParamCentered<Trimpot>(mm2px(Vec(29.756, 35.728)), module, WavePropagation::ATT2_PARAM));
        addParam(createParamCentered<Trimpot>(mm2px(Vec(48.449, 35.728)), module, WavePropagation::ATT3_PARAM));


		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(10.957, 24)), module, WavePropagation::TIME_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(29.649, 24)), module, WavePropagation::SPREAD_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(48.342, 24)), module, WavePropagation::DECAY_PARAM));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(11.171, 45.049)), module, WavePropagation::TIME_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(29.864, 45.049)), module, WavePropagation::SPREAD_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(48.556, 45.049)), module, WavePropagation::DECAY_INPUT));


		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(29.445, 72.73)), module, WavePropagation::_01_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(47.974, 72.73)), module, WavePropagation::_02_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(66.503, 72.73)), module, WavePropagation::_03_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(66.503, 54.201)), module, WavePropagation::_04_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(85.031, 72.73)), module, WavePropagation::_05_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(85.031, 35.672)), module, WavePropagation::_06_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(85.031, 91.258)), module, WavePropagation::_07_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(103.56, 72.73)), module, WavePropagation::_08_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(103.56, 35.672)), module, WavePropagation::_09_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(103.56, 109.656)), module, WavePropagation::_10_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(103.56, 54.201)), module, WavePropagation::_11_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(122.089, 91.258)), module, WavePropagation::_12_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(122.089, 72.73)), module, WavePropagation::_13_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(122.089, 17.144)), module, WavePropagation::_14_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(122.089, 35.672)), module, WavePropagation::_15_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(122.089, 54.201)), module, WavePropagation::_16_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(122.089, 109.656)), module, WavePropagation::_17_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(140.618, 54.201)), module, WavePropagation::_18_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(103.56, 17.144)), module, WavePropagation::_19_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(140.618, 91.258)), module, WavePropagation::_20_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(140.618, 72.73)), module, WavePropagation::_21_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(140.618, 109.656)), module, WavePropagation::_22_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(140.618, 17.144)), module, WavePropagation::_23_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(140.618, 35.672)), module, WavePropagation::_24_OUTPUT));

		addChild(createLightCentered<SmallLight<RedLight>>(mm2px(Vec(20.181, 72.73)), module, WavePropagation::_00A_LIGHT));
		addChild(createLightCentered<SmallLight<RedLight>>(mm2px(Vec(23.139, 72.73)), module, WavePropagation::_00B_LIGHT));
		addChild(createLightCentered<SmallLight<RedLight>>(mm2px(Vec(38.706, 72.73)), module, WavePropagation::_01A_LIGHT));
		addChild(createLightCentered<SmallLight<RedLight>>(mm2px(Vec(41.664, 72.73)), module, WavePropagation::_01B_LIGHT));
		addChild(createLightCentered<SmallLight<RedLight>>(mm2px(Vec(57.235, 72.73)), module, WavePropagation::_02A_LIGHT));
		addChild(createLightCentered<SmallLight<RedLight>>(mm2px(Vec(60.193, 72.73)), module, WavePropagation::_02B_LIGHT));
		addChild(createLightCentered<SmallLight<RedLight>>(mm2px(Vec(54.284, 66.42)), module, WavePropagation::_02C_LIGHT));
		addChild(createLightCentered<SmallLight<RedLight>>(mm2px(Vec(57.236, 63.468)), module, WavePropagation::_02D_LIGHT));
		addChild(createLightCentered<SmallLight<RedLight>>(mm2px(Vec(60.193, 60.511)), module, WavePropagation::_02E_LIGHT));
		addChild(createLightCentered<SmallLight<RedLight>>(mm2px(Vec(75.764, 72.73)), module, WavePropagation::_03A_LIGHT));
		addChild(createLightCentered<SmallLight<RedLight>>(mm2px(Vec(78.721, 72.73)), module, WavePropagation::_03B_LIGHT));
		addChild(createLightCentered<SmallLight<RedLight>>(mm2px(Vec(72.813, 79.04)), module, WavePropagation::_03C_LIGHT));
		addChild(createLightCentered<SmallLight<RedLight>>(mm2px(Vec(75.765, 81.992)), module, WavePropagation::_03D_LIGHT));
		addChild(createLightCentered<SmallLight<RedLight>>(mm2px(Vec(78.722, 84.949)), module, WavePropagation::_03E_LIGHT));
		addChild(createLightCentered<SmallLight<RedLight>>(mm2px(Vec(72.813, 47.891)), module, WavePropagation::_04A_LIGHT));
		addChild(createLightCentered<SmallLight<RedLight>>(mm2px(Vec(75.765, 44.939)), module, WavePropagation::_04B_LIGHT));
		addChild(createLightCentered<SmallLight<RedLight>>(mm2px(Vec(78.722, 41.982)), module, WavePropagation::_04C_LIGHT));
		addChild(createLightCentered<SmallLight<RedLight>>(mm2px(Vec(94.292, 72.73)), module, WavePropagation::_05A_LIGHT));
		addChild(createLightCentered<SmallLight<RedLight>>(mm2px(Vec(97.25, 72.73)), module, WavePropagation::_05B_LIGHT));
		addChild(createLightCentered<SmallLight<RedLight>>(mm2px(Vec(91.342, 66.42)), module, WavePropagation::_05C_LIGHT));
		addChild(createLightCentered<SmallLight<RedLight>>(mm2px(Vec(94.294, 63.468)), module, WavePropagation::_05D_LIGHT));
		addChild(createLightCentered<SmallLight<RedLight>>(mm2px(Vec(97.25, 60.511)), module, WavePropagation::_05E_LIGHT));
		addChild(createLightCentered<SmallLight<RedLight>>(mm2px(Vec(94.292, 35.672)), module, WavePropagation::_06A_LIGHT));
		addChild(createLightCentered<SmallLight<RedLight>>(mm2px(Vec(97.25, 35.672)), module, WavePropagation::_06B_LIGHT));
		addChild(createLightCentered<SmallLight<RedLight>>(mm2px(Vec(91.342, 29.362)), module, WavePropagation::_06C_LIGHT));
		addChild(createLightCentered<SmallLight<RedLight>>(mm2px(Vec(94.294, 26.41)), module, WavePropagation::_06D_LIGHT));
		addChild(createLightCentered<SmallLight<RedLight>>(mm2px(Vec(97.25, 23.454)), module, WavePropagation::_06E_LIGHT));
		addChild(createLightCentered<SmallLight<RedLight>>(mm2px(Vec(91.342, 97.569)), module, WavePropagation::_07A_LIGHT));
		addChild(createLightCentered<SmallLight<RedLight>>(mm2px(Vec(94.294, 100.521)), module, WavePropagation::_07B_LIGHT));
		addChild(createLightCentered<SmallLight<RedLight>>(mm2px(Vec(97.25, 103.478)), module, WavePropagation::_07C_LIGHT));
		addChild(createLightCentered<SmallLight<RedLight>>(mm2px(Vec(112.821, 72.73)), module, WavePropagation::_08A_LIGHT));
		addChild(createLightCentered<SmallLight<RedLight>>(mm2px(Vec(115.779, 72.73)), module, WavePropagation::_08B_LIGHT));
		addChild(createLightCentered<SmallLight<RedLight>>(mm2px(Vec(109.871, 79.04)), module, WavePropagation::_08C_LIGHT));
		addChild(createLightCentered<SmallLight<RedLight>>(mm2px(Vec(112.823, 81.992)), module, WavePropagation::_08D_LIGHT));
		addChild(createLightCentered<SmallLight<RedLight>>(mm2px(Vec(115.779, 84.949)), module, WavePropagation::_08E_LIGHT));
		addChild(createLightCentered<SmallLight<RedLight>>(mm2px(Vec(112.821, 35.672)), module, WavePropagation::_09A_LIGHT));
		addChild(createLightCentered<SmallLight<RedLight>>(mm2px(Vec(115.779, 35.672)), module, WavePropagation::_09B_LIGHT));
		addChild(createLightCentered<SmallLight<RedLight>>(mm2px(Vec(109.871, 29.362)), module, WavePropagation::_09C_LIGHT));
		addChild(createLightCentered<SmallLight<RedLight>>(mm2px(Vec(112.823, 26.41)), module, WavePropagation::_09D_LIGHT));
		addChild(createLightCentered<SmallLight<RedLight>>(mm2px(Vec(115.779, 23.454)), module, WavePropagation::_09E_LIGHT));
		addChild(createLightCentered<SmallLight<RedLight>>(mm2px(Vec(112.821, 109.656)), module, WavePropagation::_10A_LIGHT));
		addChild(createLightCentered<SmallLight<RedLight>>(mm2px(Vec(115.779, 109.656)), module, WavePropagation::_10B_LIGHT));
		addChild(createLightCentered<SmallLight<RedLight>>(mm2px(Vec(112.821, 54.201)), module, WavePropagation::_11A_LIGHT));
		addChild(createLightCentered<SmallLight<RedLight>>(mm2px(Vec(115.779, 54.201)), module, WavePropagation::_11B_LIGHT));
		addChild(createLightCentered<SmallLight<RedLight>>(mm2px(Vec(131.35, 91.258)), module, WavePropagation::_12A_LIGHT));
		addChild(createLightCentered<SmallLight<RedLight>>(mm2px(Vec(134.308, 91.258)), module, WavePropagation::_12B_LIGHT));
		addChild(createLightCentered<SmallLight<RedLight>>(mm2px(Vec(128.4, 97.569)), module, WavePropagation::_12C_LIGHT));
		addChild(createLightCentered<SmallLight<RedLight>>(mm2px(Vec(131.352, 100.521)), module, WavePropagation::_12D_LIGHT));
		addChild(createLightCentered<SmallLight<RedLight>>(mm2px(Vec(134.308, 103.478)), module, WavePropagation::_12E_LIGHT));
		addChild(createLightCentered<SmallLight<RedLight>>(mm2px(Vec(131.35, 72.73)), module, WavePropagation::_13A_LIGHT));
		addChild(createLightCentered<SmallLight<RedLight>>(mm2px(Vec(134.308, 72.73)), module, WavePropagation::_13B_LIGHT));
		addChild(createLightCentered<SmallLight<RedLight>>(mm2px(Vec(128.4, 66.42)), module, WavePropagation::_13C_LIGHT));
		addChild(createLightCentered<SmallLight<RedLight>>(mm2px(Vec(131.352, 63.468)), module, WavePropagation::_13D_LIGHT));
		addChild(createLightCentered<SmallLight<RedLight>>(mm2px(Vec(134.308, 60.511)), module, WavePropagation::_13E_LIGHT));
		addChild(createLightCentered<SmallLight<RedLight>>(mm2px(Vec(131.35, 17.144)), module, WavePropagation::_14A_LIGHT));
		addChild(createLightCentered<SmallLight<RedLight>>(mm2px(Vec(134.308, 17.144)), module, WavePropagation::_14B_LIGHT));
		addChild(createLightCentered<SmallLight<RedLight>>(mm2px(Vec(131.35, 35.672)), module, WavePropagation::_15A_LIGHT));
		addChild(createLightCentered<SmallLight<RedLight>>(mm2px(Vec(134.308, 35.672)), module, WavePropagation::_15B_LIGHT));

		addChild(createLightCentered<MediumLight<YellowLight>>(mm2px(Vec(17.23, 72.73)), module, WavePropagation::_00OUT_LIGHT));
		addChild(createLightCentered<MediumLight<YellowLight>>(mm2px(Vec(35.756, 72.73)), module, WavePropagation::_01OUT_LIGHT));
		addChild(createLightCentered<MediumLight<YellowLight>>(mm2px(Vec(54.284, 72.73)), module, WavePropagation::_02OUT_LIGHT));
		addChild(createLightCentered<MediumLight<YellowLight>>(mm2px(Vec(72.813, 72.73)), module, WavePropagation::_03OUT_LIGHT));
		addChild(createLightCentered<MediumLight<YellowLight>>(mm2px(Vec(72.813, 54.426)), module, WavePropagation::_04OUT_LIGHT));
		addChild(createLightCentered<MediumLight<YellowLight>>(mm2px(Vec(91.342, 72.73)), module, WavePropagation::_05OUT_LIGHT));
		addChild(createLightCentered<MediumLight<YellowLight>>(mm2px(Vec(91.342, 35.672)), module, WavePropagation::_06OUT_LIGHT));
		addChild(createLightCentered<MediumLight<YellowLight>>(mm2px(Vec(91.342, 91.258)), module, WavePropagation::_07OUT_LIGHT));
		addChild(createLightCentered<MediumLight<YellowLight>>(mm2px(Vec(109.871, 72.73)), module, WavePropagation::_08OUT_LIGHT));
		addChild(createLightCentered<MediumLight<YellowLight>>(mm2px(Vec(109.871, 35.672)), module, WavePropagation::_09OUT_LIGHT));
		addChild(createLightCentered<MediumLight<YellowLight>>(mm2px(Vec(109.871, 109.656)), module, WavePropagation::_10OUT_LIGHT));
		addChild(createLightCentered<MediumLight<YellowLight>>(mm2px(Vec(109.871, 54.201)), module, WavePropagation::_11OUT_LIGHT));
		addChild(createLightCentered<MediumLight<YellowLight>>(mm2px(Vec(128.4, 91.258)), module, WavePropagation::_12OUT_LIGHT));
		addChild(createLightCentered<MediumLight<YellowLight>>(mm2px(Vec(128.4, 72.73)), module, WavePropagation::_13OUT_LIGHT));
		addChild(createLightCentered<MediumLight<YellowLight>>(mm2px(Vec(128.4, 17.144)), module, WavePropagation::_14OUT_LIGHT));
		addChild(createLightCentered<MediumLight<YellowLight>>(mm2px(Vec(128.4, 35.672)), module, WavePropagation::_15OUT_LIGHT));
		addChild(createLightCentered<MediumLight<YellowLight>>(mm2px(Vec(129.392, 54.096)), module, WavePropagation::_16OUT_LIGHT));
		addChild(createLightCentered<MediumLight<YellowLight>>(mm2px(Vec(128.53, 109.824)), module, WavePropagation::_17OUT_LIGHT));
		addChild(createLightCentered<MediumLight<YellowLight>>(mm2px(Vec(146.928, 54.201)), module, WavePropagation::_18OUT_LIGHT));
		addChild(createLightCentered<MediumLight<YellowLight>>(mm2px(Vec(109.871, 17.144)), module, WavePropagation::_19OUT_LIGHT));
		addChild(createLightCentered<MediumLight<YellowLight>>(mm2px(Vec(146.928, 91.258)), module, WavePropagation::_20OUT_LIGHT));
		addChild(createLightCentered<MediumLight<YellowLight>>(mm2px(Vec(146.928, 72.73)), module, WavePropagation::_21OUT_LIGHT));
		addChild(createLightCentered<MediumLight<YellowLight>>(mm2px(Vec(146.928, 109.656)), module, WavePropagation::_22OUT_LIGHT));
		addChild(createLightCentered<MediumLight<YellowLight>>(mm2px(Vec(146.928, 17.144)), module, WavePropagation::_23OUT_LIGHT));
		addChild(createLightCentered<MediumLight<YellowLight>>(mm2px(Vec(146.928, 35.672)), module, WavePropagation::_24OUT_LIGHT));
	}
};

Model* modelWavePropagation = createModel<WavePropagation, WavePropagationWidget>("WavePropagation");