#pragma once

#include <algorithm>
#include <array>
#include <cstddef>
#include <cstdint>
#include <string>

namespace esphome {
namespace mcf8316 {

enum class Register : unsigned;

// Designates a bit-field of the specified width within a 32-bit register.
template<Register reg, unsigned bit_, unsigned width_, typename T> struct Field final {
  using Type = T;
  static constexpr uint32_t mask = ((1ul << width_) - 1ul) << bit_;
  static constexpr unsigned bit = bit_;
  static constexpr unsigned width = width_;
};

// Designates a bit-field that has been split across two registers.
template<typename MSBField, typename LSBField, typename T> struct CompositeField final {
  using Type = T;
  static constexpr MSBField msb = MSBField();
  static constexpr LSBField lsb = LSBField();
  static constexpr unsigned width = MSBField::width + LSBField::width;
};
template<typename T, typename MSBField, typename LSBField>
constexpr CompositeField<MSBField, LSBField, T> make_composite_field(MSBField msb, LSBField lsb) {
  return CompositeField<MSBField, LSBField, T>();
}

// MCF8316D register addresses (12 bits)
enum class Register : unsigned {
  // Configuration shadow registers (restored from EEPROM on reset)
  ISD_CONFIG = 0x080,
  REV_DRIVE_CONFIG = 0x082,
  MOTOR_STARTUP1 = 0x084,
  MOTOR_STARTUP2 = 0x086,
  CLOSED_LOOP1 = 0x088,
  CLOSED_LOOP2 = 0x08a,
  CLOSED_LOOP3 = 0x08c,
  CLOSED_LOOP4 = 0x08e,
  FAULT_CONFIG1 = 0x090,
  FAULT_CONFIG2 = 0x092,
  REF_PROFILES1 = 0x094,
  REF_PROFILES2 = 0x096,
  REF_PROFILES3 = 0x098,
  REF_PROFILES4 = 0x09a,
  REF_PROFILES5 = 0x09c,
  REF_PROFILES6 = 0x09e,
  INT_ALGO1 = 0x0a0,  // aka. INT_ALGO_1
  INT_ALGO2 = 0x0a2,  // aka. INT_ALGO_2
  PIN_CONFIG = 0x0a4,
  DEVICE_CONFIG1 = 0x0a6,
  DEVICE_CONFIG2 = 0x0a8,
  PERI_CONFIG1 = 0x0aa,
  GD_CONFIG1 = 0x0ac,
  GD_CONFIG2 = 0x0ae,

  // Fault status registers
  GATE_DRIVER_FAULT_STATUS = 0x0e0,
  CONTROLLER_FAULT_STATUS = 0x0e2,
  EEPROM_FAULT_STATUS = 0x24c,

  // System status registers
  ALGO_STATUS = 0x0e4,
  MTR_PARAMS = 0x0e6,
  ALGO_STATUS_MPET = 0x0e8,

  // Algorithm control registers
  ALGO_CTRL1 = 0x0ea,
  ALGO_DEBUG1 = 0x0ec,
  ALGO_DEBUG2 = 0x0ee,
  CURRENT_PI = 0x0f0,
  SPEED_PI = 0x0f2,
  DAC_1 = 0x0f4,
  DAC_2 = 0x0f6,
  EEPROM_SECURITY = 0x0f8,

