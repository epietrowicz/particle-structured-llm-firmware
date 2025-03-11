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
    // {"temperature": 22.5, "humidity": 60, "status": "ok"}
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
        else if (iter.name() == "humidity")
        {
          Log.info("Humidity: %s", (const char *)iter.value().toString());
          data.set("humidity", iter.value().toDouble());
        }
        else if (iter.name() == "status")
        {
          Log.info("Status: %s", (const char *)iter.value().toString());
          data.set("status", (const char *)iter.value().toString());
        }
      }
      newData = true;
    }
  }
  if (newData)
  {
    statusLedger.set(data, particle::Ledger::MERGE);
    Log.info("Updating Ledger: %s", data.toJSON().c_str());
    newData = false;
  }
}
