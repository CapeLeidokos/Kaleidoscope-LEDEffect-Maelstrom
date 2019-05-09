#include <Kaleidoscope.h>
#include <Kaleidoscope-LEDEffect-Maelstrom.h>
#include <kaleidoscope/plugin/LEDControl.h>
#include <kaleidoscope/plugin/LEDControl/LEDUtils.h>
#include <kaleidoscope/list_helpers.h>
#include <Kaleidoscope.h>

#include <stdint.h>
#include <math.h>
   
typedef kaleidoscope::coordinates_2D::RealCartesianCoordinate RCC;
typedef kaleidoscope::coordinates_2D::IntegerPolarCoordinate IPC;

constexpr cRGB black{CRGB(0, 0, 0)};
constexpr cRGB green{CRGB(0, 255, 0)};

namespace kaleidoscope {
namespace plugin {
namespace led_effect_whirl {
                                         
IPC readPolarCoord(KeyAddr key_addr) {
   IPC ipc;
   memcpy_P(&ipc, &led_effect_whirl::pCoords[key_addr.toInt()], sizeof(IPC));
   return ipc;
}

} // namespace led_effect_whirl

cRGB LEDEffectMaelstrom::colorModeFirefly(IPC ipc, uint16_t time_millis) {
   
   uint8_t t_angle = (time_millis >> 3) % 255;
   
   constexpr uint16_t angle_max = 64; // length of the worm at full radius
   
   uint16_t angle_range_test = (ipc.radius() > 0) ? (angle_max*255)/ipc.radius() : 255;
   uint8_t angle_range = (angle_range_test < 255) ? angle_range_test : 255;
   
   uint8_t angle_lower_bound = t_angle;
   uint8_t angle_upper_bound = (uint16_t(t_angle) + angle_range) % 255;
   
   if(angle_lower_bound < angle_upper_bound) {
      if((ipc.angle() < angle_lower_bound) || (ipc.angle() > angle_upper_bound)) {
         return black;
      }
   }
   else {
      if((ipc.angle() < angle_lower_bound) && (ipc.angle() > angle_upper_bound)) {
         return black;
      }
   }
   
   uint8_t t_radius = uint8_t(time_millis >> 5) % 255;
   
   constexpr uint8_t worm_thickness = 30;
   
   uint8_t radius_upper_bound = 255 - t_radius;
   uint8_t radius_lower_bound = radius_upper_bound - worm_thickness;
   
   if(ipc.radius() < radius_lower_bound) { return black; }
   if(ipc.radius() > radius_upper_bound) { return black; }
   
   uint8_t rel_angle = (uint16_t(ipc.angle()) - angle_lower_bound)*255/angle_range;

   constexpr uint8_t base_hue = 50;
   
//    return hsvToRgb(ipc.radius(), 255, 255);
   return hsvToRgb(base_hue, rel_angle, rel_angle);
//    return CRGB(rel_angle, rel_angle, rel_angle);
}

cRGB LEDEffectMaelstrom::colorModeMaelstrom(IPC ipc, uint16_t time_millis) {
 
   return hsvToRgb((uint16_t(ipc.radius()) + (time_millis >> 3))%255,
                   255,
                   ((uint16_t(ipc.angle()) << 1) + (time_millis >> 2))%255);
}

cRGB LEDEffectMaelstrom::colorModePropeller(IPC ipc, uint16_t time_millis) {
   
   uint8_t mAngle = (uint16_t(ipc.angle() << 2)) % 255;
   
   uint8_t t_angle = uint8_t(time_millis >> 3) % 255;

   constexpr uint8_t angle_range = 60;
   
   uint8_t angle_lower_bound = t_angle;
   uint8_t angle_upper_bound = (uint16_t(t_angle) + angle_range) % 255;
   
   if(angle_lower_bound < angle_upper_bound) {
      if((mAngle < angle_lower_bound) || (mAngle > angle_upper_bound)) {
         return black;
      }
   }
   else {
      if((mAngle < angle_lower_bound) && (mAngle > angle_upper_bound)) {
         return black;
      }
   }
   
   return green;
}

cRGB LEDEffectMaelstrom::colorModeBullsEye(IPC ipc, uint16_t time_millis) {
   
   uint8_t t_radius = uint8_t(time_millis >> 2) % 255;
   
   if(((uint16_t(ipc.radius()) - t_radius) >> 7) % 2) {
      return CRGB(255, 0, 0);
   }
   
   return CRGB(255, 255, 255);
}

cRGB LEDEffectMaelstrom::colorModeSpinningCross(IPC ipc, uint16_t time_millis) {
   
   uint8_t t_angle = uint8_t(time_millis >> 3) % 255;
   
   switch(((uint16_t(ipc.angle()) + t_angle) >> 5) % 4) {
      case 0:
         return CRGB(255, 0, 0);
         break;
      case 1:
         return CRGB(0, 0, 255);
         break;
   }
   
   return CRGB(0, 0, 0);
}

void LEDEffectMaelstrom::TransientLEDMode::update()
{
   for (auto key_addr : KeyAddr::all()) {
      
      uint16_t now = Kaleidoscope.millisAtCycleStart();
      
      IPC ipc = led_effect_whirl::readPolarCoord(key_addr);
      
      cRGB key_color;
         
      if(KeyboardHardware.isRightHandSideKey(key_addr)) {
         key_color = (*parent_->left_hand_color_function_)(ipc, now);
      }
      else {
         key_color = (*parent_->right_hand_color_function_)(ipc, now);
      }
      
      ::LEDControl.setCrgbAt(key_addr, key_color);
   }
}

} // namespace plugin
} // namespace kaleidoscope

kaleidoscope::plugin::LEDEffectMaelstrom LEDEffectMaelstrom;