  // Algorithm variable registers
  ALGORITHM_STATE = 0x18e,
  FG_SPEED_FDBK = 0x194,
  BUS_CURRENT = 0x40c,
  PHASE_CURRENT_A = 0x444,
  PHASE_CURRENT_B = 0x446,
  PHASE_CURRENT_C = 0x448,
  IMAG_SQR = 0x46a,
  CSA_GAIN_FEEDBACK = 0x46c,
  VOLTAGE_GAIN_FEEDBACK = 0x477,
  VM_VOLTAGE = 0x47c,
  PHASE_VOLTAGE_VA = 0x484,
  PHASE_VOLTAGE_VB = 0x486,
  PHASE_VOLTAGE_VC = 0x488,
  SIN_COMMUTATION_ANGLE = 0x4bc,
  COS_COMMUTATION_ANGLE = 0x4be,
  IALPHA = 0x4dc,
  IBETA = 0x4de,
  VALPHA = 0x4e0,
  VBETA = 0x4e2,
  ID = 0x4ec,
  IQ = 0x4ee,
  VD = 0x4f0,
  VQ = 0x4f2,
  IQ_REF_ROTOR_ALIGN = 0x52a,
  SPEED_REF_OPEN_LOOP = 0x540,
  IQ_REF_OPEN_LOOP = 0x550,
  SPEED_REF_CLOSED_LOOP = 0x5d2,
  ID_REF_CLOSED_LOOP = 0x612,
  IQ_REF_CLOSED_LOOP = 0x614,
  ISD_STATE = 0x6ae,
  ISD_SPEED = 0x6b8,
  IPD_STATE = 0x6ea,
  IPD_ANGLE = 0x72e,
  ED = 0x772,
  EQ = 0x774,
  SPEED_FDBK = 0x782,
  THETA_EST = 0x786,
};

// Returns true if the register is a configuration shadow register.
constexpr inline bool is_config_register(Register reg) {
  return reg >= Register::ISD_CONFIG && reg <= Register::GD_CONFIG2;
}

#define MCF8316_FOR_EACH_CONFIG_REGISTER(x) \
  x(ISD_CONFIG) x(REV_DRIVE_CONFIG) x(MOTOR_STARTUP1) x(MOTOR_STARTUP2) x(CLOSED_LOOP1) x(CLOSED_LOOP2) \
      x(CLOSED_LOOP3) x(CLOSED_LOOP4) x(FAULT_CONFIG1) x(FAULT_CONFIG2) x(REF_PROFILES1) x(REF_PROFILES2) \
          x(REF_PROFILES3) x(REF_PROFILES4) x(REF_PROFILES5) x(REF_PROFILES6) x(INT_ALGO1) x(INT_ALGO2) x(PIN_CONFIG) \
              x(DEVICE_CONFIG1) x(DEVICE_CONFIG2) x(PERI_CONFIG1) x(GD_CONFIG1) x(GD_CONFIG2)

#define MCF8316_FOR_EACH_FIELD_OF_REGISTER(reg, x) MCF8316_FOR_EACH_##reg##_FIELD(x)

// EEPROM register fields
// ISD_CONFIG
enum class ForwardDriveResyncThreshold : unsigned {
  THRESHOLD_5_PERCENT = 0,   // 5 %
  THRESHOLD_10_PERCENT = 1,  // 10 %
  THRESHOLD_15_PERCENT = 2,  // 15 %
  THRESHOLD_20_PERCENT = 3,  // 20 %
  THRESHOLD_25_PERCENT = 4,  // 25 %
  THRESHOLD_30_PERCENT = 5,  // 30 %
  THRESHOLD_35_PERCENT = 6,  // 35 %
  THRESHOLD_40_PERCENT = 7,  // 40 %
  THRESHOLD_45_PERCENT = 8,  // 45 %
  THRESHOLD_50_PERCENT = 9,  // 50 %
};
enum class BrakeMode : unsigned {
  BRAKE_HIGH_SIDE = 0,
  BRAKE_LOW_SIDE = 1,
};
enum class BrakeConfig : unsigned {
  BRAKE_TIME = 0,
  BRAKE_CURRENT_AND_TIME = 1,
};
enum class BrakeCurrentThreshold : unsigned {
  BRAKE_CURRENT_0_1_A = 0,  // 0.1 A
  BRAKE_CURRENT_0_2_A = 1,  // 0.2 A
  BRAKE_CURRENT_0_3_A = 2,  // 0.3 A
  BRAKE_CURRENT_0_5_A = 3,  // 0.5 A
  BRAKE_CURRENT_1_0_A = 4,  // 1.0 A
  BRAKE_CURRENT_2_0_A = 5,  // 2.0 A
};
enum class BrakeTime : unsigned {
  BRAKE_TIME_10_MS = 0,      // 10 ms
  BRAKE_TIME_50_MS = 1,      // 50 ms
  BRAKE_TIME_100_MS = 2,     // 100 ms
  BRAKE_TIME_200_MS = 3,     // 200 ms
  BRAKE_TIME_300_MS = 4,     // 300 ms
  BRAKE_TIME_400_MS = 5,     // 400 ms
  BRAKE_TIME_500_MS = 6,     // 500 ms
  BRAKE_TIME_750_MS = 7,     // 750 ms
  BRAKE_TIME_1000_MS = 8,    // 1000 ms
  BRAKE_TIME_2000_MS = 9,    // 2000 ms
  BRAKE_TIME_3000_MS = 10,   // 3000 ms
  BRAKE_TIME_4000_MS = 11,   // 4000 ms
  BRAKE_TIME_5000_MS = 12,   // 5000 ms
  BRAKE_TIME_7500_MS = 13,   // 7500 ms
  BRAKE_TIME_10000_MS = 14,  // 10000 ms
  BRAKE_TIME_15000_MS = 15,  // 15000 ms
};
enum class StationaryDetectionThreshold : unsigned {
  THRESHOLD_50_MV = 0,    // 50 mV
  THRESHOLD_75_MV = 1,    // 75 mV
  THRESHOLD_100_MV = 2,   // 100 mV
  THRESHOLD_250_MV = 3,   // 250 mV
  THRESHOLD_500_MV = 4,   // 500 mV
  THRESHOLD_750_MV = 5,   // 750 mV
  THRESHOLD_1000_MV = 6,  // 1000 mV
  THRESHOLD_1500_MV = 7,  // 1500 mV
};
constexpr auto ISD_EN = Field<Register::ISD_CONFIG, 30, 1, bool>();
constexpr auto BRAKE_EN = Field<Register::ISD_CONFIG, 29, 1, bool>();
constexpr auto HIZ_EN = Field<Register::ISD_CONFIG, 28, 1, bool>();
constexpr auto RVS_DR_EN = Field<Register::ISD_CONFIG, 27, 1, bool>();
constexpr auto RESYNC_EN = Field<Register::ISD_CONFIG, 26, 1, bool>();
constexpr auto FW_DRV_RESYN_THR = Field<Register::ISD_CONFIG, 22, 4, ForwardDriveResyncThreshold>();
constexpr auto BRK_MODE = Field<Register::ISD_CONFIG, 21, 1, BrakeMode>();
constexpr auto BRK_CONFIG = Field<Register::ISD_CONFIG, 20, 1, BrakeConfig>();
constexpr auto BRK_CURR_THR = Field<Register::ISD_CONFIG, 17, 3, BrakeCurrentThreshold>();
constexpr auto BRK_TIME = Field<Register::ISD_CONFIG, 13, 4, BrakeTime>();
constexpr auto HIZ_TIME = Field<Register::ISD_CONFIG, 9, 4, unsigned>();
constexpr auto STAT_DETECT_THR = Field<Register::ISD_CONFIG, 6, 3, StationaryDetectionThreshold>();
constexpr auto REV_DRV_HANDOFF_THR = Field<Register::ISD_CONFIG, 2, 4, unsigned>();
constexpr auto REV_DRV_OPEN_LOOP_CURRENT = Field<Register::ISD_CONFIG, 0, 2, unsigned>();
#define MCF8316_FOR_EACH_ISD_CONFIG_FIELD(x) \
  x(ISD_EN) x(BRAKE_EN) x(HIZ_EN) x(RVS_DR_EN) x(RESYNC_EN) x(FW_DRV_RESYN_THR) x(BRK_MODE) x(BRK_CONFIG) \
      x(BRK_CURR_THR) x(BRK_TIME) x(HIZ_TIME) x(STAT_DETECT_THR) x(REV_DRV_HANDOFF_THR) x(REV_DRV_OPEN_LOOP_CURRENT)

// REV_DRIVE_CONFIG
constexpr auto REV_DRV_OPEN_LOOP_ACCEL_A1 = Field<Register::REV_DRIVE_CONFIG, 27, 4, unsigned>();
constexpr auto REV_DRV_OPEN_LOOP_ACCEL_A2 = Field<Register::REV_DRIVE_CONFIG, 23, 4, unsigned>();
constexpr auto ACTIVE_BRAKE_CURRENT_LIMIT = Field<Register::REV_DRIVE_CONFIG, 20, 3, unsigned>();
constexpr auto ACTIVE_BRAKE_KP = Field<Register::REV_DRIVE_CONFIG, 10, 10, unsigned>();
constexpr auto ACTIVE_BRAKE_KI = Field<Register::REV_DRIVE_CONFIG, 0, 10, unsigned>();
#define MCF8316_FOR_EACH_REV_DRIVE_CONFIG_FIELD(x) \
  x(REV_DRV_OPEN_LOOP_ACCEL_A1) x(REV_DRV_OPEN_LOOP_ACCEL_A2) x(ACTIVE_BRAKE_CURRENT_LIMIT) x(ACTIVE_BRAKE_KP) \
      x(ACTIVE_BRAKE_KI)

// MOTOR_STARTUP1
enum class MotorStartup : unsigned {
  ALIGN = 0,
  DOUBLE_ALIGN = 1,
  IPD = 2,
  SLOW_FIRST_CYCLE = 3,
};
enum class AlignSlowRampRate : unsigned {
  LIMIT_0_1_A_S = 0,    // 0.1 A/s
  LIMIT_1_A_S = 1,      // 1 A/s
  LIMIT_5_A_S = 2,      // 5 A/s
  LIMIT_10_A_S = 3,     // 10 A/s
  LIMIT_15_A_S = 4,     // 15 A/s
  LIMIT_25_A_S = 5,     // 25 A/s
  LIMIT_50_A_S = 6,     // 50 A/s
  LIMIT_100_A_S = 7,    // 100 A/s
  LIMIT_150_A_S = 8,    // 150 A/s
  LIMIT_200_A_S = 9,    // 200 A/s
  LIMIT_250_A_S = 10,   // 250 A/s
  LIMIT_500_A_S = 11,   // 500 A/s
  LIMIT_1000_A_S = 12,  // 1000 A/s
  LIMIT_2000_A_S = 13,  // 2000 A/s
  LIMIT_5000_A_S = 14,  // 5000 A/s
  LIMIT_NONE = 15,      // no limit
};
enum class CurrentLimit : unsigned {
  LIMIT_0_125_A = 0,  // 0.125 A
  LIMIT_0_25_A = 1,   // 0.25 A
  LIMIT_0_5_A = 2,    // 0.5 A
  LIMIT_1_0_A = 3,    // 1.0 A
  LIMIT_1_5_A = 4,    // 1.5 A
  LIMIT_2_0_A = 5,    // 2.0 A
  LIMIT_2_5_A = 6,    // 2.5 A
  LIMIT_3_0_A = 7,    // 3.0 A
  LIMIT_3_5_A = 8,    // 3.5 A
  LIMIT_4_0_A = 9,    // 4.0 A
  LIMIT_4_5_A = 10,   // 4.5 A
  LIMIT_5_0_A = 11,   // 5.0 A
  LIMIT_5_5_A = 12,   // 5.5 A
  LIMIT_6_0_A = 13,   // 6.0 A
  LIMIT_7_0_A = 14,   // 7.0 A
  LIMIT_8_0_A = 15,   // 8.0 A
};
enum class IPDClockFrequency : unsigned {
  FREQ_50_HZ = 0,     // 50 Hz
  FREQ_100_HZ = 1,    // 100 Hz
  FREQ_250_HZ = 2,    // 250 Hz
  FREQ_500_HZ = 3,    // 500 Hz
  FREQ_1000_HZ = 4,   // 1000 Hz
  FREQ_2000_HZ = 5,   // 2000 Hz
  FREQ_5000_HZ = 6,   // 5000 Hz
  FREQ_10000_HZ = 7,  // 10000 Hz
};
enum class IPDCurrentThreshold : unsigned {
  THRESHOLD_0_25_A = 0,    // 0.25 A
  THRESHOLD_0_5_A = 1,     // 0.5 A
  THRESHOLD_0_75_A = 2,    // 0.75 A
  THRESHOLD_1_0_A = 3,     // 1.0 A
  THRESHOLD_1_25_A = 4,    // 1.25 A
  THRESHOLD_1_5_A = 5,     // 1.5 A
  THRESHOLD_2_0_A = 6,     // 2.0 A
  THRESHOLD_2_5_A = 7,     // 2.5 A
  THRESHOLD_3_0_A = 8,     // 3.0 A
  THRESHOLD_3_667_A = 9,   // 3.667 A
  THRESHOLD_4_0_A = 10,    // 4.0 A
  THRESHOLD_4_667_A = 11,  // 4.667 A
  THRESHOLD_5_0_A = 12,    // 5.0 A
  THRESHOLD_5_333_A = 13,  // 5.333 A
  THRESHOLD_6_0_A = 14,    // 6.0 A
  THRESHOLD_6_667_A = 15,  // 6.667 A
  THRESHOLD_7_333_A = 16,  // 7.333 A
  THRESHOLD_8_0_A = 17,    // 8.0 A
};
enum class IPDReleaseMode : unsigned {
  BRAKE = 0,
  TRISTATE = 1,
};
enum class IPDAdvanceAngle : unsigned {
  ANGLE_0_DEG = 0,   // 0 degrees
  ANGLE_30_DEG = 1,  // 30 degrees
  ANGLE_60_DEG = 2,  // 60 degrees
  ANGLE_90_DEG = 3,  // 90 degress
};
enum class IPDRepeat : unsigned {
  REPEAT_1 = 0,  // 1 time
  REPEAT_2 = 1,  // 2 times
  REPEAT_3 = 2,  // 3 times
  REPEAT_4 = 3,  // 4 times
};
constexpr auto MTR_STARTUP = Field<Register::MOTOR_STARTUP1, 29, 2, MotorStartup>();
constexpr auto ALIGN_SLOW_RAMP_RATE = Field<Register::MOTOR_STARTUP1, 25, 4, AlignSlowRampRate>();
constexpr auto ALIGN_TIME = Field<Register::MOTOR_STARTUP1, 21, 4, unsigned>();
constexpr auto ALIGN_OR_SLOW_CURRENT_ILIMIT = Field<Register::MOTOR_STARTUP1, 17, 4, CurrentLimit>();
constexpr auto IPD_CLK_FREQ = Field<Register::MOTOR_STARTUP1, 14, 3, IPDClockFrequency>();
constexpr auto IPD_CURR_THR = Field<Register::MOTOR_STARTUP1, 9, 5, IPDCurrentThreshold>();
constexpr auto IPD_RLS_MODE = Field<Register::MOTOR_STARTUP1, 8, 1, IPDReleaseMode>();
constexpr auto IPD_ADV_ANGLE = Field<Register::MOTOR_STARTUP1, 6, 2, IPDAdvanceAngle>();
constexpr auto IPD_REPEAT = Field<Register::MOTOR_STARTUP1, 4, 2, IPDRepeat>();
constexpr auto IQ_RAMP_EN = Field<Register::MOTOR_STARTUP1, 2, 1, bool>();
constexpr auto ACTIVE_BRAKE_EN = Field<Register::MOTOR_STARTUP1, 1, 1, bool>();
constexpr auto REV_DRV_CONFIG = Field<Register::MOTOR_STARTUP1, 0, 1, unsigned>();
#define MCF8316_FOR_EACH_MOTOR_STARTUP1_FIELD(x) \
  x(MTR_STARTUP) x(ALIGN_SLOW_RAMP_RATE) x(ALIGN_TIME) x(ALIGN_OR_SLOW_CURRENT_ILIMIT) x(IPD_CLK_FREQ) x(IPD_CURR_THR) \
      x(IPD_RLS_MODE) x(IPD_ADV_ANGLE) x(IPD_REPEAT) x(IQ_RAMP_EN) x(ACTIVE_BRAKE_EN) x(REV_DRV_CONFIG)

// MOTOR_STARTUP2
enum class OpenLoopAcceleration : unsigned {
  ACCEL_0_1_HZ_S = 0,       // 0.1 Hz/s
  ACCEL_0_5_HZ_S = 1,       // 0.5 Hz/s
  ACCEL_1_0_HZ_S = 2,       // 1.0 Hz/s
  ACCEL_2_5_HZ_S = 3,       // 2.5 Hz/s
  ACCEL_5_0_HZ_S = 4,       // 5.0 Hz/s
  ACCEL_10_0_HZ_S = 5,      // 10.0 Hz/s
  ACCEL_25_0_HZ_S = 6,      // 25.0 Hz/s
  ACCEL_50_0_HZ_S = 7,      // 50.0 Hz/s
  ACCEL_75_0_HZ_S = 8,      // 75.0 Hz/s
  ACCEL_100_0_HZ_S = 9,     // 100.0 Hz/s
  ACCEL_250_0_HZ_S = 10,    // 250.0 Hz/s
  ACCEL_500_0_HZ_S = 11,    // 500.0 Hz/s
  ACCEL_750_0_HZ_S = 12,    // 750.0 Hz/s
  ACCEL_1000_0_HZ_S = 13,   // 1000.0 Hz/s
  ACCEL_5000_0_HZ_S = 14,   // 5000.0 Hz/s
  ACCEL_10000_0_HZ_S = 15,  // 10000.0 Hz/s
};
enum class OpenLoopAcceleration2 : unsigned {
  ACCEL_0_0_HZ_S2 = 0,       // 0.0 Hz/s^2
  ACCEL_0_5_HZ_S2 = 1,       // 0.5 Hz/s^2
  ACCEL_1_0_HZ_S2 = 2,       // 1.0 Hz/s^2
  ACCEL_2_5_HZ_S2 = 3,       // 2.5 Hz/s^2
  ACCEL_5_0_HZ_S2 = 4,       // 5.0 Hz/s^2
  ACCEL_10_0_HZ_S2 = 5,      // 10.0 Hz/s^2
  ACCEL_25_0_HZ_S2 = 6,      // 25.0 Hz/s^2
  ACCEL_50_0_HZ_S2 = 7,      // 50.0 Hz/s^2
  ACCEL_75_0_HZ_S2 = 8,      // 75.0 Hz/s^2
  ACCEL_100_0_HZ_S2 = 9,     // 100.0 Hz/s^2
  ACCEL_250_0_HZ_S2 = 10,    // 250.0 Hz/s^2
  ACCEL_500_0_HZ_S2 = 11,    // 500.0 Hz/s^2
  ACCEL_750_0_HZ_S2 = 12,    // 750.0 Hz/s^2
  ACCEL_1000_0_HZ_S2 = 13,   // 1000.0 Hz/s^2
  ACCEL_5000_0_HZ_S2 = 14,   // 5000.0 Hz/s^2
  ACCEL_10000_0_HZ_S2 = 15,  // 10000.0 Hz/s^2
};
enum class OpenToCloseLoopHandoffThreshold : unsigned {
  THRESHOLD_1_PERCENT = 0,
  THRESHOLD_2_PERCENT = 1,
  THRESHOLD_3_PERCENT = 2,
  THRESHOLD_4_PERCENT = 3,
  THRESHOLD_5_PERCENT = 4,
  THRESHOLD_6_PERCENT = 5,
  THRESHOLD_7_PERCENT = 6,
  THRESHOLD_8_PERCENT = 7,
  THRESHOLD_9_PERCENT = 8,
  THRESHOLD_10_PERCENT = 9,
  THRESHOLD_11_PERCENT = 10,
  THRESHOLD_12_PERCENT = 11,
  THRESHOLD_13_PERCENT = 12,
  THRESHOLD_14_PERCENT = 13,
  THRESHOLD_15_PERCENT = 14,
  THRESHOLD_16_PERCENT = 15,
  THRESHOLD_17_PERCENT = 16,
  THRESHOLD_18_PERCENT = 17,
  THRESHOLD_19_PERCENT = 18,
  THRESHOLD_20_PERCENT = 19,
  THRESHOLD_22_5_PERCENT = 20,
  THRESHOLD_25_PERCENT = 21,
  THRESHOLD_27_5_PERCENT = 22,
  THRESHOLD_30_PERCENT = 23,
  THRESHOLD_32_5_PERCENT = 24,
  THRESHOLD_35_PERCENT = 25,
  THRESHOLD_37_5_PERCENT = 26,
  THRESHOLD_40_PERCENT = 27,
  THRESHOLD_42_5_PERCENT = 28,
  THRESHOLD_45_PERCENT = 29,
  THRESHOLD_47_5_PERCENT = 30,
  THRESHOLD_50_PERCENT = 31,
};
enum class SlowFirstCycleFrequency : unsigned {
  FREQ_0_1_PERCENT = 0,    // 0.1 % of MAX_SPEED
  FREQ_0_3_PERCENT = 1,    // 0.3 % of MAX_SPEED
  FREQ_0_5_PERCENT = 2,    // 0.5 % of MAX_SPEED
  FREQ_0_7_PERCENT = 3,    // 0.7 % of MAX_SPEED
  FREQ_1_0_PERCENT = 4,    // 1.0 % of MAX_SPEED
  FREQ_1_5_PERCENT = 5,    // 1.5 % of MAX_SPEED
  FREQ_2_0_PERCENT = 6,    // 2.0 % of MAX_SPEED
  FREQ_2_5_PERCENT = 7,    // 2.5 % of MAX_SPEED
  FREQ_3_0_PERCENT = 8,    // 3.0 % of MAX_SPEED
  FREQ_4_0_PERCENT = 9,    // 4.0 % of MAX_SPEED
  FREQ_5_0_PERCENT = 10,   // 5.0 % of MAX_SPEED
  FREQ_7_5_PERCENT = 11,   // 7.5 % of MAX_SPEED
  FREQ_10_0_PERCENT = 12,  // 10.0 % of MAX_SPEED
  FREQ_15_0_PERCENT = 13,  // 15.0 % of MAX_SPEED
  FREQ_20_0_PERCENT = 14,  // 20.0 % of MAX_SPEED
  FREQ_25_0_PERCENT = 15,  // 25.0 % of MAX_SPEED
};
enum class FirstCycleFrequencySelect : unsigned {
  FREQ_0_HZ = 0,                // 0 Hz
  USE_SLOW_FIRST_CYC_FREQ = 1,  // from SLOW_FIRST_CYC_FREQ
};
enum class ThetaErrorRampRate : unsigned {
  RATE_0_01_DEG_MS = 0,  // 0.01 deg/ms
  RATE_0_05_DEG_MS = 1,  // 0.05 deg/ms
  RATE_0_10_DEG_MS = 2,  // 0.10 deg/ms
  RATE_0_15_DEG_MS = 3,  // 0.15 deg/ms
  RATE_0_20_DEG_MS = 4,  // 0.20 deg/ms
  RATE_0_50_DEG_MS = 5,  // 0.50 deg/ms
  RATE_1_00_DEG_MS = 6,  // 1.00 deg/ms
  RATE_2_00_DEG_MS = 7,  // 2.00 deg/ms
};
constexpr auto OL_ILIMIT = Field<Register::MOTOR_STARTUP2, 27, 4, CurrentLimit>();
constexpr auto OL_ACC_A1 = Field<Register::MOTOR_STARTUP2, 23, 4, OpenLoopAcceleration>();
constexpr auto OL_ACC_A2 = Field<Register::MOTOR_STARTUP2, 19, 4, OpenLoopAcceleration2>();
constexpr auto AUTO_HANDOFF_EN = Field<Register::MOTOR_STARTUP2, 18, 1, bool>();
constexpr auto OPN_CL_HANDOFF_THR = Field<Register::MOTOR_STARTUP2, 13, 5, OpenToCloseLoopHandoffThreshold>();
constexpr auto ALIGN_ANGLE = Field<Register::MOTOR_STARTUP2, 8, 5, unsigned>();
constexpr auto SLOW_FIRST_CYC_FREQ = Field<Register::MOTOR_STARTUP2, 4, 4, SlowFirstCycleFrequency>();
constexpr auto FIRST_CYCLE_FREQ_SEL = Field<Register::MOTOR_STARTUP2, 3, 1, FirstCycleFrequencySelect>();
constexpr auto THETA_ERROR_RAMP_RATE = Field<Register::MOTOR_STARTUP2, 0, 3, ThetaErrorRampRate>();
#define MCF8316_FOR_EACH_MOTOR_STARTUP2_FIELD(x) \
  x(OL_ILIMIT) x(OL_ACC_A1) x(OL_ACC_A2) x(AUTO_HANDOFF_EN) x(OPN_CL_HANDOFF_THR) x(ALIGN_ANGLE) \
      x(SLOW_FIRST_CYC_FREQ) x(FIRST_CYCLE_FREQ_SEL) x(THETA_ERROR_RAMP_RATE)

// CLOSED_LOOP1
enum class ClosedLoopAcceleration : unsigned {
  ACCEL_0_5_HZ_S = 0,       // 0.5 Hz/s
  ACCEL_1_0_HZ_S = 1,       // 1.0 Hz/s
  ACCEL_2_5_HZ_S = 2,       // 2.5 Hz/s
  ACCEL_5_0_HZ_S = 3,       // 5.0 Hz/s
  ACCEL_7_5_HZ_S = 4,       // 7.5 Hz/s
  ACCEL_10_0_HZ_S = 5,      // 10.0 Hz/s
  ACCEL_20_0_HZ_S = 6,      // 20.0 Hz/s
  ACCEL_40_0_HZ_S = 7,      // 40.0 Hz/s
  ACCEL_60_0_HZ_S = 8,      // 60.0 Hz/s
  ACCEL_80_0_HZ_S = 9,      // 80.0 Hz/s
  ACCEL_100_0_HZ_S = 10,    // 100.0 Hz/s
  ACCEL_200_0_HZ_S = 11,    // 200.0 Hz/s
  ACCEL_300_0_HZ_S = 12,    // 300.0 Hz/s
  ACCEL_400_0_HZ_S = 13,    // 400.0 Hz/s
  ACCEL_500_0_HZ_S = 14,    // 500.0 Hz/s
  ACCEL_600_0_HZ_S = 15,    // 600.0 Hz/s
  ACCEL_700_0_HZ_S = 16,    // 700.0 Hz/s
  ACCEL_800_0_HZ_S = 17,    // 800.0 Hz/s
  ACCEL_900_0_HZ_S = 18,    // 9000.0 Hz/s
  ACCEL_1000_0_HZ_S = 19,   // 1000.0 Hz/s
  ACCEL_2000_0_HZ_S = 20,   // 2000.0 Hz/s
  ACCEL_4000_0_HZ_S = 21,   // 4000.0 Hz/s
  ACCEL_6000_0_HZ_S = 22,   // 6000.0 Hz/s
  ACCEL_8000_0_HZ_S = 23,   // 8000.0 Hz/s
  ACCEL_10000_0_HZ_S = 24,  // 10000.0 Hz/s
  ACCEL_20000_0_HZ_S = 25,  // 20000.0 Hz/s
  ACCEL_30000_0_HZ_S = 26,  // 30000.0 Hz/s
  ACCEL_40000_0_HZ_S = 27,  // 40000.0 Hz/s
  ACCEL_50000_0_HZ_S = 28,  // 50000.0 Hz/s
  ACCEL_60000_0_HZ_S = 29,  // 60000.0 Hz/s
  ACCEL_70000_0_HZ_S = 30,  // 70000.0 Hz/s
  ACCEL_NO_LIMIT = 31,      // No limit
};
enum class PWMOutputFrequency : unsigned {
  FREQ_10_KHZ = 0,   // 10 kHz
  FREQ_15_KHZ = 1,   // 15 kHz
  FREQ_20_KHZ = 2,   // 20 kHz
  FREQ_25_KHZ = 3,   // 25 kHz
  FREQ_30_KHZ = 4,   // 30 kHz
  FREQ_35_KHZ = 5,   // 35 kHz
  FREQ_40_KHZ = 6,   // 40 kHz
  FREQ_45_KHZ = 7,   // 45 kHz
  FREQ_50_KHZ = 8,   // 50 kHz
  FREQ_55_KHZ = 9,   // 55 kHz
  FREQ_60_KHZ = 10,  // 60 kHz
};
enum class PWMMode : unsigned {
  CONTINUOUS = 0,
  DISCONTINUOUS = 1,
};
enum class FGSelect : unsigned {
  FG_IN_ISD_OPEN_AND_CLOSED_LOOP = 0,
  FG_IN_CLOSED_LOOP = 1,
  FG_IN_OPEN_LOOP_FIRST_TRY = 2,
};
enum class FGConfig : unsigned {
  FG_WHILE_DRIVEN = 0,
  FG_UNTIL_BEMF_BELOW_THRESHOLD = 1,
};
enum class FGBEMFThreshold : unsigned {
  THR_1_MV = 0,   // +/- 1 mV
  THR_2_MV = 1,   // +/- 2 mV
  THR_5_MV = 2,   // +/- 5 mV
  THR_10_MV = 3,  // +/- 10 mV
  THR_20_MV = 4,  // +/- 20 mV
  THR_30_MV = 5,  // +/- 30 mV
};
using ClosedLoopDeceleration = ClosedLoopAcceleration;
constexpr auto OVERMODULATION_ENABLE = Field<Register::CLOSED_LOOP1, 30, 1, bool>();
constexpr auto CL_ACC = Field<Register::CLOSED_LOOP1, 25, 5, ClosedLoopAcceleration>();
constexpr auto CL_DEC = Field<Register::CLOSED_LOOP1, 19, 5, ClosedLoopDeceleration>();
constexpr auto PWM_FREQ_OUT = Field<Register::CLOSED_LOOP1, 15, 4, PWMOutputFrequency>();
constexpr auto PWM_MODE = Field<Register::CLOSED_LOOP1, 14, 1, PWMMode>();
constexpr auto FG_SEL = Field<Register::CLOSED_LOOP1, 12, 2, FGSelect>();
constexpr auto FG_DIV = Field<Register::CLOSED_LOOP1, 8, 4, unsigned>();  // see fg_div_from_motor_poles
constexpr auto FG_CONFIG = Field<Register::CLOSED_LOOP1, 7, 1, FGConfig>();
constexpr auto FG_BEMF_THR = Field<Register::CLOSED_LOOP1, 4, 3, FGBEMFThreshold>();
constexpr auto AVS_EN = Field<Register::CLOSED_LOOP1, 3, 1, bool>();
constexpr auto DEADTIME_COMP_EN = Field<Register::CLOSED_LOOP1, 2, 1, bool>();
constexpr auto LOW_SPEED_RECIRC_BRAKE_EN = Field<Register::CLOSED_LOOP1, 0, 1, bool>();
#define MCF8316_FOR_EACH_CLOSED_LOOP1_FIELD(x) \
  x(OVERMODULATION_ENABLE) x(CL_ACC) x(CL_DEC) x(PWM_FREQ_OUT) x(PWM_MODE) x(FG_SEL) x(FG_DIV) x(FG_CONFIG) \
      x(FG_BEMF_THR) x(AVS_EN) x(DEADTIME_COMP_EN) x(LOW_SPEED_RECIRC_BRAKE_EN)

// CLOSED_LOOP2
enum class MotorStop : unsigned {
  HI_Z = 0,
  RECIRCULATE = 1,
  LOW_SIDE_BRAKE = 2,
  HIGH_SIDE_BRAKE = 3,
  ACTIVE_SPIN_DOWN = 4,
};
enum class MotorStopBrakeTime : unsigned {
  TIME_1_MS = 0,       // 1 ms
  TIME_5_MS = 5,       // 5 ms
  TIME_10_MS = 6,      // 10 ms
  TIME_50_MS = 7,      // 50 ms
  TIME_100_MS = 8,     // 100 ms
  TIME_250_MS = 9,     // 250 ms
  TIME_500_MS = 10,    // 500 ms
  TIME_1000_MS = 11,   // 1000 ms
  TIME_2500_MS = 12,   // 2500 ms
  TIME_5000_MS = 13,   // 5000 ms
  TIME_10000_MS = 14,  // 10000 ms
  TIME_15000_MS = 15,  // 15000 ms
};
enum class BrakeSpeedThreshold : unsigned {
  THR_100_PERCENT = 0,
  THR_90_PERCENT = 1,
  THR_80_PERCENT = 2,
  THR_70_PERCENT = 3,
  THR_60_PERCENT = 4,
  THR_50_PERCENT = 5,
  THR_45_PERCENT = 6,
  THR_40_PERCENT = 7,
  THR_35_PERCENT = 8,
  THR_30_PERCENT = 9,
  THR_25_PERCENT = 10,
  THR_20_PERCENT = 11,
  THR_15_PERCENT = 12,
  THR_10_PERCENT = 13,
  THR_5_PERCENT = 14,
  THR_2_5_PERCENT = 15,  // 2.5 %
};
constexpr auto MTR_STOP = Field<Register::CLOSED_LOOP2, 28, 3, MotorStop>();
constexpr auto MTR_STOP_BRK_TIME = Field<Register::CLOSED_LOOP2, 24, 4, MotorStopBrakeTime>();
constexpr auto ACT_SPIN_THR = Field<Register::CLOSED_LOOP2, 20, 4, unsigned>();
constexpr auto BRAKE_SPEED_THRESHOLD = Field<Register::CLOSED_LOOP2, 16, 4, BrakeSpeedThreshold>();
constexpr auto MOTOR_RES = Field<Register::CLOSED_LOOP2, 8, 8, unsigned>();
constexpr auto MOTOR_IND = Field<Register::CLOSED_LOOP2, 0, 8, unsigned>();
#define MCF8316_FOR_EACH_CLOSED_LOOP2_FIELD(x) \
  x(MTR_STOP) x(MTR_STOP_BRK_TIME) x(ACT_SPIN_THR) x(BRAKE_SPEED_THRESHOLD) x(MOTOR_RES) x(MOTOR_IND)

// CLOSED_LOOP3
constexpr auto MOTOR_BEMF_CONST = Field<Register::CLOSED_LOOP3, 23, 8, unsigned>();
constexpr auto CURR_LOOP_KP = Field<Register::CLOSED_LOOP3, 13, 10, unsigned>();
constexpr auto CURR_LOOP_KI = Field<Register::CLOSED_LOOP3, 3, 10, unsigned>();
constexpr auto SPD_LOOP_KP_MSB = Field<Register::CLOSED_LOOP3, 0, 3, unsigned>();
#define MCF8316_FOR_EACH_CLOSED_LOOP3_FIELD(x) x(MOTOR_BEMF_CONST) x(CURR_LOOP_KP) x(CURR_LOOP_KI) x(SPD_LOOP_KP_MSB)

// CLOSED_LOOP4
constexpr auto SPD_LOOP_KP_LSB = Field<Register::CLOSED_LOOP4, 24, 7, unsigned>();
constexpr auto SPD_LOOP_KI = Field<Register::CLOSED_LOOP4, 14, 10, unsigned>();
constexpr auto MAX_SPEED = Field<Register::CLOSED_LOOP4, 0, 14, unsigned>();
#define MCF8316_FOR_EACH_CLOSED_LOOP4_FIELD(x) x(SPD_LOOP_KP_LSB) x(SPD_LOOP_KI) x(MAX_SPEED)

// REF_PROFILES1
enum class ReferenceProfileConfig : unsigned {
  EQUATION = 0,
  LINEAR = 1,
  STAIRCASE = 2,
  FORWARD_REVERSE = 3,
};
constexpr auto REF_PROFILE_CONFIG = Field<Register::REF_PROFILES1, 29, 2, ReferenceProfileConfig>();
constexpr auto DUTY_ON1 = Field<Register::REF_PROFILES1, 21, 8, unsigned>();
constexpr auto DUTY_OFF1 = Field<Register::REF_PROFILES1, 13, 8, unsigned>();
constexpr auto DUTY_CLAMP1 = Field<Register::REF_PROFILES1, 5, 8, unsigned>();
constexpr auto DUTY_A_MSB = Field<Register::REF_PROFILES1, 0, 5, unsigned>();
#define MCF8316_FOR_EACH_REF_PROFILES1_FIELD(x) \
  x(REF_PROFILE_CONFIG) x(DUTY_ON1) x(DUTY_OFF1) x(DUTY_CLAMP1) x(DUTY_A_MSB)

// REF_PROFILES2
constexpr auto DUTY_A_LSB = Field<Register::REF_PROFILES2, 28, 3, unsigned>();
constexpr auto DUTY_B = Field<Register::REF_PROFILES2, 20, 8, unsigned>();
constexpr auto DUTY_C = Field<Register::REF_PROFILES2, 12, 8, unsigned>();
constexpr auto DUTY_D = Field<Register::REF_PROFILES2, 4, 8, unsigned>();
constexpr auto DUTY_E_MSB = Field<Register::REF_PROFILES2, 0, 4, unsigned>();
#define MCF8316_FOR_EACH_REF_PROFILES2_FIELD(x) x(DUTY_A_LSB) x(DUTY_B) x(DUTY_C) x(DUTY_D) x(DUTY_E_MSB)

// REF_PROFILES3
constexpr auto DUTY_E_LSB = Field<Register::REF_PROFILES3, 27, 4, unsigned>();
constexpr auto DUTY_ON2 = Field<Register::REF_PROFILES3, 19, 8, unsigned>();
constexpr auto DUTY_OFF2 = Field<Register::REF_PROFILES3, 11, 8, unsigned>();
constexpr auto DUTY_CLAMP2 = Field<Register::REF_PROFILES3, 3, 8, unsigned>();
constexpr auto DUTY_HYS = Field<Register::REF_PROFILES3, 1, 2, unsigned>();
#define MCF8316_FOR_EACH_REF_PROFILES3_FIELD(x) x(DUTY_E_LSB) x(DUTY_ON2) x(DUTY_OFF2) x(DUTY_CLAMP2) x(DUTY_HYS)

// REF_PROFILES4
constexpr auto REF_OFF1 = Field<Register::REF_PROFILES4, 23, 8, unsigned>();
constexpr auto REF_CLAMP1 = Field<Register::REF_PROFILES4, 15, 8, unsigned>();
constexpr auto REF_A = Field<Register::REF_PROFILES4, 7, 8, unsigned>();
constexpr auto REF_B_MSB = Field<Register::REF_PROFILES4, 0, 7, unsigned>();
#define MCF8316_FOR_EACH_REF_PROFILES4_FIELD(x) x(REF_OFF1) x(REF_CLAMP1) x(REF_A) x(REF_B_MSB)

// REF_PROFILES5
constexpr auto REF_B_LSB = Field<Register::REF_PROFILES5, 30, 1, unsigned>();
constexpr auto REF_C = Field<Register::REF_PROFILES5, 22, 8, unsigned>();
constexpr auto REF_D = Field<Register::REF_PROFILES5, 14, 8, unsigned>();
constexpr auto REF_E = Field<Register::REF_PROFILES5, 6, 8, unsigned>();
constexpr auto MIN_DUTY = Field<Register::REF_PROFILES5, 4, 2, unsigned>();
constexpr auto VOLTAGE_MODE_CONFIG = Field<Register::REF_PROFILES5, 2, 2, unsigned>();
constexpr auto DUTY_COMMAND_FILTER = Field<Register::REF_PROFILES5, 1, 1, unsigned>();
#define MCF8316_FOR_EACH_REF_PROFILES5_FIELD(x) \
  x(REF_B_LSB) x(REF_C) x(REF_D) x(REF_E) x(MIN_DUTY) x(VOLTAGE_MODE_CONFIG) x(DUTY_COMMAND_FILTER)

// REF_PROFILES6
constexpr auto REF_OFF2 = Field<Register::REF_PROFILES6, 23, 8, unsigned>();
constexpr auto REF_CLAMP2 = Field<Register::REF_PROFILES6, 15, 8, unsigned>();
#define MCF8316_FOR_EACH_REF_PROFILES6_FIELD(x) x(REF_OFF2) x(REF_CLAMP2)

// FAULT_CONFIG1
enum class EEPROMFaultMode : unsigned {
  LATCHED_FAULT = 0,
  REPORT_ONLY = 1,
};
enum class LockMode : unsigned {
  LATCHED_FAULT_HI_Z = 0,
  LATCHED_FAULT_LOW_SIDE_BRAKE = 1,
  LATCHED_FAULT_HIGH_SIDE_BRAKE = 2,
  AUTO_RETRY_HI_Z = 3,
  AUTO_RETRY_LOW_SIDE_BRAKE = 4,
  AUTO_RETRY_HIGH_SIDE_BRAKE = 5,
  REPORT_ONLY = 6,
  DISABLED = 7,
};
enum class LockCurrentLimitDeglitch : unsigned {
  TIME_NONE = 0,        // no deglitch
  TIME_0_1_MS = 1,      // 0.1 ms
  TIME_0_2_MS = 2,      // 0.2 ms
  TIME_0_5_MS = 3,      // 0.5 ms
  TIME_1_0_MS = 4,      // 1.0 ms
  TIME_2_5_MS = 5,      // 2.5 ms
  TIME_5_0_MS = 6,      // 5.0 ms
  TIME_7_5_MS = 7,      // 7.5 ms
  TIME_10_0_MS = 8,     // 10.0 ms
  TIME_25_0_MS = 9,     // 25.0 ms
  TIME_50_0_MS = 10,    // 50.0 ms
  TIME_75_0_MS = 11,    // 75.0 ms
  TIME_100_0_MS = 12,   // 100.0 ms
  TIME_200_0_MS = 13,   // 200.0 ms
  TIME_500_0_MS = 14,   // 500.0 ms
  TIME_1000_0_MS = 15,  // 1000.0 ms
};
enum class LockRetryTime : unsigned {
  TIME_300_MS = 0,
  TIME_500_MS = 1,
  TIME_1000_MS = 2,
  TIME_2000_MS = 3,
  TIME_3000_MS = 4,
  TIME_4000_MS = 5,
  TIME_5000_MS = 6,
  TIME_6000_MS = 7,
  TIME_7000_MS = 8,
  TIME_8000_MS = 9,
  TIME_9000_MS = 10,
  TIME_10000_MS = 11,
  TIME_11000_MS = 12,
  TIME_12000_MS = 13,
  TIME_13000_MS = 14,
  TIME_14000_MS = 15,
};
enum class CRCErrorMode : unsigned {
  LATCHED_FAULT_HI_Z = 0,
  REPORT_ONLY = 1,
};
constexpr auto ILIMIT = Field<Register::FAULT_CONFIG1, 27, 4, CurrentLimit>();
constexpr auto HW_LOCK_ILIMIT = Field<Register::FAULT_CONFIG1, 23, 4, CurrentLimit>();
constexpr auto LOCK_ILIMIT = Field<Register::FAULT_CONFIG1, 19, 4, CurrentLimit>();
constexpr auto EEP_FAULT_MODE = Field<Register::FAULT_CONFIG1, 18, 1, EEPROMFaultMode>();
constexpr auto LOCK_ILIMIT_MODE = Field<Register::FAULT_CONFIG1, 15, 3, LockMode>();
constexpr auto LOCK_ILIMIT_DEG = Field<Register::FAULT_CONFIG1, 11, 4, LockCurrentLimitDeglitch>();
constexpr auto LCK_RETRY = Field<Register::FAULT_CONFIG1, 7, 4, LockRetryTime>();
constexpr auto CRC_ERR_MODE = Field<Register::FAULT_CONFIG1, 6, 1, CRCErrorMode>();
constexpr auto MTR_LCK_MODE = Field<Register::FAULT_CONFIG1, 3, 3, LockMode>();
constexpr auto IPD_TIMEOUT_FAULT_EN = Field<Register::FAULT_CONFIG1, 2, 1, bool>();
constexpr auto IPD_FREQ_FAULT_EN = Field<Register::FAULT_CONFIG1, 1, 1, bool>();
constexpr auto SATURATION_FLAGS_EN = Field<Register::FAULT_CONFIG1, 0, 1, bool>();
#define MCF8316_FOR_EACH_FAULT_CONFIG1_FIELD(x) \
  x(ILIMIT) x(HW_LOCK_ILIMIT) x(LOCK_ILIMIT) x(EEP_FAULT_MODE) x(LOCK_ILIMIT_MODE) x(LOCK_ILIMIT_DEG) x(LCK_RETRY) \
      x(CRC_ERR_MODE) x(MTR_LCK_MODE) x(IPD_TIMEOUT_FAULT_EN) x(IPD_FREQ_FAULT_EN) x(SATURATION_FLAGS_EN)

// FAULT_CONFIG2
enum class AbnormalSpeedThreshold : unsigned {
  THR_130_PERCENT = 0,  // 130 %
  THR_140_PERCENT = 1,  // 140 %
  THR_150_PERCENT = 2,  // 150 %
  THR_160_PERCENT = 3,  // 160 %
  THR_170_PERCENT = 4,  // 170 %
  THR_180_PERCENT = 5,  // 180 %
  THR_190_PERCENT = 6,  // 190 %
  THR_200_PERCENT = 7,  // 200 %
};
enum class AbnormalBEMFThreshold : unsigned {
  THR_40_PERCENT = 0,    // 40 %
  THR_45_PERCENT = 1,    // 45 %
  THR_50_PERCENT = 2,    // 50 %
  THR_55_PERCENT = 3,    // 55 %
  THR_60_PERCENT = 4,    // 60 %
  THR_65_PERCENT = 5,    // 65 %
  THR_67_5_PERCENT = 6,  // 67.5 %
  THR_70_PERCENT = 7,    // 70 %
};
enum class NoMotorCurrentThreshold : unsigned {
  THR_0_0375_A = 0,  // 0.0375 A
  THR_0_0750_A = 1,  // 0.0750 A
  THR_0_1000_A = 2,  // 0.1000 A
  THR_0_1250_A = 3,  // 0.1250 A
  THR_0_2500_A = 4,  // 0.2500 A
  THR_0_5000_A = 5,  // 0.5000 A
  THR_0_7500_A = 6,  // 0.7500 A
  THR_1_0000_A = 7,  // 1.0000 A
};
enum class HWLockCurrentLimitDeglitch : unsigned {
  TIME_NONE = 0,  // no deglitch
  TIME_1_US = 1,  // 1 us
  TIME_2_US = 2,  // 1 us
  TIME_3_US = 3,  // 1 us
  TIME_4_US = 4,  // 1 us
  TIME_5_US = 5,  // 1 us
  TIME_6_US = 6,  // 1 us
  TIME_7_US = 7,  // 1 us
};
enum class VoltageHysteresis : unsigned {
  HYSTERESIS_0_5_V = 0,  // 0.5 V
  HYSTERESIS_1_0_V = 1,  // 1.0 V
  HYSTERESIS_2_0_V = 2,  // 2.0 V
  HYSTERESIS_3_0_V = 3,  // 3.0 V
};
enum class MinVMMotor : unsigned {
  THR_NONE = 0,  // no minimum
  THR_6_V = 1,   // 6 V
  THR_7_V = 2,   // 7 V
  THR_8_V = 3,   // 8 V
  THR_10_V = 4,  // 10 V
  THR_14_V = 5,  // 14 V
  THR_16_V = 6,  // 16 V
  THR_18_V = 7,  // 18 V
};
enum class MaxVMMotor : unsigned {
  THR_NONE = 0,  // no minimum
  THR_18_V = 1,  // 18 V
  THR_20_V = 2,  // 20 V
  THR_22_V = 3,  // 22 V
  THR_28_V = 4,  // 28 V
  THR_30_V = 5,  // 30 V
  THR_32_V = 6,  // 32 V
  THR_34_V = 7,  // 34 V
};
enum class MinMaxVMMode : unsigned {
  LATCH = 0,
  AUTO_CLEAR = 1,
};
enum class AutoRetryTimes : unsigned {
  TIMES_UNLIMITED = 0,  // no limit
  TIMES_2 = 1,          // 2 times
  TIMES_3 = 2,          // 3 times
  TIMES_5 = 3,          // 5 times
  TIMES_7 = 4,          // 7 times
  TIMES_10 = 5,         // 10 times
  TIMES_15 = 6,         // 15 times
  TIMES_20 = 7,         // 20 times
};
constexpr auto LOCK1_EN = Field<Register::FAULT_CONFIG2, 30, 1, bool>();
constexpr auto LOCK2_EN = Field<Register::FAULT_CONFIG2, 29, 1, bool>();
constexpr auto LOCK3_EN = Field<Register::FAULT_CONFIG2, 28, 1, bool>();
constexpr auto LOCK_ABN_SPEED = Field<Register::FAULT_CONFIG2, 25, 3, AbnormalSpeedThreshold>();
constexpr auto ABNORMAL_BEMF_THR = Field<Register::FAULT_CONFIG2, 22, 3, AbnormalBEMFThreshold>();
constexpr auto NO_MTR_THR = Field<Register::FAULT_CONFIG2, 19, 3, NoMotorCurrentThreshold>();
constexpr auto HW_LOCK_ILIMIT_MODE = Field<Register::FAULT_CONFIG2, 16, 3, LockMode>();
constexpr auto HW_LOCK_ILIMIT_DEG = Field<Register::FAULT_CONFIG2, 13, 3, HWLockCurrentLimitDeglitch>();
constexpr auto VOLTAGE_HYSTERESIS = Field<Register::FAULT_CONFIG2, 11, 2, VoltageHysteresis>();
constexpr auto MIN_VM_MOTOR = Field<Register::FAULT_CONFIG2, 8, 3, MinVMMotor>();
constexpr auto MIN_VM_MODE = Field<Register::FAULT_CONFIG2, 7, 1, MinMaxVMMode>();
constexpr auto MAX_VM_MOTOR = Field<Register::FAULT_CONFIG2, 4, 3, MaxVMMotor>();
constexpr auto MAX_VM_MODE = Field<Register::FAULT_CONFIG2, 3, 1, MinMaxVMMode>();
constexpr auto AUTO_RETRY_TIMES = Field<Register::FAULT_CONFIG2, 0, 3, AutoRetryTimes>();
#define MCF8316_FOR_EACH_FAULT_CONFIG2_FIELD(x) \
  x(LOCK1_EN) x(LOCK2_EN) x(LOCK3_EN) x(LOCK_ABN_SPEED) x(ABNORMAL_BEMF_THR) x(NO_MTR_THR) x(HW_LOCK_ILIMIT_MODE) \
      x(HW_LOCK_ILIMIT_DEG) x(VOLTAGE_HYSTERESIS) x(MIN_VM_MOTOR) x(MIN_VM_MODE) x(MAX_VM_MOTOR) x(MAX_VM_MODE) \
          x(AUTO_RETRY_TIMES)

// PIN_CONFIG
enum class PWMDitherStep : unsigned {
  SLEW_1 = 0,
  SLEW_2 = 1,
  SLEW_5 = 2,
  SLEW_10 = 3,
};
enum class VDCFilter : unsigned {
  DISABLE = 0,
  DEFAULT = 1,
  FREQ_100_HZ = 2,
  FREQ_1000_HZ = 3,
};
enum class BrakePinMode : unsigned {
  LOW_SIDE_BRAKE = 0,
  ALIGN_BRAKE = 1,
};
enum class BrakeInput : unsigned {
  BRAKE_PIN = 0,
  OVERRIDE_ON = 1,
  OVERRIDE_OFF = 2,
  BRAKE_PIN2 = 3,  // same behavior as BRAKE_PIN
};
enum class SpeedMode : unsigned {
  SPEED_PIN_ANALOG = 0,
  SPEED_PIN_DUTY_CYCLE = 1,
  DIGITAL_SPEED_CTRL = 2,
  SPEED_PIN_FREQUENCY = 3,
};
constexpr auto PWM_DITHER_STEP = Field<Register::PIN_CONFIG, 29, 2, PWMDitherStep>();
constexpr auto VDC_FILTER = Field<Register::PIN_CONFIG, 27, 2, VDCFilter>();
constexpr auto LEAD_ANGLE = Field<Register::PIN_CONFIG, 22, 5, unsigned>();
constexpr auto MAX_POWER = Field<Register::PIN_CONFIG, 11, 14, unsigned>();
constexpr auto FG_IDLE_CONFIG = Field<Register::PIN_CONFIG, 9, 2, unsigned>();
constexpr auto FG_FAULT_CONFIG = Field<Register::PIN_CONFIG, 7, 2, unsigned>();
constexpr auto ALARM_PIN_EN = Field<Register::PIN_CONFIG, 6, 1, bool>();
constexpr auto BRAKE_PIN_MODE = Field<Register::PIN_CONFIG, 5, 1, BrakePinMode>();
constexpr auto ALIGN_BRAKE_ANGLE_SEL = Field<Register::PIN_CONFIG, 4, 1, unsigned>();
constexpr auto BRAKE_INPUT = Field<Register::PIN_CONFIG, 2, 2, BrakeInput>();
constexpr auto SPEED_MODE = Field<Register::PIN_CONFIG, 0, 2, SpeedMode>();
#define MCF8316_FOR_EACH_PIN_CONFIG_FIELD(x) \
  x(PWM_DITHER_DEPTH) x(VDC_FILTER) x(LEAD_ANGLE) x(MAX_POWER) x(FG_IDLE_CONFIG) x(FG_FAULT_CONFIG) x(ALARM_PIN_EN) \
      x(BRAKE_PIN_MODE) x(ALIGN_BRAKE_ANGLE_SEL) x(BRAKE_INPUT) x(SPEED_MODE)

// DEVICE_CONFIG1
enum class DACSelect : unsigned {
  DACOUT2 = 0,
  SOA = 1,
  SOB = 2,
  SOC = 3,
};
enum class PWMDitherMode : unsigned {
  TRIANGULAR = 0,
  RANDOM = 1,
};
enum class SlewRateI2CPins : unsigned {
  SLEW_4_8_MA = 0,   // 4.8 mA
  SLEW_3_9_MA = 1,   // 3.9 mA
  SLEW_1_86_MA = 2,  // 1.86 mA
  SLEW_30_8_MA = 3,  // 30.8 mA
};
enum class BusVoltage : unsigned {
  BUS_15_V = 0,
  BUS_30_V = 1,
  BUS_40_V = 2,
};
constexpr auto DAC_SOx_SEL = Field<Register::DEVICE_CONFIG1, 28, 2, DACSelect>();
constexpr auto PWM_DITHER_MODE = Field<Register::DEVICE_CONFIG1, 27, 1, PWMDitherMode>();
constexpr auto I2C_TARGET_ADDR = Field<Register::DEVICE_CONFIG1, 20, 7, uint8_t>();
constexpr auto EEPROM_LOCK_KEY = Field<Register::DEVICE_CONFIG1, 5, 15, unsigned>();
constexpr auto SLEW_RATE_I2C_PINS = Field<Register::DEVICE_CONFIG1, 3, 2, SlewRateI2CPins>();
constexpr auto PULLUP_ENABLE = Field<Register::DEVICE_CONFIG1, 2, 1, bool>();
constexpr auto BUS_VOLT = Field<Register::DEVICE_CONFIG1, 0, 2, BusVoltage>();
#define MCF8316_FOR_EACH_DEVICE_CONFIG1_FIELD(x) \
  x(DAC_SOx_SEL) x(PWM_DITHER_MODE) x(I2C_TARGET_ADDR) x(EEPROM_LOCK_KEY) x(SLEW_RATE_I2C_PINS) x(PULLUP_ENABLE) \
      x(BUS_VOLT)

// DEVICE_CONFIG2
enum class SleepEntryTime : unsigned {
  SLEEP_AFTER_50_US = 0,
  SLEEP_AFTER_200_US = 1,
  SLEEP_AFTER_20_MS = 2,
  SLEEP_AFTER_200_MS = 3,
};
enum class DeviceMode : unsigned {
  STANDBY = 0,
  SLEEP = 1,
};
enum class PWMDitherDepth : unsigned {
  DEPTH_DISABLED = 0,     // no dither
  DEPTH_5_PERCENT = 1,    // 5 %
  DEPTH_7_5_PERCENT = 2,  // 7.5 %
  DEPTH_10_PERCENT = 3,   // 10 %
};
enum class WatchdogTimeout : unsigned {
  TIMEOUT_I2C_1_S_GPIO_100_MS = 0,
  TIMEOUT_I2C_2_S_GPIO_200_MS = 1,
  TIMEOUT_I2C_5_S_GPIO_500_MS = 2,
  TIMEOUT_I2C_10_S_GPIO_1000_MS = 3,
};
enum class WatchdogInputMode : unsigned {
  I2C = 0,
  GPIO = 1,
};
enum class WatchdogFaultMode : unsigned {
  REPORT_ONLY = 0,
  LATCH_HI_Z = 1,
};
constexpr auto INPUT_MAXIMUM_FREQ = Field<Register::DEVICE_CONFIG2, 16, 15, unsigned>();
constexpr auto SLEEP_ENTRY_TIME = Field<Register::DEVICE_CONFIG2, 14, 2, SleepEntryTime>();
constexpr auto DYNAMIC_CSA_GAIN_EN = Field<Register::DEVICE_CONFIG2, 13, 1, bool>();
constexpr auto DYNAMIC_VOLTAGE_GAIN_EN = Field<Register::DEVICE_CONFIG2, 12, 1, bool>();
constexpr auto DEV_MODE = Field<Register::DEVICE_CONFIG2, 11, 1, DeviceMode>();
constexpr auto PWM_DITHER_DEPTH = Field<Register::DEVICE_CONFIG2, 9, 2, PWMDitherDepth>();
constexpr auto EXT_CLK_EN = Field<Register::DEVICE_CONFIG2, 8, 1, bool>();
constexpr auto EXT_CLK_CONFIG = Field<Register::DEVICE_CONFIG2, 5, 3, unsigned>();
constexpr auto EXT_WDT_EN = Field<Register::DEVICE_CONFIG2, 4, 1, bool>();
constexpr auto EXT_WDT_CONFIG = Field<Register::DEVICE_CONFIG2, 2, 2, WatchdogTimeout>();
constexpr auto EXT_WDT_INPUT_MODE = Field<Register::DEVICE_CONFIG2, 1, 1, WatchdogInputMode>();
constexpr auto EXT_WDT_FAULT_MODE = Field<Register::DEVICE_CONFIG2, 0, 1, WatchdogFaultMode>();
#define MCF8316_FOR_EACH_DEVICE_CONFIG2_FIELD(x) \
  x(INPUT_MAXIMUM_FREQ) x(SLEEP_ENTRY_TIME) x(DYNAMIC_CSA_GAIN_EN) x(DYNAMIC_VOLTAGE_GAIN_EN) x(DEV_MODE) \
      x(PWM_DITHER_DEPTH) x(EXT_CLK_EN) x(EXT_CLK_CONFIG) x(EXT_WDT_EN) x(EXT_WDT_CONFIG) x(EXT_WDT_INPUT_MODE) \
          x(EXT_WDT_FAULT_MODE)

// PERI_CONFIG1
enum class AbnormalBEMFPersistentTime : unsigned {
  TIME_2_CYCLES = 0,
  TIME_500_MS = 1,
  TIME_1000_MS = 2,
  TIME_2000_MS = 3,
};
enum class InputReferenceWindow : unsigned {
  WINDOW_DISABLED = 0,    // no window
  WINDOW_5_PERCENT = 1,   // 5 %
  WINDOW_10_PERCENT = 2,  // 10 %
  WINDOW_15_PERCENT = 3,  // 15 %
};
enum class DirInput : unsigned {
  DIR_PIN = 0,
  OVERRIDE_CLOCKWISE = 1,
  OVERRIDE_COUNTER_CLOCKWISE = 2,
  DIR_PIN2 = 3,  // same behavior as DIR_PIN
};
enum class DirChangeMode : unsigned {
  STOP = 0,
  REVERSE_DRIVE = 1,
};
enum class InputReferenceMode : unsigned {
  SPEED = 0,
  POWER = 1,
  TORQUE = 2,
  MODULATION_INDEX = 3,
};
enum class EEPROMLockMode : unsigned {
  READ_OPEN_AND_WRITE_OPEN = 0,
  READ_PASSCODE_AND_WRITE_PASSCODE = 1,
  READ_PASSCODE_AND_WRITE_LOCKED = 2,
  READ_LOCKED_AND_WRITE_LOCKED = 3,
};
constexpr auto SPREAD_SPECTRUM_MODULATION_DIS = Field<Register::PERI_CONFIG1, 30, 1, bool>();
constexpr auto NO_MTR_FLT_CLOSEDLOOP_DIS = Field<Register::PERI_CONFIG1, 28, 1, bool>();
constexpr auto ABNORMAL_BEMF_PERSISTENT_TIME = Field<Register::PERI_CONFIG1, 26, 2, AbnormalBEMFPersistentTime>();
constexpr auto FLUX_WEAK_REF = Field<Register::PERI_CONFIG1, 24, 2, unsigned>();
constexpr auto INPUT_REFERENCE_WINDOW = Field<Register::PERI_CONFIG1, 22, 2, InputReferenceWindow>();
constexpr auto BUS_POWER_LIMIT_ENABLE = Field<Register::PERI_CONFIG1, 21, 1, bool>();
constexpr auto DIR_INPUT = Field<Register::PERI_CONFIG1, 19, 2, DirInput>();
constexpr auto DIR_CHANGE_MODE = Field<Register::PERI_CONFIG1, 18, 1, DirChangeMode>();
constexpr auto SPEED_LIMIT_ENABLE = Field<Register::PERI_CONFIG1, 17, 1, bool>();
constexpr auto ACTIVE_BRAKE_SPEED_DELTA_LIMIT_ENTRY = Field<Register::PERI_CONFIG1, 13, 4, unsigned>();
constexpr auto ACTIVE_BRAKE_MOD_INDEX_LIMIT = Field<Register::PERI_CONFIG1, 10, 3, unsigned>();
constexpr auto SPEED_RANGE_SEL = Field<Register::PERI_CONFIG1, 9, 1, unsigned>();
constexpr auto INPUT_REFERENCE_MODE = Field<Register::PERI_CONFIG1, 7, 2, InputReferenceMode>();
constexpr auto EEPROM_LOCK_MODE = Field<Register::PERI_CONFIG1, 5, 2, EEPROMLockMode>();
#define MCF8316_FOR_EACH_PERI_CONFIG1_FIELD(x) \
  x(SPREAD_SPECTRUM_MODULATION_DIS) x(NO_MTR_FLT_CLOSEDLOOP_DIS) x(ABNORMAL_BEMF_PERSISTENT_TIME) x(FLUX_WEAK_REF) \
      x(INPUT_REFERENCE_WINDOW) x(BUS_POWER_LIMIT_ENABLE) x(DIR_INPUT) x(DIR_CHANGE_MODE) x(SPEED_LIMIT_ENABLE) \
          x(ACTIVE_BRAKE_SPEED_DELTA_LIMIT_ENTRY) x(ACTIVE_BRAKE_MOD_INDEX_LIMIT) x(SPEED_RANGE_SEL) \
              x(INPUT_REFERENCE_MODE) x(EEPROM_LOCK_MODE)

// GD_CONFIG1
enum class SlewRate : unsigned {
  SLEW_125_V_US = 2,  // 125 V/us
  SLEW_200_V_US = 3,  // 200 V/us
};
enum class OvervoltageLevel : unsigned {
  LIMIT_34_V = 0,  // 34 V
  LIMIT_22_V = 1,  // 22 V
};
enum class OvercurrentDeglitch : unsigned {
  TIME_0_2_US = 0,  // 0.2 us
  TIME_0_6_US = 1,  // 0.6 us
  TIME_1_2_US = 2,  // 1.2 us
  TIME_1_6_US = 3,  // 1.6 us
};
enum class OvercurrentLevel : unsigned {
  LIMIT_16_A = 0,  // 16 A
  LIMIT_24_A = 1,  // 24 A
};
enum class OvercurrentMode : unsigned {
  LATCHED_FAULT = 0,  // latched fault
  AUTO_RETRY = 1,     // auto-retry after 500 ms
};
constexpr auto SLEW_RATE = Field<Register::GD_CONFIG1, 26, 2, SlewRate>();
constexpr auto OVP_SEL = Field<Register::GD_CONFIG1, 19, 1, OvervoltageLevel>();
constexpr auto OVP_EN = Field<Register::GD_CONFIG1, 18, 1, bool>();
constexpr auto OTW_REP = Field<Register::GD_CONFIG1, 16, 1, bool>();
constexpr auto OCP_DEG = Field<Register::GD_CONFIG1, 12, 2, OvercurrentDeglitch>();
constexpr auto OCP_LVL = Field<Register::GD_CONFIG1, 10, 1, OvercurrentLevel>();
constexpr auto OCP_MODE = Field<Register::GD_CONFIG1, 8, 2, OvercurrentMode>();
constexpr auto CSA_GAIN = Field<Register::GD_CONFIG1, 0, 2, unsigned>();
#define MCF8316_FOR_EACH_GD_CONFIG1_FIELD(x) \
  x(SLEW_RATE) x(OVP_SEL) x(OVP_EN) x(OTW_REP) x(OCP_DEG) x(OCP_LVL) x(OCP_MODE) x(CSA_GAIN)

// GD_CONFIG2
enum class BuckCurrentLimit : unsigned {
  LIMIT_600_MA = 0,  // 600 mA
  LIMIT_150_MA = 1,  // 150 mA
};
enum class BuckVoltage : unsigned {
  OUTPUT_3_3_V = 0,  // 3.3 V
  OUTPUT_5_0_V = 1,  // 5.0 V
  OUTPUT_4_0_V = 2,  // 4.0 V
  OUTPUT_5_7_V = 3,  // 5.7 V
};
enum class MinOnTime : unsigned {
  TIME_0_00_US = 0,  // 0 us
  TIME_AUTO = 1,     // automatic based on slew rate
  TIME_0_50_US = 2,  // 0.5 us
  TIME_0_75_US = 3,  // 0.75 us
  TIME_1_00_US = 4,  // 1.0 us
  TIME_1_25_US = 5,  // 1.25 us
  TIME_1_50_US = 6,  // 1.5 us
  TIME_2_00_US = 7,  // 2.0 us
};
constexpr auto BUCK_PS_DIS = Field<Register::GD_CONFIG2, 24, 1, bool>();
constexpr auto BUCK_CL = Field<Register::GD_CONFIG2, 23, 1, BuckCurrentLimit>();
constexpr auto BUCK_SEL = Field<Register::GD_CONFIG2, 21, 2, BuckVoltage>();
constexpr auto BUCK_DIS = Field<Register::GD_CONFIG2, 20, 1, bool>();
constexpr auto MIN_ON_TIME = Field<Register::GD_CONFIG2, 17, 3, MinOnTime>();
#define MCF8316_FOR_EACH_GD_CONFIG2_FIELD(x) x(BUCK_PS_DIS) x(BUCK_CL) x(BUCK_SEL) x(BUCK_DIS) x(MIN_ON_TIME)

// INT_ALGO1
enum class SpeedPinGlitchFilter : unsigned {
  TIME_NONE = 0,    // no filter
  TIME_0_2_US = 1,  // 0.2 us
  TIME_0_5_US = 2,  // 0.5 us
  TIME_1_0_US = 3,  // 1.0 us
};
enum class ISDStopTime : unsigned {
  TIME_1_MS = 0,    // 1 ms
  TIME_5_MS = 1,    // 5 ms
  TIME_50_MS = 2,   // 50 ms
  TIME_100_MS = 3,  // 100 ms
};
using ISDRunTime = ISDStopTime;
enum class ISDTimeout : unsigned {
  TIME_500_MS = 0,   // 500 ms
  TIME_750_MS = 1,   // 750 ms
  TIME_1000_MS = 2,  // 1000 ms
  TIME_2000_MS = 3,  // 2000 ms
};
enum class AutoHandoffMinBEMF : unsigned {
  BEMF_0_MV = 0,     // 0 mV
  BEMF_50_MV = 1,    // 50 mV
  BEMF_100_MV = 2,   // 100 mV
  BEMF_250_MV = 3,   // 250 mV
  BEMF_500_MV = 4,   // 500 mV
  BEMF_1000_MV = 5,  // 1000 mV
  BEMF_1250_MV = 6,  // 1250 mV
  BEMF_1500_MV = 7,  // 1500 mV
};
enum class BrakeCurrentPersistence : unsigned {
  TIME_50_MS = 0,   // 50 ms
  TIME_100_MS = 1,  // 100 ms
  TIME_250_MS = 2,  // 250 ms
  TIME_500_MS = 3,  // 500 ms
};
constexpr auto ACTIVE_BRAKE_SPEED_DELTA_LIMIT_EXIT = Field<Register::INT_ALGO1, 29, 2, unsigned>();
constexpr auto SPEED_PIN_GLITCH_FILTER = Field<Register::INT_ALGO1, 27, 2, SpeedPinGlitchFilter>();
constexpr auto FAST_ISD_EN = Field<Register::INT_ALGO1, 26, 1, bool>();
constexpr auto ISD_STOP_TIME = Field<Register::INT_ALGO1, 24, 2, ISDStopTime>();
constexpr auto ISD_RUN_TIME = Field<Register::INT_ALGO1, 22, 2, ISDRunTime>();
constexpr auto ISD_TIMEOUT = Field<Register::INT_ALGO1, 20, 2, ISDTimeout>();
constexpr auto AUTO_HANDOFF_MIN_BEMF = Field<Register::INT_ALGO1, 17, 3, AutoHandoffMinBEMF>();
constexpr auto BRAKE_CURRENT_PERSIST = Field<Register::INT_ALGO1, 15, 2, BrakeCurrentPersistence>();
constexpr auto REV_DRV_OPEN_LOOP_DEC = Field<Register::INT_ALGO1, 0, 3, unsigned>();
#define MCF8316_FOR_EACH_INT_ALGO1_FIELD(x) \
  x(ACTIVE_BRAKE_SPEED_DELTA_LIMIT_EXIT) x(SPEED_PIN_GLITCH_FILTER) x(FAST_ISD_EN) x(ISD_STOP_TIME) x(ISD_RUN_TIME) \
      x(ISD_TIMEOUT) x(AUTO_HANDOFF_MIN_BEMF) x(BRAKE_CURRENT_PERSIST) x(REV_DRV_OPEN_LOOP_DEC)

// INT_ALGO2
enum ClosedLoopSlowAcceleration : unsigned {
  ACCEL_0_1_HZ_S = 0,      // 0.1 Hz/s
  ACCEL_1_0_HZ_S = 1,      // 1.0 Hz/s
  ACCEL_2_0_HZ_S = 2,      // 2.0 Hz/s
  ACCEL_3_0_HZ_S = 3,      // 3.0 Hz/s
  ACCEL_5_0_HZ_S = 4,      // 5.0 Hz/s
  ACCEL_10_0_HZ_S = 5,     // 10.0 Hz/s
  ACCEL_20_0_HZ_S = 6,     // 20.0 Hz/s
  ACCEL_30_0_HZ_S = 7,     // 30.0 Hz/s
  ACCEL_40_0_HZ_S = 8,     // 40.0 Hz/s
  ACCEL_50_0_HZ_S = 9,     // 50.0 Hz/s
  ACCEL_100_0_HZ_S = 10,   // 100.0 Hz/s
  ACCEL_200_0_HZ_S = 11,   // 200.0 Hz/s
  ACCEL_500_0_HZ_S = 12,   // 500.0 Hz/s
  ACCEL_750_0_HZ_S = 13,   // 750.0 Hz/s
  ACCEL_1000_0_HZ_S = 14,  // 1000.0 Hz/s
  ACCEL_2000_0_HZ_S = 15,  // 2000.0 Hz/s
};
constexpr auto FLUX_WEAK_KP = Field<Register::INT_ALGO2, 21, 10, unsigned>();
constexpr auto FLUX_WEAK_KI = Field<Register::INT_ALGO2, 11, 5, unsigned>();
constexpr auto FLUX_WEAK_ENABLE = Field<Register::INT_ALGO2, 10, 1, bool>();
constexpr auto CL_SLOW_ACC = Field<Register::INT_ALGO2, 6, 4, ClosedLoopSlowAcceleration>();
constexpr auto ACTIVE_BRAKE_BUS_CURRENT_SLEW_RATE = Field<Register::INT_ALGO2, 3, 3, unsigned>();
constexpr auto ISD_BEMF_FILT_ENABLE = Field<Register::INT_ALGO2, 2, 1, bool>();
constexpr auto CIRCULAR_CURRENT_LIMIT_ENABLE = Field<Register::INT_ALGO2, 1, 1, bool>();
constexpr auto IPD_HIGH_RESOLUTION_EN = Field<Register::INT_ALGO2, 1, 1, bool>();
#define MCF8316_FOR_EACH_INT_ALGO2_FIELD(x) \
  x(FLUX_WEAK_KP) x(FLUX_WEAK_KI) x(FLUX_WEAK_ENABLE) x(CL_SLOW_ACC) x(ACTIVE_BRAKE_BUS_CURRENT_SLEW_RATE) \
      x(ISD_BEMF_FILT_ENABLE) x(CIRCULAR_CURRENT_LIMIT_ENABLE) x(IPD_HIGH_RESOLUTION_EN)

// Composite EEPROM fields split across two registers
constexpr auto SPD_LOOP_KP = make_composite_field<unsigned>(SPD_LOOP_KP_MSB, SPD_LOOP_KP_LSB);
constexpr auto DUTY_A = make_composite_field<unsigned>(DUTY_A_MSB, DUTY_A_LSB);
constexpr auto DUTY_E = make_composite_field<unsigned>(DUTY_E_MSB, DUTY_E_LSB);
constexpr auto REF_B = make_composite_field<unsigned>(REF_B_MSB, REF_B_LSB);
#define MCF8316_FOR_EACH_COMPOSITE_EEPROM_FIELD(x) x(SPD_LOOP_KP) x(DUTY_A) x(DUTY_E) x(REF_B)

// Other register fields of interest (not an exhaustive list)
// MTR_PARAMS
constexpr auto MPET_MOTOR_R = Field<Register::MTR_PARAMS, 24, 8, unsigned>();
constexpr auto MPET_MOTOR_BEMF_CONST = Field<Register::MTR_PARAMS, 16, 8, unsigned>();
constexpr auto MPET_MOTOR_L = Field<Register::MTR_PARAMS, 8, 8, unsigned>();

// ALGO_STATUS_MPEG
constexpr auto MPET_R_STATUS = Field<Register::ALGO_STATUS_MPET, 31, 1, bool>();
constexpr auto MPET_L_STATUS = Field<Register::ALGO_STATUS_MPET, 30, 1, bool>();
constexpr auto MPET_KE_STATUS = Field<Register::ALGO_STATUS_MPET, 29, 1, bool>();
constexpr auto MPET_MECH_STATUS = Field<Register::ALGO_STATUS_MPET, 28, 1, bool>();
constexpr auto MPET_PWM_FREQ = Field<Register::ALGO_STATUS_MPET, 24, 4, unsigned>();

// ALGO_CTRL1
constexpr auto EEPROM_WRT = Field<Register::ALGO_CTRL1, 31, 1, bool>();
constexpr auto EEPROM_READ = Field<Register::ALGO_CTRL1, 30, 1, bool>();
constexpr auto CLR_FLT = Field<Register::ALGO_CTRL1, 29, 1, bool>();
constexpr auto CLR_FLT_RETRY_COUNT = Field<Register::ALGO_CTRL1, 28, 1, bool>();
constexpr auto EEPROM_WRITE_ACCESS_KEY = Field<Register::ALGO_CTRL1, 20, 8, unsigned>();
constexpr auto WATCHDOG_TICKLE = Field<Register::ALGO_CTRL1, 10, 1, bool>();

// ALGO_DEBUG1
constexpr auto DIGITAL_SPEED_OVERRIDE = Field<Register::ALGO_DEBUG1, 31, 1, bool>();
constexpr auto DIGITAL_SPEED_CTRL = Field<Register::ALGO_DEBUG1, 16, 15, unsigned>();

// ALGO_DEBUG2
constexpr auto MPET_CMD = Field<Register::ALGO_DEBUG2, 5, 1, unsigned>();
constexpr auto MPET_R = Field<Register::ALGO_DEBUG2, 4, 1, bool>();
constexpr auto MPET_L = Field<Register::ALGO_DEBUG2, 3, 1, bool>();
constexpr auto MPET_KE = Field<Register::ALGO_DEBUG2, 2, 1, bool>();
constexpr auto MPET_MECH = Field<Register::ALGO_DEBUG2, 1, 1, bool>();
constexpr auto MPET_WRITE_SHADOW = Field<Register::ALGO_DEBUG2, 0, 1, bool>();

// CURRENT_PI (fields renamed to avoid conflict with similarly named CURR_LOOP_KI/KI)
constexpr auto CURRENT_PI_LOOP_KI = Field<Register::CURRENT_PI, 16, 16, unsigned>();
constexpr auto CURRENT_PI_LOOP_KP = Field<Register::CURRENT_PI, 0, 16, unsigned>();

// SPEED_PI
constexpr auto SPEED_PI_LOOP_KI = Field<Register::SPEED_PI, 16, 16, unsigned>();
constexpr auto SPEED_PI_LOOP_KP = Field<Register::SPEED_PI, 0, 16, unsigned>();

// ALGORITHM_STATE
enum class AlgorithmState : unsigned {
  MOTOR_IDLE = 0x0,
  MOTOR_ISD = 0x1,
  MOTOR_TRISTATE = 0x2,
  MOTOR_BRAKE_ON_START = 0x3,
  MOTOR_IPD = 0x4,
  MOTOR_SLOW_FIRST_CYCLE = 0x5,
  MOTOR_ALIGN = 0x6,
  MOTOR_OPEN_LOOP = 0x7,
  MOTOR_CLOSED_LOOP_UNALIGNED = 0x8,
  MOTOR_CLOSED_LOOP_ALIGNED = 0x9,
  MOTOR_CLOSED_LOOP_ACTIVE_BRAKING = 0xa,
  MOTOR_SOFT_STOP = 0xb,
  MOTOR_RECIRCULATE_STOP = 0xc,
  MOTOR_BRAKE_ON_STOP = 0xd,
  MOTOR_FAULT = 0xe,
  MOTOR_MPET_MOTOR_STOP_CHECK = 0xf,
  MOTOR_MPET_MOTOR_STOP_WAIT = 0x10,
  MOTOR_MPET_MOTOR_BRAKE = 0x11,
  MOTOR_MPET_ALGORITHM_PARAMETERS_INIT = 0x12,
  MOTOR_MPET_RL_MEASURE = 0x13,
  MOTOR_MPET_KE_MEASURE = 0x14,
  MOTOR_MPET_STALL_CURRENT_MEASURE = 0x15,
  MOTOR_MPET_TORQUE_MODE = 0x16,
  MOTOR_MPET_DONE = 0x17,
  MOTOR_MPET_FAULT = 0x18,
};
bool is_mpet_running(AlgorithmState state);
const char *algorithm_state_name(AlgorithmState state);
constexpr auto ALGORITHM_STATE = Field<Register::ALGORITHM_STATE, 0, 16, AlgorithmState>();

// GATE_DRIVER_FAULT_STATUS
enum GateDriverFaultStatus : uint32_t {
  GATE_DRIVER_FAULT = 1u << 31,
  GATE_DRIVER_OCP = 1u << 28,
  GATE_DRIVER_OVP = 1u << 26,
  GATE_DRIVER_OTW = 1u << 23,
  GATE_DRIVER_OTS = 1u << 22,
  GATE_DRIVER_OCP_HC = 1u << 21,
  GATE_DRIVER_OCP_LC = 1u << 20,
  GATE_DRIVER_OCP_HB = 1u << 19,
  GATE_DRIVER_OCP_LB = 1u << 18,
  GATE_DRIVER_OCP_HA = 1u << 17,
  GATE_DRIVER_OCP_LA = 1u << 16,
  GATE_DRIVER_BUCK_OCP = 1u << 13,
  GATE_DRIVER_BUCK_UV = 1u << 12,
  GATE_DRIVER_VCP_UV = 1u << 11,
};

// CONTROLLER_FAULT_STATUS
enum ControllerFaultStatus : uint32_t {
  CONTROLLER_FAULT = 1u << 31,
  CONTROLLER_IPD_FREQ_FAULT = 1u << 29,
  CONTROLLER_IPD_T1_FAULT = 1u << 28,
  CONTROLLER_IPD_T2_FAULT = 1u << 27,
  CONTROLLER_MPET_IPD_FAULT = 1u << 25,
  CONTROLLER_MPET_BEMF_FAULT = 1u << 24,
  CONTROLLER_ABN_SPEED = 1u << 23,
  CONTROLLER_ABN_BEMF = 1u << 22,
  CONTROLLER_NO_MTR = 1u << 21,
  CONTROLLER_MTR_LCK = 1u << 20,
  CONTROLLER_LOCK_ILIMIT = 1u << 19,
  CONTROLLER_HW_LOCK_ILIMIT = 1u << 18,
  CONTROLLER_MTR_UNDER_VOLTAGE = 1u << 17,
  CONTROLLER_MTR_OVER_VOLTAGE = 1u << 16,
  CONTROLLER_SPEED_LOOP_SATURATION = 1u << 15,
  CONTROLLER_CURRENT_LOOP_SATURATION = 1u << 14,
  CONTROLLER_MAX_SPEED_SATURATION = 1u << 13,
  CONTROLLER_BUS_POWER_LIMIT_SATURATION = 1u << 12,
  CONTROLLER_EEPROM_WRITE_LOCK_SET = 1u << 11,
  CONTROLLER_EEPROM_READ_LOCK_SET = 1u << 10,
  CONTROLLER_I2C_CRC_FAULT_STATUS = 1u << 6,
  CONTROLLER_EEPROM_ERR_STATUS = 1u << 5,
  CONTROLLER_BOOT_STL_FAULT = 1u << 4,
  CONTROLLER_WATCHDOG_FAULT = 1u << 3,
  CONTROLLER_CPU_RESET_FAULT_STATUS = 1u << 2,
  CONTROLLER_WWDT_FAULT_STATUS = 1u << 1,
};

// Format a bit-packed fault status value to a human-readable string.
std::string format_gate_driver_fault_status(GateDriverFaultStatus value);
std::string format_controller_fault_status(ControllerFaultStatus value);

// Discards the parity bit from a config register.
constexpr uint32_t discard_config_register_parity(uint32_t value) {
  return value & 0x7fffffffu;
}

// Holds the value of a specific register and provides type-safe access to its fields.
struct RegisterValue_ {
  uint32_t value{};

