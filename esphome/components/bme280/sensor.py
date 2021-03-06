import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import i2c, sensor
from esphome.const import (
    CONF_HUMIDITY,
    CONF_ID,
    CONF_IIR_FILTER,
    CONF_OVERSAMPLING,
    CONF_PRESSURE,
    CONF_TEMPERATURE,
    DEVICE_CLASS_HUMIDITY,
    DEVICE_CLASS_PRESSURE,
    DEVICE_CLASS_TEMPERATURE,
    ICON_EMPTY,
    STATE_CLASS_MEASUREMENT,
    UNIT_CELSIUS,
    UNIT_HECTOPASCAL,
    UNIT_PERCENT,
)

DEPENDENCIES = ["i2c"]

bme280_ns = cg.esphome_ns.namespace("bme280")
BME280Oversampling = bme280_ns.enum("BME280Oversampling")
OVERSAMPLING_OPTIONS = {
    "NONE": BME280Oversampling.BME280_OVERSAMPLING_NONE,
    "1X": BME280Oversampling.BME280_OVERSAMPLING_1X,
    "2X": BME280Oversampling.BME280_OVERSAMPLING_2X,
    "4X": BME280Oversampling.BME280_OVERSAMPLING_4X,
    "8X": BME280Oversampling.BME280_OVERSAMPLING_8X,
    "16X": BME280Oversampling.BME280_OVERSAMPLING_16X,
}

BME280IIRFilter = bme280_ns.enum("BME280IIRFilter")
IIR_FILTER_OPTIONS = {
    "OFF": BME280IIRFilter.BME280_IIR_FILTER_OFF,
    "2X": BME280IIRFilter.BME280_IIR_FILTER_2X,
    "4X": BME280IIRFilter.BME280_IIR_FILTER_4X,
    "8X": BME280IIRFilter.BME280_IIR_FILTER_8X,
    "16X": BME280IIRFilter.BME280_IIR_FILTER_16X,
}

BME280Component = bme280_ns.class_(
    "BME280Component", cg.PollingComponent, i2c.I2CDevice
)

CONFIG_SCHEMA = (
    cv.Schema(
        {
            cv.GenerateID(): cv.declare_id(BME280Component),
            cv.Optional(CONF_TEMPERATURE): sensor.sensor_schema(
                UNIT_CELSIUS,
                ICON_EMPTY,
                1,
                DEVICE_CLASS_TEMPERATURE,
                STATE_CLASS_MEASUREMENT,
            ).extend(
                {
                    cv.Optional(CONF_OVERSAMPLING, default="16X"): cv.enum(
                        OVERSAMPLING_OPTIONS, upper=True
                    ),
                }
            ),
            cv.Optional(CONF_PRESSURE): sensor.sensor_schema(
                UNIT_HECTOPASCAL,
                ICON_EMPTY,
                1,
                DEVICE_CLASS_PRESSURE,
                STATE_CLASS_MEASUREMENT,
            ).extend(
                {
                    cv.Optional(CONF_OVERSAMPLING, default="16X"): cv.enum(
                        OVERSAMPLING_OPTIONS, upper=True
                    ),
                }
            ),
            cv.Optional(CONF_HUMIDITY): sensor.sensor_schema(
                UNIT_PERCENT,
                ICON_EMPTY,
                1,
                DEVICE_CLASS_HUMIDITY,
                STATE_CLASS_MEASUREMENT,
            ).extend(
                {
                    cv.Optional(CONF_OVERSAMPLING, default="16X"): cv.enum(
                        OVERSAMPLING_OPTIONS, upper=True
                    ),
                }
            ),
            cv.Optional(CONF_IIR_FILTER, default="OFF"): cv.enum(
                IIR_FILTER_OPTIONS, upper=True
            ),
        }
    )
    .extend(cv.polling_component_schema("60s"))
    .extend(i2c.i2c_device_schema(0x77))
)


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await i2c.register_i2c_device(var, config)

    if CONF_TEMPERATURE in config:
        conf = config[CONF_TEMPERATURE]
        sens = await sensor.new_sensor(conf)
        cg.add(var.set_temperature_sensor(sens))
        cg.add(var.set_temperature_oversampling(conf[CONF_OVERSAMPLING]))

    if CONF_PRESSURE in config:
        conf = config[CONF_PRESSURE]
        sens = await sensor.new_sensor(conf)
        cg.add(var.set_pressure_sensor(sens))
        cg.add(var.set_pressure_oversampling(conf[CONF_OVERSAMPLING]))

    if CONF_HUMIDITY in config:
        conf = config[CONF_HUMIDITY]
        sens = await sensor.new_sensor(conf)
        cg.add(var.set_humidity_sensor(sens))
        cg.add(var.set_humidity_oversampling(conf[CONF_OVERSAMPLING]))

    cg.add(var.set_iir_filter(config[CONF_IIR_FILTER]))
    cg.add_library("SparkFun BME280", "2.0.8")