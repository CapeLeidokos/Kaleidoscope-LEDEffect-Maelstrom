#pragma once

#include "kaleidoscope/macro_map.h"
#include "kaleidoscope/plugin/LEDMode.h"
#include <kaleidoscope/coordinates_2D.h>
         
#ifndef _KEY_LED_COORDS_RIGHT_HAND_STACKED
#error "LEDEffect-Maelstrom requires the keyboard hardware to define the macro _KEY_LED_COORDS_RIGHT_HAND_STACKED"
#endif

#define COMPUTE_MAX_RADIUS(COMPUTE_REL_POINT_, ...) \
   kaleidoscope::coordinates_2D::maxNorm2(MAP_LIST(COMPUTE_REL_POINT_, __VA_ARGS__))

#define LED_EFFECT_WHIRL_GENERATE_COORDINATES_SPLIT(IS_RIGHT_HAND, \
                                                   COMPUTE_REL_POINT, \
                                                   LH_CENTER, \
                                                   RH_CENTER) \
   \
   namespace kaleidoscope { \
   namespace plugin { \
   namespace led_effect_whirl { \
      \
   constexpr double max_radius \
      = COMPUTE_MAX_RADIUS(COMPUTE_REL_POINT, \
                           _KEY_LED_COORDS_RIGHT_HAND_STACKED(RCC)); \
\
   constexpr IPC cartesianToUintPolarHanded(const RCC &point) { \
      return (IS_RIGHT_HAND(point)) ? \
            cartesianToUintPolar(point, RH_CENTER, max_radius) \
         :  cartesianToUintPolar(point, LH_CENTER, max_radius); \
   } \
   \
   const IPC PROGMEM pCoords[] = { MAP_LIST(cartesianToUintPolarHanded, \
                                          KEY_COORDINATES(RCC)) }; \
   } /* namespace led_effect_whirl */ \
   } /* namespace plugin */ \
   } /* namespace kaleidoscope */
   
#define LED_EFFECT_WHIRL_GENERATE_COORDINATES(COMPUTE_REL_POINT, CENTER) \
   LED_EFFECT_WHIRL_GENERATE_COORDINATES_SPLIT(true, COMPUTE_REL_POINT, CENTER, CENTER)
   
namespace kaleidoscope {
namespace plugin {
namespace led_effect_whirl {
   
typedef kaleidoscope::coordinates_2D::IntegerPolarCoordinate IPC;
typedef kaleidoscope::coordinates_2D::RealCartesianCoordinate RCC;

extern const IPC PROGMEM pCoords[];
} // namespace led_effect_whirl
   
class LEDEffectMaelstrom : public Plugin,
                       public LEDModeInterface 
{   
   public:
      
      typedef cRGB (*ColorModeFunction)(KeyAddr key_addr, uint16_t time_millis);
      
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
      
      void setLeftHandColorFunction(ColorModeFunction cf) { left_hand_color_function_ = cf; }
      void setRightHandColorFunction(ColorModeFunction cf) { right_hand_color_function_ = cf; }
      
      static cRGB colorModeWorm(KeyAddr key_addr, uint16_t time_millis);
      static cRGB colorModeMaelstrom(KeyAddr key_addr, uint16_t time_millis);
      static cRGB colorModePropeller(KeyAddr key_addr, uint16_t time_millis);
      static cRGB colorModeBullsEye(KeyAddr key_addr, uint16_t time_millis);
      
   private:
      
      ColorModeFunction left_hand_color_function_ = &colorModeWorm;
      ColorModeFunction right_hand_color_function_ = &colorModeBullsEye;
};

} // namespace plugin
} // namespace kaleidoscope

extern kaleidoscope::plugin::LEDEffectMaelstrom LEDEffectMaelstrom;
