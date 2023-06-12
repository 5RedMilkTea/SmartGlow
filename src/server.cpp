#include "server.h"
// Number of milliseconds to wait without receiving any data before we give up
const int kNetworkTimeout = 30 * 1000;

// Number of milliseconds to wait if no data is available before trying again
const int kNetworkDelay = 1000;

const char* SERVER_IP = "54.183.146.244";

void readESP(int light_val)
{
    int err = 0;
    WiFiClient c;
    HttpClient http(c);

    String get_string = String("/?var=") + String(light_val); 
    //err = http.get(kHostname, kPath);
    char final_str[get_string.length() + 1];
    strcpy(final_str, get_string.c_str());

    // Sends string to URL as designated IP server
    err = http.get(SERVER_IP, 5000, final_str, NULL);

    //delay(2000);
    if (err == 0) {
        Serial.println("startedRequest ok");
        err = http.responseStatusCode();
        if (err >= 0) {
        Serial.print("Got status code: ");
        Serial.println(err);
        // Usually you'd check that the response code is 200 or a
        // similar "success" code (200-299) before carrying on,
        // but we'll print out whatever response we get

        err = http.skipResponseHeaders();
        if (err >= 0) {
            int bodyLen = http.contentLength();
            Serial.print("Content length is: ");
            Serial.println(bodyLen);
            Serial.println();
            Serial.println("Body returned follows:");
            // Now we've got to the body, so we can print it out
            unsigned long timeoutStart = millis();
            char c;
            // Whilst we haven't timed out & haven't reached the end of the body
            while ((http.connected() || http.available()) &&
                ((millis() - timeoutStart) < kNetworkTimeout)) {
            if (http.available()) {
                c = http.read();
                // Print out this character
                Serial.print(c);
                bodyLen--;
                // We read something, reset the timeout counter
                timeoutStart = millis();
            } else {
                // We haven't got any data, so let's pause to allow some to
                // arrive
                delay(kNetworkDelay);
            }
            }
        } else {
            Serial.print("Failed to skip response headers: ");
            Serial.println(err);
        }
        } else {
        Serial.print("Getting response failed: ");
        Serial.println(err);
        }
    } else {
        Serial.print("Connect failed: ");
        Serial.println(err);
    }
    http.stop();
    // And just stop, now that we've tried a download
    
}
