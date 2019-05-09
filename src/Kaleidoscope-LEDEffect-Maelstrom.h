#pragma once

#include "kaleidoscope/macro_map.h"
#include "kaleidoscope/plugin/LEDMode.h"
#include "kaleidoscope/coordinates_2D.h"
#include "kaleidoscope/macro_helpers.h"
         
#ifndef KEY_COORDINATES
#error "LEDEffect-Maelstrom requires the keyboard hardware to define the macro KEY_COORDINATES"
#endif

#define LED_EFFECT_MAELSTROM_SETUP_SPLIT( IS_RIGHT_HAND,                       \
                                          LH_CENTER,                           \
                                          RH_CENTER)                           \
                                                                               \
   namespace kaleidoscope {                                              __NL__\
   namespace plugin {                                                    __NL__\
   namespace led_effect_whirl {                                          __NL__\
                                                                         __NL__\
   constexpr RCC leftHandRelPoint(const RCC &coord) {                    __NL__\
      return coord - LH_CENTER;                                          __NL__\
   }                                                                     __NL__\
   constexpr RCC rightHandRelPoint(const RCC &coord) {                   __NL__\
      return coord - RH_CENTER;                                          __NL__\
   }                                                                     __NL__\
                                                                         __NL__\
   constexpr RCC relPoint(const RCC &coord) {                            __NL__\
      return (IS_RIGHT_HAND(coord)) ?                                    __NL__\
            rightHandRelPoint(coord)                                     __NL__\
         :  leftHandRelPoint(coord);                                     __NL__\
   }                                                                     __NL__\
                                                                         __NL__\
   constexpr double max_radius                                           __NL__\
      = kaleidoscope::coordinates_2D::maxNorm2(                          __NL__\
          MAP_LIST(relPoint, KEY_COORDINATES(RCC)));                     __NL__\
                                                                         __NL__\
   constexpr IPC cartesianToUintPolarHanded(const RCC &point) {          __NL__\
      return (IS_RIGHT_HAND(point)) ?                                    __NL__\
            cartesianToUintPolar(point, RH_CENTER, max_radius)           __NL__\
         :  cartesianToUintPolar(point, LH_CENTER, max_radius);          __NL__\
   }                                                                     __NL__\
                                                                         __NL__\
   const IPC PROGMEM pCoords[] = { MAP_LIST(cartesianToUintPolarHanded,  __NL__\
                                          KEY_COORDINATES(RCC)) };       __NL__\
                                                                         __NL__\
   } /* namespace led_effect_whirl */                                    __NL__\
   } /* namespace plugin */                                              __NL__\
   } /* namespace kaleidoscope */
   
#define LED_EFFECT_MAELSTROM_SETUP(CENTER)                                     \
   LED_EFFECT_MAELSTROM_SETUP_SPLIT(alwaysTrue, CENTER, CENTER)
   
namespace kaleidoscope {
namespace plugin {
namespace led_effect_whirl {
   
typedef kaleidoscope::coordinates_2D::IntegerPolarCoordinate IPC;
typedef kaleidoscope::coordinates_2D::RealCartesianCoordinate RCC;

constexpr bool alwaysTrue(const RCC &) { return true; }

extern const IPC PROGMEM pCoords[];
} // namespace led_effect_whirl
   
class LEDEffectMaelstrom : public Plugin,
                       public LEDModeInterface 
{   
   public:
      
      typedef kaleidoscope::coordinates_2D::IntegerPolarCoordinate IPC;
      typedef cRGB (*ColorModeFunction)(IPC ipc, uint16_t time_millis);
      
      // This class' instance has dynamic lifetime
      //
      class TransientLEDMode : public LEDMode {
         
         public:

            // Please note that storing the parent ptr is only required
            // for those LED modes that require access to
            // members of their parent class. Most LED modes can do without.
            //
            TransientLEDMode(const LEDEffectMaelstrom *parent)
               : parent_(parent) {}

            virtual void update() final;
            
         private:
            
            const LEDEffectMaelstrom *parent_;
      };
      
      void setLeftHandColorFunction(ColorModeFunction cf) { 
         left_hand_color_function_ = cf; 
      }
      void setRightHandColorFunction(ColorModeFunction cf) { 
         right_hand_color_function_ = cf; 
      }
      
      static cRGB colorModeFirefly(IPC ipc, uint16_t time_millis);
      static cRGB colorModeMaelstrom(IPC ipc, uint16_t time_millis);
      static cRGB colorModePropeller(IPC ipc, uint16_t time_millis);
      static cRGB colorModeBullsEye(IPC ipc, uint16_t time_millis);
      static cRGB colorModeSpinningCross(IPC ipc, uint16_t time_millis);
      
   private:
      
      ColorModeFunction left_hand_color_function_ = nullptr;
      ColorModeFunction right_hand_color_function_ = nullptr;
};

} // namespace plugin
} // namespace kaleidoscope

extern kaleidoscope::plugin::LEDEffectMaelstrom LEDEffectMaelstrom;
