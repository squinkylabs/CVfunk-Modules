# CV Funk Module Collection for VCV Rack

Explore the vast possibilities of modular synthesis with the CV Funk Module Collection, designed for VCV Rack. This suite of modules simplifies generating a complex spectrum of modulation from simple inputs. The modules are each carefully engineered to inject dynamic modulation, intricate sequencing, and immersive soundscapes into your musical creations. From the precise steps of the Penta Sequencer to the wide range of control of the Envelope Array, each module is designed to explore patch-programmable modular synthesis, inviting you to delve into the complexities of sound design with intuitive macro controls.

![Light CV funk Module Panels](/img/lightmodules.png)
![Dark CV funk Module Panels](/img/darkmodules.png)

## Modules Overview

### Penta Sequencer
A 5-step sequencer that redefines rhythmic and melodic structuring, offering Circular and Star modes for diverse sequencing, complete with directional control and adjustable slew for smooth transitions, all visually guided by intuitive step indicators.

### Comparator Stepper
A fusion of comparison logic and step sequencing, this module allows for dynamic pattern creation, influenced by external voltage levels. It provides hands-on control over bias, range, and step characteristics, complemented by clear visual feedback on the current state and progression.

### Envelope Array
An envelope generation powerhouse, the Envelope Array offers unparalleled control over the shape and dynamics of your sound. With six individually controllable stages, each featuring adjustable slant, curve, and time parameters, it opens up a realm of expressive modulation possibilities.

### Wave Propagation
Simulate the mesmerizing movement of waves through a network of nodes with the Wave Propagation module. This module not only brings a visual spectacle to your rack but also offers a unique approach to spatial sound design and dynamic modulation, with 24 outputs representing the ebb and flow of energy through the network.


<img src="/img/ComparatorStepperLight.png" alt="Image 1" height="500"> <img src="/img/ComparatorStepperDark.png" alt="Image 2" height="500">

# Comparator Stepper Module

The Comparator Stepper combines comparator functionalities with a step sequencer to provide precise control over step signal generation. The stepper increments a voltage in a voltage window, and can be used to generate CV for fixed-interval scales of any width.

## Features

- **Comparator with Adjustable Range**: Offers precision control over signal comparison, with adjustable bias and range parameters.
- **Step Sequencing**: Incorporates a step sequencer that modulates output based on a step size parameter with window-range feedback from the connected comparator, enhancing rhythmic and melodic possibilities.
- **Dynamic Step Modulation**: Features dynamic step size control, allowing for varied step increments influenced by external CV inputs.
- **Dual Comparator Outputs**: Provides separate outputs for 'above threshold' and 'below threshold' signals, enabling diverse signal routing options.
- **Visual Feedback**: Equipped with LED indicators for comparator status and step level, offering instant visual cues to the user.

## Usage

1. **Parameter Control**: Adjust the BIAS, RANGE, and STEP parameters to set the initial conditions for comparison and step modulation.
2. **Comparator Input**: Feed the signal to be compared into the COMPARATOR INPUT. The module compares this input against the internally set threshold, influenced by BIAS and RANGE parameters.
3. **Normalled Connections**:Using the comparator input breaks the normalization to the stepper section, allowing both sections to be used independently. When normalled, the comparator provides feedback voltagers to the stepper to bump it back into the window range after a step goes out of bounds. The lag of one step for corrections produces variation in the output.
4. **Interval Sequencing**: Utilize the STEP input to dynamically control the interval between steps. The step knob ranges from -1...1V, and is summed with the input. Inputs are scaled down 1/5, so that a -5...5V signal will sweep the full knob range for step size.
5. **External Modulation**: Patch signals into BIAS input, RANGE input, and INVERT input for dynamic control over the module's parameters.
6. **Trigger and Reset**: Use TRIGGER INPUT to advance the step sequencer and RESET input to reset the sequence to its initial state.

