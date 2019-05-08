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

cRGB LEDEffectMaelstrom::colorModeWorm(KeyAddr key_addr, uint16_t time_millis) {

   IPC ipc = led_effect_whirl::readPolarCoord(key_addr);
   
   uint8_t t_angle = uint8_t(time_millis >> 2) % 255;
   
   constexpr uint8_t angle_range = 40;
   
   uint8_t angle_lower_bound = t_angle;
   uint8_t angle_upper_bound = (uint16_t(t_angle) + angle_range) % 255;
   
   if(angle_lower_bound < angle_upper_bound) {
      if((ipc.angle_ < angle_lower_bound) || (ipc.angle_ > angle_upper_bound)) {
         return black;
      }
   }
   else {
      if((ipc.angle_ < angle_lower_bound) && (ipc.angle_ > angle_upper_bound)) {
         return black;
      }
   }
   
   uint8_t t_radius = uint8_t(time_millis >> 4) % 255;
   
   uint8_t radius_upper_bound = 255 - t_radius;
   uint8_t radius_lower_bound = radius_upper_bound - 40;
   
   if(ipc.radius_ < radius_lower_bound) { return black; }
   if(ipc.radius_ > radius_upper_bound) { return black; }

   return hsvToRgb(ipc.radius_, 255, 255);
}

cRGB LEDEffectMaelstrom::colorModeMaelstrom(KeyAddr key_addr, uint16_t time_millis) {

   IPC ipc = led_effect_whirl::readPolarCoord(key_addr);
 
   return hsvToRgb((uint16_t(ipc.radius_) + (time_millis >> 3))%255,
                   255,
                   ((uint16_t(ipc.angle_) << 1) + (time_millis >> 2))%255);
}

cRGB LEDEffectMaelstrom::colorModePropeller(KeyAddr key_addr, uint16_t time_millis) {

   IPC ipc = led_effect_whirl::readPolarCoord(key_addr);
   
   uint8_t mAngle = (uint16_t(ipc.angle_ << 2)) % 255;
   
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

cRGB LEDEffectMaelstrom::colorModeBullsEye(KeyAddr key_addr, uint16_t time_millis) {

   IPC ipc = led_effect_whirl::readPolarCoord(key_addr);
   
   uint8_t t_radius = uint8_t(time_millis >> 2) % 255;
   
   if(((uint16_t(ipc.radius_) - t_radius) >> 7) % 2) {
      return CRGB(255, 0, 0);
   }
   
   return CRGB(255, 255, 255);
}

void LEDEffectMaelstrom::TransientLEDMode::update()
{
   for (auto key_addr : KeyAddr::all()) {
      auto led_id = KeyboardHardware.getLedIndex(key_addr);
      
      uint16_t now = Kaleidoscope.millisAtCycleStart();
      
      cRGB key_color;
         
      if(led_id < 32) {
         key_color = (*parent_->left_hand_color_function_)(key_addr, now);
      }
      else {
         key_color = (*parent_->right_hand_color_function_)(key_addr, now);
      }
      ::LEDControl.setCrgbAt(led_id, key_color);
   }
}

} // namespace plugin
} // namespace kaleidoscope

kaleidoscope::plugin::LEDEffectMaelstrom LEDEffectMaelstrom;
