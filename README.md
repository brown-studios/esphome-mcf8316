# esphome-mcf8316

An ESPHome component for driving the [MCF8316D](https://www.ti.com/product/MCF8316D) sensorless field-oriented control brushless DC motor controller with an I2C interface.

This component is not compatible with the MCF8316A or MCF8316C revisions which have different register layouts (and [errata](https://e2e.ti.com/support/motor-drivers-group/motor-drivers/f/motor-drivers-forum/1066777/faq-production-device-information)).

**Status: WORK IN PROGRESS**

## Component schema

```yaml
mcf8316:
    - id: minuet_fan_motor_driver
        wake: <SPEED/WAKE PIN>
        nfault: <NFAULT PIN>
        watchdog: <WATCHDOG PIN>
        on_fault:
            then:
                # Handle FaultStatus in 'x' variable
        on_algorithm_state:
            then:
                # Handle AlgorithmState in 'x' variable
```

* wake: (REQUIRED) The pin connected to the WAKE/SPEED input.
* nfault: (REQUIRED) The pin connected to the NFAULT output.
* watchdog: (OPTIONAL) The pin connected to the EXT_WD input.  When provided, configures the chip for a 1 second watchdog timeout and sends watchdog messages periodically.
* watchdog_over_i2c: (OPTIONAL) If true, configures the chip for a 1 second watchdog timeout and sends watchdog messages periodically over I2C.  Note that using the watchdog over I2C may interfere with the chip's brake function due to a [known issue](https://e2e.ti.com/support/motor-drivers-group/motor-drivers/f/motor-drivers-forum/1555307/mcf8316d-brake-triggers-watchdog_fault-when-watchdog-is-enabled/5991916) of the MCF8316D.
* on_fault: (OPTIONAL) An action that is triggered whenever the fault status changes. For example, this action can be used to observe when the motor driver encounters a fault so that a recovery action can be performed.
* on_algorithm_state: (OPTIONAL) An action that is triggered whenever the motor driver's algorithm state changes. For example, this action can be used to observe when the motor driver returns to the idle state after stopping the motor.

## Development

This project follows the ESPHome code style with [pre-commit](https://pre-commit.com/) hooks for linting and code formatting.

To run the style checks manually on all files and fix issues, run the following command:

```
uv run pre-commit run --all-files
```

## Resources

- [MCF8316D datasheet](https://www.ti.com/lit/ds/symlink/mcf8316d.pdf)
- [Tuning guide](https://www.ti.com/lit/an/slla663/slla663.pdf)
