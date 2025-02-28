# Centralized Residential HVAC Controls

## Overview
Introducing our advanced zonal HVAC control system - designed to optimize comfort and energy efficiency in your home or small commercial space. With precise temperature and humidity monitoring across multiple zones, you can enjoy tailored climate control in each room, all managed through an intuitive central controller or remotely via the cloud. With our product, you can reduce energy waste, enhance comfort, and take control of your environment with smart, reliable technology.

## Contributors
- **Adrian Torres**
- **Nicholas Sakarellos**

## Initial Implementation
This code represents our initial design implementation, featuring a single sensor connected to the **Renesas RA0E1** board and a display screen. We utilized the **Renesas e2 studio** to code the software necessary for this design. 

The display shows key data, including:
- Current temperature and humidity readings
- The fan's theoretical operational percentage
- The user-defined target temperature

![Initial test](/images/intial_test.png)

The fan speed is controlled using a basic **fan curve**, which adjusts the fan's operation based on the current temperature compared to the desired temperature. For now, the target temperature can be manually set by pressing the onboard button, which cycles the setpoint between **60°F and 90°F**. This allows the user to fine-tune the desired room temperature.

## Future Enhancements
Moving forward, we plan to enhance this system by:
- Adding **more sensors** for improved accuracy
- Implementing **zoning capabilities**, where each zone will have its own dedicated fan speed control
- Introducing **independent climate clusters** with:
  - **Thermal and humidity sensors** per zone
  - **Heating pads** as heat sources
  - **Vent modules** for airflow control
- Centralizing all individual clusters into **one main panel**
- Exploring the possibility of integrating with a **mobile app/controller** for remote access

## Testing Setup
Our testing setup includes an **individual cluster** consisting of an **OLED display** and a **temperature/humidity sensor**. 

During testing:
- We simulate an increase in room temperature by placing a finger over the sensor, which causes a rise in the temperature reading.
- The change is reflected **in real-time** on the OLED display, providing immediate feedback.
- We tested the **fan speed adjustment mechanism**, which dynamically changes based on the user-defined desired temperature.
- This ensures that the system responds appropriately to shifts in environmental conditions and maintains optimal cooling or heating performance.

---

This README serves as a foundation for our ongoing development. As we refine and expand our HVAC control system, we will update this documentation to reflect new features and improvements.
