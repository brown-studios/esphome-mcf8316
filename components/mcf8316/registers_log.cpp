#include "esphome/core/log.h"
#include "registers.h"

#include <string>

namespace esphome {
namespace mcf8316 {

ESPHOME_LOG_TAG(TAG, "MCF8316");

#ifdef ESPHOME_LOG_HAS_DEBUG
// Logs the contents of the configuration
void log_config_field_value(const char *name, uint32_t value) {
  ESP_LOGD(TAG, "    - %s: 0x%lx", name, value);
}

#define MCF8316_LOG_CONFIG_FIELD(field) log_config_field_value(#field, uint32_t(config.get(field)));

void log_config_register_value(const char *name, uint32_t value) {
  ESP_LOGD(TAG, "  %s: 0x%08lx", name, value);
}
#define MCF8316_LOG_CONFIG_REGISTER(reg) \
  log_config_register_value(#reg, config.at<Register::reg>().value); \
  MCF8316_FOR_EACH_FIELD_OF_REGISTER(reg, MCF8316_LOG_CONFIG_FIELD);

void log_config(const Config &config) {
  ESP_LOGD(TAG, "Configuration registers:");
  MCF8316_FOR_EACH_CONFIG_REGISTER(MCF8316_LOG_CONFIG_REGISTER)
}
#else
void log_config(const Config &config) {
}
#endif

}  // namespace mcf8316
}  // namespace esphome
