#pragma once

#include "esphome/core/component.h"
// #include "esphome/core/esphal.h"  get_pin found here
#include "esphome/components/sensor/sensor.h"
#include "DHTNew.h"

namespace esphome {
namespace dht {

/// Component for reading temperature/humidity measurements from DHT11/DHT22 sensors.
class DHT : public PollingComponent {
 public:
  //void set_pin(GPIOPin *pin) { dhtPin_ = pin->get_pin(); }
  //void set_dht_model(DHTModel_t model);
  //void set_model(DHTModel_t model) { model_ = model; }
  void set_temperature_sensor(sensor::Sensor *temperature_sensor) { temperature_sensor_ = temperature_sensor; }
  void set_humidity_sensor(sensor::Sensor *humidity_sensor) { humidity_sensor_ = humidity_sensor; }

  /// Set up the pins and check connection.
  void setup() override;
  void dump_config() override;
  /// Update sensor values and push them to the frontend.
  void update() override;
  /// HARDWARE_LATE setup priority.
  float get_setup_priority() const override;
  int getMinimumSamplingPeriod() {return _model == DHT_MODEL_DHT11 ? 1000 : 2000;}

 protected:
  //Read sensor and return error message;
  const char* readSensor();

  //uint8_t dhtPin_;
  
  //DHTModel_t model_{DHT_MODEL_DHT11};
  //DHTModel_t model_{};
  //bool is_auto_detect_{false};
  sensor::Sensor *temperature_sensor_{nullptr};
  sensor::Sensor *humidity_sensor_{nullptr};

  //Object from DHTNew library
  //const uint8_t tmpPin = dhtPin_;  // Attempted conversion to const uint8_t for dhtnew
  // This creates the dhtnew object when the dht class is constructed so pin and model
  // is not available until set_pin and set_dht_model methods are called.
  //::DHT dhtnew{2, DHT_MODEL_DHT22};

 
  float _temperature;
  float _humidity;

 private:
  DHTModel_t _model; // from DHTNEw
  DHTError_t _error; // from DHTNEw
  unsigned long _lastReadTime;
};

}  // namespace dht
}  // namespace esphome
