#include "dht.h"
#include "esphome/core/log.h"
#include "esphome/core/helpers.h"


/*
Using Platformio library: "diaoul/DHTNew @ 1.0.0"
*/
// Prevent conflict with reused class name
//namespace dhtnew {

//}

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

  bool success;
  /*
  if (this->model_ == DHT_MODEL_AUTO_DETECT) {
    this->model_ = DHT_MODEL_DHT22;
    success = this->read(false);
    if (!success) {
      this->model_ = DHT_MODEL_DHT11;
      return;
    }
  } else {
    success = this->read(false);
  }
  */
   success = this->read(false);

  if (success) {
    ESP_LOGD(TAG, "Got Temperature=%.1f°C Humidity=%.1f%%", _temperature, _humidity);

    if (this->temperature_sensor_ != nullptr)
      this->temperature_sensor_->publish_state(_temperature);
    if (this->humidity_sensor_ != nullptr)
      this->humidity_sensor_->publish_state(_humidity);
    this->status_clear_warning();
  } else {
    const char *str = "";
    if (this->is_auto_detect_) {
      str = " and consider manually specifying the DHT model using the model option";
    }
    ESP_LOGW(TAG, "Invalid readings! Please check your wiring (pull-up resistor, pin number)%s.", str);
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


// Returns true if a reading attempt was made (successful or not)
bool DHT::read(bool force) {
    
    // don't read more than every getMinimumSamplingPeriod() milliseconds
    unsigned long currentTime = millis();
    if (!force && ((currentTime - _lastReadTime) < getMinimumSamplingPeriod())) {
        return false;
    }

    // reset lastReadTime, temperature and humidity
    _lastReadTime = currentTime;
    _temperature = NAN;
    _humidity = NAN;

    _temperature = dhtnew.readTemperature();
   
    _humidity = dhtnew.readHumidity();
    

    return true;
}

}  // namespace dht
}  // namespace esphome
