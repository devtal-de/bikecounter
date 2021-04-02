const char* ssid     = "";
const char* password = "";
const char* host = "";
const char* url = "/index.php?";
const uint16_t port = 80;
const int sensorID = 1; // ID des Bikecounters
const int sensorDistance = 200; // Abstand der Sensoren zueinander in cm
const int minSpeed = 1; // Prüfung auf Plausibilität: außerhalb dieser Grenzen wird die Messung verworfen
const int maxSpeed = 35; // Angabe in km/h
const int downtime = 200; // Totzeit (ms) bis zur nächsten Messung
const int timeout = 1000; // Falls innerhalb der Timeout-Zeit (ms) nur ein Sensor ausgelöst wurde, wird die Messung verworfen.
#define gpioSensor1 4 // GPIO Sensor 1
#define gpioSensor2 5 // GPIO Sensor 2
