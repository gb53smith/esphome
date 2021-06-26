#include "dht.h"
#include "esphome/core/log.h"
#include "esphome/core/helpers.h"


/*
Using Platformio library: "diaoul/DHTNew @ 1.0.0"
*/

const uint8_t dhtPin = 2;
DHTModel_t myDHTModel = DHT_MODEL_DHT22;
//const uint8_t dhtPin = this->pin_->get_pin();  #not working at top
DHT dhtnew (dhtPin, myDHTModel);


//DHT dhtnew (dhtPin, myDHTModel);

namespace esphome {
namespace dht {

static const char *const TAG = "dht";

//const uint8_t dhtPin = 2;
//DHTModel_t DHTModel = DHT_MODEL_DHT22;
//DHT dht_dht(dhtPin, DHTModel);
 //const uint8_t dhtPin = 2;
 //const uint8_t dhtPin = this->pin_->get_pin();
 //::DHTModel_t myDHTModel = DHT_MODEL_DHT22;
 //::DHT dhtnew(dhtPin, myDHTModel);

void DHT::setup() {
  ESP_LOGCONFIG(TAG, "Setting up DHT...");
  this->pin_->digital_write(true);
  this->pin_->setup();
  this->pin_->digital_write(true);
  // works but outside of thsi function
  //const uint8_t dhtPin = this->pin_->get_pin();
  //::DHTModel_t myDHTModel = DHT_MODEL_DHT22;
  //::DHT dhtnew(dhtPin, myDHTModel);
}

void DHT::dump_config() {
  ESP_LOGCONFIG(TAG, "DHT:");
  LOG_PIN("  Pin: ", this->pin_);
  if (this->model_ == DHT_MODEL_DHT11) {
    ESP_LOGCONFIG(TAG, "  Model: DHT11");
  } else {
    ESP_LOGCONFIG(TAG, "  Model: DHT22 (or equivalent)");
  }

  LOG_UPDATE_INTERVAL(this);

  LOG_SENSOR("  ", "Temperature", this->temperature_sensor_);
  LOG_SENSOR("  ", "Humidity", this->humidity_sensor_);
}

void DHT::update() {

  const char *errorString = this->readSensor();
  const char *success = "none";

  if (strcmp(errorString, success) == 0) {
    ESP_LOGD(TAG, "Got Temperature=%.1f°C Humidity=%.1f%%", _temperature, _humidity);
    if (this->temperature_sensor_ != nullptr)
      this->temperature_sensor_->publish_state(_temperature);
    if (this->humidity_sensor_ != nullptr)
      this->humidity_sensor_->publish_state(_humidity);
    this->status_clear_warning();
  } else {
      ESP_LOGD(TAG, "Got this DHT error: %s.", errorString);
    if (this->temperature_sensor_ != nullptr)
      this->temperature_sensor_->publish_state(NAN);
    if (this->humidity_sensor_ != nullptr)
      this->humidity_sensor_->publish_state(NAN);
    this->status_set_warning();
  }
}

float DHT::get_setup_priority() const { return setup_priority::DATA; }
 void DHT::set_dht_model(DHTModel_t model) {
  this->model_ = model;
  //this->is_auto_detect_ = model == DHT_MODEL_AUTO_DETECT;
} 


//Read sensor and return error message;
const char* DHT::readSensor() {
    
    // don't read more than every getMinimumSamplingPeriod() milliseconds
    // The DHTNew library just blocks the read without a error message type.
    unsigned long currentTime = millis();
    if ((currentTime - _lastReadTime) < getMinimumSamplingPeriod()) {
        return "Increase_sample_period";
    }

    // reset lastReadTime, temperature and humidity
    _lastReadTime = currentTime;
    _temperature = NAN;
    _humidity = NAN;

    _temperature = dhtnew.readTemperature();
   
    _humidity = dhtnew.readHumidity();
    
    const char* errorString = dhtnew.getErrorString();
    
    return errorString;
}

}  // namespace dht
}  // namespace esphome
