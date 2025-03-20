/*
 * Project myProject
 * Author: Your Name
 * Date:
 * For comprehensive documentation and examples, please visit:
 * https://docs.particle.io/firmware/best-practices/firmware-template/
 */

// Include Particle Device OS APIs
#include "Particle.h"

// Let Device OS manage the connection to the Particle Cloud
SYSTEM_MODE(AUTOMATIC);

SerialLogHandler logHandler(LOG_LEVEL_INFO);
Ledger statusLedger;
String input = "";
Variant data;

bool newData = false;

void setup()
{
  Serial1.begin(115200);
  statusLedger = Particle.ledger("b5som-d2c");
}

void loop()
{
  while (Serial1.available())
  {
    // {"lights": 50, "window": 20, "temperature": 30, "fan": 0}
    String incoming = Serial1.readString();
    Log.info("Received: %s", incoming.c_str());
    JSONValue json = JSONValue::parseCopy(incoming.c_str());
    if (json.isValid())
    {
      JSONObjectIterator iter(json);
      while (iter.next())
      {
        if (iter.name() == "temperature")
        {
          Log.info("Temperature: %s", (const char *)iter.value().toString());
          data.set("temperature", iter.value().toDouble());
        }
        else if (iter.name() == "lights")
        {
          Log.info("Lights: %s", (const char *)iter.value().toString());
          data.set("lights", iter.value().toDouble());
        }
        else if (iter.name() == "window")
        {
          Log.info("Window: %s", (const char *)iter.value().toString());
          data.set("window", (const char *)iter.value().toString());
        }
        else if (iter.name() == "fan")
        {
          Log.info("Fan: %s", (const char *)iter.value().toString());
          data.set("fan", (const char *)iter.value().toString());
        }
      }
      newData = true;
    }
  }
  if (newData)
  {
    statusLedger.set(data, particle::Ledger::REPLACE);
    Log.info("Updating Ledger: %s", data.toJSON().c_str());
    newData = false;
  }
}
