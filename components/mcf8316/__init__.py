from esphome import automation, pins
import esphome.codegen as cg
from esphome.components import i2c
import esphome.config_validation as cv
from esphome.const import CONF_ID, CONF_TRIGGER_ID

CODEOWNERS = ["@j9brown"]

DEPENDENCIES = ["i2c"]
MULTI_CONF = True

CONF_WAKE_PIN = "wake"
CONF_NFAULT_PIN = "nfault"
CONF_WATCHDOG_PIN = "watchdog"
CONF_WATCHDOG_OVER_I2C = "watchdog_over_i2c"
CONF_ON_FAULT = "on_fault"
CONF_ON_ALGORITHM_STATE = "on_algorithm_state"

mcf8316_ns = cg.esphome_ns.namespace("mcf8316")

MCF8316Component = mcf8316_ns.class_("MCF8316Component", cg.Component)

FaultStatus = MCF8316Component.class_("FaultStatus")
FaultTrigger = mcf8316_ns.class_(
    "FaultTrigger", automation.Trigger.template(FaultStatus)
)

AlgorithmState = mcf8316_ns.class_("AlgorithmState")
AlgorithmStateTrigger = mcf8316_ns.class_(
    "AlgorithmStateTrigger", automation.Trigger.template(AlgorithmState)
)

CONFIG_SCHEMA = cv.All(
    cv.COMPONENT_SCHEMA.extend(
        {
            cv.GenerateID(): cv.declare_id(MCF8316Component),
            cv.Required(CONF_WAKE_PIN): pins.gpio_output_pin_schema,
            cv.Required(CONF_NFAULT_PIN): pins.gpio_input_pullup_pin_schema,
            cv.Exclusive(CONF_WATCHDOG_PIN, "watchdog"): pins.gpio_output_pin_schema,
            cv.Exclusive(CONF_WATCHDOG_OVER_I2C, "watchdog"): bool,
            cv.Optional(CONF_ON_FAULT): automation.validate_automation(
                {
                    cv.GenerateID(CONF_TRIGGER_ID): cv.declare_id(FaultTrigger),
                }
            ),
            cv.Optional(CONF_ON_ALGORITHM_STATE): automation.validate_automation(
                {
                    cv.GenerateID(CONF_TRIGGER_ID): cv.declare_id(AlgorithmStateTrigger),
                }
            ),
        }
    ).extend(i2c.i2c_device_schema(0x01))
)


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await i2c.register_i2c_device(var, config)
    cg.add(var.set_wake_pin(await cg.gpio_pin_expression(config[CONF_WAKE_PIN])))
    cg.add(var.set_nfault_pin(await cg.gpio_pin_expression(config[CONF_NFAULT_PIN])))
    if CONF_WATCHDOG_PIN in config:
        cg.add(var.set_watchdog_pin(await cg.gpio_pin_expression(config[CONF_WATCHDOG_PIN])))
    if CONF_WATCHDOG_OVER_I2C in config:
        cg.add(var.set_watchdog_over_i2c(config[CONF_WATCHDOG_OVER_I2C]))
    for conf in config.get(CONF_ON_FAULT, []):
        trigger = cg.new_Pvariable(conf[CONF_TRIGGER_ID], var)
        await automation.build_automation(trigger, [(FaultStatus, "x")], conf)
    for conf in config.get(CONF_ON_ALGORITHM_STATE, []):
        trigger = cg.new_Pvariable(conf[CONF_TRIGGER_ID], var)
        await automation.build_automation(trigger, [(AlgorithmState, "x")], conf)