## Patch Suggestions

- **Dynamic Rhythms**: Create dynamic rhythmic patterns by modulating the STEP and BIAS parameters with LFOs or other modulation sources.
- **Melodic Sequencing**: Use the STEPPER OUTPUT to drive the pitch of an oscillator, crafting evolving melodic lines influenced by the comparator's operation.
- **Counting**: Adjust the ratio between RANGE and STEP size to derive the number of desired steps. For example setting RANGE to 4V and STEP size to 1V will result in a comparator gate to be outputted once every four steps.


<img src="/img/EnvelopeArrayLight.png" alt="Image 1" height="500"> <img src="/img/EnvelopeArrayDark.png" alt="Image 2" height="500">


# Envelope Array Module

The Envelope Array is an advanced envelope generator module for VCV Rack, designed to provide a wide array of dynamic control over amplitude shaping with six stages and complex modulation capabilities. The module can be self-patched to create a wide variety of modulations and is capable of self-oscillation when self-patched.

## Features

- **Multi-Stage Envelopes**: Six independent envelope generators with configurable time and curve settings for intricate modulation possibilities.
- **Curve and Slant Control**: Adjustable curve and slant parameters for shaping the envelope's attack and decay characteristics.
- **Attenuation Inputs**: Dedicated attenuation controls for slant, curve, and time parameters to fine-tune envelope responses.
- **End of Cycle Outputs**: Individual EOF outputs for each envelope stage, offering synchronization options with other modules.
- **Voltage Control**: Extensive CV inputs for real-time modulation of all key parameters, enhancing dynamic interaction within patches.
- **Polyphonic Capabilities**: Designed to support polyphonic inputs, enabling complex voice modulation within a polyphonic setup.

## Usage

1. **Set Envelope Stages**: Adjust TIME1 and TIME6 knobs to define the initial and final stages of the envelope's duration.
2. **Shape Modulation**: Use SLANT and CURVE knobs to shape the envelope's trajectory, creating everything from linear to exponential responses.
3. **Dynamic Control**: Patch CV signals into the respective attenuation inputs for real-time modulation of slant, curve, and time parameters.
4. **Monitor Envelopes**: Observe the module's LED indicators for real-time visual feedback on the envelope stages and end-of-cycle signals.
5. **Integrate with System**: Utilize the EOF outputs to trigger or synchronize with other modules, creating complex rhythmic patterns or sequences.

## Patch Suggestions

- **Complex Modulation**: Use the Envelope Array to modulate filter cutoffs, oscillator frequencies, or VCA amplitudes, creating evolving textures and timbres.
- **Rhythmic Sequencing**: Connect the EOF outputs to sequencer reset inputs, using the envelope stages to define rhythmic patterns.


<img src="/img/PentaSequencerLight.png" alt="Image 1" height="500"> <img src="/img/PentaSequencerDark.png" alt="Image 2" height="500">


# Penta Sequencer Module

The Penta Sequencer is a versatile 5-step sequencer module designed for VCV Rack, it offers some unique sequencing capabilities in a small footprint. The sequencer outputs all five of its notes simultaneously, and has a computed slew that adjusts dynamically to the trigger input. 

## Features

- **5-Step Sequencing**: Offers a concise yet powerful 5-step sequencing capability, suitable for generating rhythmic patterns and melodic sequences.
- **Multiple Modes**: Includes Circular (CIRC) and Star (STAR) sequencing modes, providing different traversal paths through the 5 steps for varied musical outcomes.
- **Direction Control**: Supports both Clockwise (CW) and Counterclockwise (CCW) directions, giving users the flexibility to reverse the sequence flow.
- **Dynamic Slew Control**: Features a slew control knob that adjusts the transition smoothness between steps, allowing for glide effects between voltages.
- **Trigger Input**: Advances the sequence with external trigger signals, enabling synchronization with other modules.
- **Reset Functionality**: Includes a reset input for returning the sequencer to its initial state on demand.
- **Visual Feedback**: Equipped with step lights under each knob to indicate the current step and mode-specific lights to provide visual cues for the sequencing path and direction.

