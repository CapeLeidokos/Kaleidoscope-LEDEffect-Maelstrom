# Kaleidoscope-LEDEffect-Maelstrom

Circular/rotating color effects for Kaleidoscope-driven keyboards.

# Introduction

## How it works

You can define one or two individual effects for each half of a keyboard.
For every half you must define a center point of the circular domain. Alternatively, only one center point can be used.

# Usage

## Setting up the plugin

The following examples are based on the keyboardio Model01.

### Using two individual effects

Add the following to your sketch.

```cpp
#include "Kaleidoscope-LEDEffect-Maelstrom.h"

// Define coordinate center points
//
constexpr RCC right_hand_center(7.247, 5.636);
constexpr RCC left_hand_center = right_hand_center.mirrorX();

// An auxiliary function that is used to determine which keys belong 
// to the right hand part of the keyboard.
//
constexpr bool isRightHandCoord(const RCC &coord) { return coord.x() > 0; }

// Issue the generation of key coordinates
//
LED_EFFECT_MAELSTROM_SETUP_SPLIT(isRightHandCoord, 
                                 left_hand_center, 
                                 right_hand_center)
                                                
...

KALEIDOSCOPE_INIT_PLUGINS(
   ...
   LEDControl,
   
   LEDEffectMaelstrom,
   ...
)

void setup() {
   ...
   // Choose any of the predefined LED modes
   //
   LEDEffectMaelstrom.setLeftHandColorFunction(&LEDEffectMaelstrom.colorModeSpinningCross);
   LEDEffectMaelstrom.setRightHandColorFunction(&LEDEffectMaelstrom.colorModeBullsEye);
   ...
}
```

### Using a single effects

Add the following to your sketch.

```cpp
#include "Kaleidoscope-LEDEffect-Maelstrom.h"

// Define a coordinate center point
//
constexpr RCC center(0, 5.636);

// Issue the generation of key coordinates
//
LED_EFFECT_MAELSTROM_SETUP(center)
                                                
...

KALEIDOSCOPE_INIT_PLUGINS(
   ...
   LEDControl,
   
   LEDEffectMaelstrom,
   ...
)

void setup() {
   ...
   // Choose any of the predefined LED modes (using the same effect for 
   // both hands is recommended).
   //
   LEDEffectMaelstrom.setLeftHandColorFunction(&LEDEffectMaelstrom.colorModeSpinningCross);
   LEDEffectMaelstrom.setRightHandColorFunction(&LEDEffectMaelstrom.colorModeSpinningCross);
   ...
}
```

### Using you own color function

Custom color function can be implemented and registered via the 
`setLeftHandColorFunction` and `setRightHandColorFunction` methods.
Check the implementation of the stock color modes to gain inspiration.

### You keyboard hardware does not support macro `KEY_COORDINATES`

No problem. Just implement you own version of `KEY_COORDINATES`. Check out
the keyboard hardware definition in the Kaleidoscope hardware src folder. The Keyboardio Model01 is a good example how `KEY_COORDINATES` can be defined for a split keyboard.

Add the definition of `KEY_COORDINATES` to the top of you sketch before 
`Kaleidoscope-LEDEffect-Maelstrom.h` is included.

