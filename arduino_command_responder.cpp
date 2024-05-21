/* Copyright 2019 The TensorFlow Authors. All Rights Reserved.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
==============================================================================*/

#if defined(ARDUINO) && !defined(ARDUINO_ARDUINO_NANO33BLE)
#define ARDUINO_EXCLUDE_CODE
#endif  // defined(ARDUINO) && !defined(ARDUINO_ARDUINO_NANO33BLE)

#ifndef ARDUINO_EXCLUDE_CODE

#include "command_responder.h"
#include "Arduino.h"

// Respond to commands: Toggle LEDs based on detected keywords.
void RespondToCommand(tflite::ErrorReporter* error_reporter,
                      int32_t current_time, const char* found_command,
                      uint8_t score, bool is_new_command) {
    static bool is_initialized = false;
    if (!is_initialized) {
        pinMode(LED_BUILTIN, OUTPUT);
        pinMode(LEDR, OUTPUT);
        pinMode(LEDG, OUTPUT);
        pinMode(LEDB, OUTPUT);
        digitalWrite(LEDR, HIGH);  // RGB LEDs are active low
        digitalWrite(LEDG, HIGH);
        digitalWrite(LEDB, HIGH);
        is_initialized = true;
    }

    static int32_t last_command_time = 0;
    static int count = 0;

    if (is_new_command) {
        TF_LITE_REPORT_ERROR(error_reporter, "Heard %s (%d) @%dms", found_command,
                             score, current_time);

        // Responding to specific commands by lighting up different LEDs
        if (strcmp(found_command, "banana") == 0) {
            last_command_time = current_time;
            digitalWrite(LEDG, LOW);  // Green for "banana"
        } else if (strcmp(found_command, "brunch") == 0) {
            last_command_time = current_time;
            digitalWrite(LEDR, LOW);  // Red for "brunch"
        } else if (strcmp(found_command, "unknown") == 0) {
            last_command_time = current_time;
            digitalWrite(LEDB, LOW);  // Blue for "unknown"
        }
    }

    // Turn off LEDs if the last command was more than 3 seconds ago
    if (last_command_time != 0 && current_time > (last_command_time + 3000)) {
        digitalWrite(LED_BUILTIN, LOW);
        digitalWrite(LEDR, HIGH);
        digitalWrite(LEDG, HIGH);
        digitalWrite(LEDB, HIGH);
        last_command_time = 0;  // Reset last command time
    }

    // Toggle the built-in LED on every inference
    ++count;
    digitalWrite(LED_BUILTIN, (count & 1) ? HIGH : LOW);
 }


#endif  // ARDUINO_EXCLUDE_CODE