  // Compares two config registers ignoring the parity bit.
  bool equals_ignoring_config_register_parity(const RegisterValue_ &other) const {
    return discard_config_register_parity(this->value) == discard_config_register_parity(other.value);
  }
};
template<Register reg> struct RegisterValue final : public RegisterValue_ {
  template<unsigned bit, unsigned width, typename T> T get(Field<reg, bit, width, T> field) const {
    return T((this->value & field.mask) >> field.bit);
  }

  template<unsigned bit, unsigned width, typename T>
  RegisterValue &set(Field<reg, bit, width, T> field, T field_value) {
    this->value = (this->value & ~field.mask) | ((uint32_t(field_value) << field.bit) & field.mask);
    return *this;
  }
};

// Holds a copy of the configuration registers and provides type-safe access to their fields.
// These registers are loaded from EEPROM on reset (all other registers are volatile).
struct Config final {
  static constexpr size_t LENGTH = 24;

  std::array<RegisterValue_, LENGTH> register_values = {};

  static constexpr Register index_to_register(size_t index) {
    return Register(index * 2 + size_t(Register::ISD_CONFIG));
  }
  static constexpr size_t register_to_index(Register reg) {
    return (size_t(reg) - size_t(Register::ISD_CONFIG)) / 2;
  }

  template<Register reg> RegisterValue<reg> &at() {
    static_assert(is_config_register(reg), "Not a configuration register");
    return *static_cast<RegisterValue<reg> *>(&this->register_values[register_to_index(reg)]);
  }

