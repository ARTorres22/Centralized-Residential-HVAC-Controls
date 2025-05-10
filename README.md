# Centralized Residential HVAC Controls

## Overview
Introducing our advanced zonal HVAC control system - designed to optimize comfort and energy efficiency in your home or small commercial space. With precise temperature and humidity monitoring across multiple zones, you can enjoy tailored climate control in each room, all managed through an intuitive central controller or remotely via the cloud. With our product, you can reduce energy waste, enhance comfort, and take control of your environment with smart, reliable technology.

## Contributors
- **Adrian Torres**
- **Nicholas Sakarellos**

## Initial Implementation
This code represents our initial design implementation, featuring a single sensor connected to the **Renesas RA0E1** board and a display screen. We utilized the **Renesas e2 studio** to code the software necessary for this design. 
Majority of the work is done in [`hal_entry.c`](initial%20test/src/hal_entry.c)

The display shows key data, including:
- Current temperature and humidity readings
- The fan's theoretical operational percentage
- The user-defined target temperature

![Initial test](/images/intial_test.png)

The fan speed is controlled using a basic **fan curve**, which adjusts the fan's operation based on the current temperature compared to the desired temperature. For now, the target temperature can be manually set by pressing the onboard button, which cycles the setpoint between **60°F and 90°F**. This allows the user to fine-tune the desired room temperature.

## Final Work
Moving forward, we enhanced this system by:
- Implemented **zoning capabilities**, where each zone will have its own dedicated fan speed control
- Had app zones connected and able to communicate with each other.
- Introduced **independent climate clusters** with:
  - **RA0E1 board** per zone
  - **Thermal and humidity sensors** per zone
  - **Heating pads** as heat sources
  - **Vent modules** for airflow control
- Centralizing all individual clusters into **one main panel**

  ![Final test](/images/final_test.png)

## Testing Setup
Our testing setup includes a model house, multiple rooms/zones, main control panel, and  one **individual cluster** per zone. 

During testing:
- We simulate an increase in room temperature by turning on the heat pads, which causes a rise in the temperature reading.
- The change is reflected **in real-time** on the OLED display, providing immediate feedback.
- We tested the **fan speed adjustment mechanism**, which dynamically changes based on the user-defined desired temperature.
- This ensures that the system responds appropriately to shifts in environmental conditions and maintains optimal cooling or heating performance.

