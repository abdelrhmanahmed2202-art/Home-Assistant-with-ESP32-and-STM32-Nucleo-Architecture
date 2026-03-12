esphome:
  name: farm-robot
  friendly_name: farm-robot

esp32:
  board: esp32dev
  framework:
    type: esp-idf

# Enable logging (Baud 0 allows using USB for power without interfering with STM32)
logger:
  baud_rate: 0

# Enable Home Assistant API
api:
  encryption:
    key: !secret api_encryption_key

ota:
  - platform: esphome
    password: !secret ota_password

wifi:
  ssid: !secret wifi_ssid
  password: !secret wifi_password

  # Fallback Hotspot
  ap:
    ssid: "Farm-Robot Fallback Hotspot"
    password: !secret fallback_ap_password

captive_portal:

# ==========================================
# 1. UART CONNECTION TO STM32
# ==========================================
uart:
  id: stm_uart
  tx_pin: 17
  rx_pin: 16
  baud_rate: 9600

# ==========================================
# 2. USER CONTROLS (HUGE SCALE)
# ==========================================
number:
  # Manual Control Sliders (Updated for large grid)
  - platform: template
    name: "Target X (Map)"
    id: target_x
    min_value: 0
    max_value: 70000
    step: 1000
    optimistic: true

  - platform: template
    name: "Target Y (Map)"
    id: target_y
    min_value: 0
    max_value: 70000
    step: 1000
    optimistic: true
    
  - platform: template
    name: "Target Z (Map)"
    id: target_z
    min_value: 0
    max_value: 55000
    step: 1000
    optimistic: true

# The "Preset" Dropdown
select:
  - platform: template
    name: "Select Plant Position"
    id: plant_pos
    options:
      - "1"
      - "2"
      - "3"
      - "4"
      - "5 (Center)"
      - "6"
      - "7"
      - "8"
      - "9"
    initial_option: "5 (Center)"
    optimistic: true

# The Logic Button (Triggers the script)
button:
  - platform: template
    name: "START PLANTING SEQUENCE"
    id: farm_robot_start_planting_sequence
    on_press:
      - script.execute: run_planting_cycle

# ==========================================
# 3. THE BRAIN (Corrected Logic & Timing)
# ==========================================
script:
  - id: run_planting_cycle
    then:
      # --- PHASE 1: Move from Rest to Target ---
      - lambda: |-
          int x = 32000; int y = 32000; // Default Center
          std::string s = id(plant_pos).state;

          // GRID LOGIC: 30k Spacing
          // Row 1 (Y=2000)
          if (s == "1") { x=2000;  y=2000; }
          if (s == "2") { x=32000; y=2000; }
          if (s == "3") { x=62000; y=2000; }
          
          // Row 2 (Y=32000)
          if (s == "4") { x=2000;  y=32000; }
          if (s == "5 (Center)") { x=32000; y=32000; }
          if (s == "6") { x=62000; y=32000; }
          
          // Row 3 (Y=62000)
          if (s == "7") { x=2000;  y=62000; }
          if (s == "8") { x=32000; y=62000; }
          if (s == "9") { x=62000; y=62000; }

          // Send Command: Move X/Y, Keep Z at 50000 (HIGH/REST)
          char cmd[50];
          sprintf(cmd, "X%dY%dZ50000\n", x, y);
          id(stm_uart).write_str(cmd);
          
          // Update Map
          id(target_x).publish_state(x);
          id(target_y).publish_state(y);
          ESP_LOGD("custom", "Phase 1: Moving to Target %s", s.c_str());

      # SAFETY DELAY: Wait 35s for the robot to drive to position
      - delay: 12s

      # --- PHASE 2: Drill Down (Z -> 0) ---
      - lambda: |-
          int x = id(target_x).state;
          int y = id(target_y).state;
          
          char cmd[50];
          sprintf(cmd, "X%dY%dZ0\n", x, y); // Go to 0 (Ground)
          id(stm_uart).write_str(cmd);
          
          id(target_z).publish_state(0);
          ESP_LOGD("custom", "Phase 2: Drilling Down");

      # SAFETY DELAY: Wait 30s for the drill to go all the way down
      - delay: 12s

      # --- PHASE 3: Retract Up (Z -> 50000) ---
      - lambda: |-
          int x = id(target_x).state;
          int y = id(target_y).state;
          
          char cmd[50];
          sprintf(cmd, "X%dY%dZ50000\n", x, y); // Go back to 50k (Rest)
          id(stm_uart).write_str(cmd);
          
          id(target_z).publish_state(50000);
          ESP_LOGD("custom", "Phase 3: Retracting");

      # SAFETY DELAY: Wait 30s for the drill to come back up!
      # CRITICAL: Do not move X/Y until this finishes.
      - delay: 12s

      # --- PHASE 4: Return Home (Center) ---
      - lambda: |-
          id(stm_uart).write_str("X32000Y32000Z50000\n");
          
          // Update map to show we are back home
          id(target_x).publish_state(32000);
          id(target_y).publish_state(32000);
          ESP_LOGD("custom", "Phase 4: Returning Home");