  template<Register reg> const RegisterValue<reg> &at() const {
    static_assert(is_config_register(reg), "Not a configuration register");
    return *static_cast<const RegisterValue<reg> *>(&this->register_values[register_to_index(reg)]);
  }

  template<Register reg, unsigned bit, unsigned width, typename T> T get(Field<reg, bit, width, T> field) const {
    return at<reg>().get(field);
  }

  template<Register reg, unsigned bit, unsigned width, typename T>
  Config &set(Field<reg, bit, width, T> field, T field_value) {
    at<reg>().set(field, field_value);
    return *this;
  }

  template<typename MSBField, typename LSBField, typename T> T get(CompositeField<MSBField, LSBField, T> field) const {
    return T((get(field.msb) << LSBField::width) | get(field.lsb));
  }

  template<typename MSBField, typename LSBField, typename T>
  Config &set(CompositeField<MSBField, LSBField, T> field, T field_value) {
    set(field.msb, typename MSBField::Type(field_value >> LSBField::width));
    set(field.lsb, typename LSBField::Type(field_value));
    return *this;
  }

  // Returns true if some motor parameters have not been explicitly configured.
  // The MCF8316D will automatically run the MPET tool when a speed command is issued
  // but it's more robust to set everything up-front.
  bool needs_mpet_for_speed_loop() const;