## Usage


1. **Knob Assignment**: Each of the five knobs (KNOB1 to KNOB5) is mapped to one of the five outputs. Advancing the sequencer rotates the mapping between output and knob, indicated by lights around each output. The knob corresponding to the bottom-most active output is indicated by a light.
2. **Mode Selection**: A gate to the CIRC input will switch between Circular (0V) and Star (>5V) mappings when held. Changing modes will change the mapping of outputs between star and circular relative to the output indicating a 1.
3. **Direction Control**: A gate to the CW input will switch between Clockwise (0V) and Counterclockwise (>5V) mappings when held. Changing direction will exchange the left and right ouputs.
4. **Advancing the Sequence**: Send trigger signals or button press to the TRIG input to advance through the sequence according to the selected mode and direction.
5. **Resetting the Sequence**: Send a trigger signal to the RESET input to return the sequencer to the first step.
6. **Adjusting Slew**: Use the SLEW knob to control the transition smoothness between steps, the slew is synchronized to both the trigger timing and interval size, this means bigger octave jumps slew faster.

## Patch Suggestions

- **Slewed Octaves: Set each of the five knobs to -2, -1, 0, 1, 2.  Set the slew to 0.25 for smooth slewing. Patch each of the five outs to a WT-VCO, adjust each to have a slightly different timbre. With a second copy of the Penta Sequencer, set five different notes, set the slew of this one to 0.0, and ideally quantize the outputs before patching to the transpose input of the other sequencer. Now the slew will be perfectly in tune.

- **LFO: Set a large clock division to the trigger input, set the slew to 1.0. You will have five very slow LFOs. The slew interpolates using the time between the last two inputs (trigger/shape/direction) to compute the ramp.

- **Fixed Root: Reset the sequencer so that the bottom output is set to 1. Send gates to switch between Star/Circle/CCW/CW, the bottom output will always stay the same.

<img src="/img/WavePropagationLight.png" alt="Image 1" height="400"> <img src="/img/WavePropagationDark.png" alt="Image 2" height="400">

# Wave Propagation Module

The Wave Propagation module for VCV Rack simulates the propagation of waves through a medium, offering unique visual and auditory feedback based on the interactions within a network of nodes. Each of the 24 nodes can generate a decay envelope when stimulated, allowing for long decay tails and rhythmic synchronized pulsations.

## Features

- **Dynamic Wave Propagation**: Simulates the spread of waves across a network, with adjustable parameters for time, decay, and spread, affecting the speed, attenuation, and direction of wave propagation.
- **Multiple Outputs**: Provides 24 outputs, each representing a node in the wave propagation network, allowing for complex spatial audio effects.
- **Visual Feedback**: Equipped with a comprehensive set of lights, offering immediate visual representation of the wave propagation through the network.
- **Extensive Modulation Options**: Features dedicated inputs for time, decay, and spread parameters, enabling dynamic control over the wave propagation characteristics.

## Usage

1. **Initial Setup**: Adjust the TIME, DECAY, and SPREAD parameters to define the base characteristics of the wave propagation.
2. **Signal Input**: Introduce signals into the _00_INPUT to initiate wave propagation across the network.
3. **Modulation**: Utilize the dedicated TIME, DECAY, and SPREAD inputs for real-time modulation of wave characteristics.
4. **Output Utilization**: Patch outputs from the 24 nodes into various destinations to explore spatial audio effects and dynamic modulation.

## Patch Suggestions

- **Spatial Audio Design**: Use outputs to create immersive soundscapes, with wave propagation simulating movement across the stereo or surround field.
- **Dynamic Modulation Source**: Employ the module as a complex, evolving modulation source, with each node modulating different parameters in your patch.