  // Compares all config registers and ignores the parity bit.
  bool equals_ignoring_config_register_parity(const Config &other) const;
};

// Maximum power is expressed as a fraction of 100 W.
constexpr float max_power_to_watts(unsigned max_power) {
  return float(max_power) * 100 / 2048;
}
constexpr unsigned max_power_from_watts(float watts) {
  return unsigned(std::clamp(watts * 2048 / 100, 0.f, 2047.f));
}

// The number of motor poles is between 0 and 30 and always even.
// When set to 0, speed calculations use electrical Hz instead of rotor Hz.
constexpr unsigned fg_div_to_motor_poles(unsigned fg_div) {
  return fg_div << 1;
}
constexpr unsigned fg_div_from_motor_poles(unsigned motor_poles) {
  if (motor_poles >= 0 && motor_poles <= 30u && (motor_poles & 1u) == 0u) {
    return motor_poles >> 1;
  }
  return 0;  // invalid
}

// The BEMF lead angle is a value between -48 and 45 degrees in steps of 3 degrees.
constexpr int lead_angle_to_degrees(unsigned lead_angle) {
  return lead_angle < 16u ? int(lead_angle * 3u) : -int((32u - lead_angle) * 3u);
}
constexpr unsigned lead_angle_from_degrees(int degrees) {
  if (degrees >= -48 && degrees <= 45 && abs(degrees) % 3 == 0) {
    return degrees >= 0 ? unsigned(degrees) / 3u : 32u - unsigned(-degrees) / 3u;
  }
  return 0;  // invalid
}

// Converts speed value from rotor Hz to electrical Hz based on `FG_DIV`.
constexpr float convert_speed_in_electrical_hz_to_rotor_hz(float speed_in_electrical_hz, unsigned fg_div) {
  return fg_div == 0 ? speed_in_electrical_hz * 3 : speed_in_electrical_hz / fg_div;
}

// Converts speed value from rotor Hz to electrical Hz based on `FG_DIV`.
constexpr float convert_speed_in_rotor_hz_to_electrical_hz(float speed_in_rotor_hz, unsigned fg_div) {
  return fg_div == 0 ? speed_in_rotor_hz / 3 : speed_in_rotor_hz * fg_div;
}

// Write the configuration to the log.
void log_config(const Config &config);

}  // namespace mcf8316
}  // namespace esphome
