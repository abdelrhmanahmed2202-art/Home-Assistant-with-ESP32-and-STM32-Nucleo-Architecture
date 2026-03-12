// note that the code is not written in C, its just a file for storing the code
type: picture-elements
image: /local/dirt_bed.jpg
elements:
  - type: state-icon
    entity: number.target_x_map
    icon: mdi:robot-industrial
    style:
      left: ${ ( states('number.target_x_map') | float / 70000 ) * 100 }%
      top: ${ ( states('number.target_y_map') | float / 70000 ) * 100 }%
      transform: translate(-50%, -50%)
      color: white
      background-color: rgba(0,0,0,0.5)
      border-radius: 50%
      pointer-events: none
  - type: image
    entity: input_select.plant_slot_1
    tap_action:
      action: more-info
    state_image:
      Empty: /local/icons/Empty_clean.png
      Tomato: /local/icons/tomato_clean.png
      Spinach: /local/icons/spinach_clean.png
      Olive: /local/icons/olives_clean.png
    style:
      top: 18%
      left: 29%
      width: 40px
      transform: translate(-50%, -50%)
      background-color: transparent
  - type: image
    entity: input_select.plant_slot_2
    tap_action:
      action: more-info
    state_image:
      Empty: /local/icons/Empty_clean.png
      Tomato: /local/icons/tomato_clean.png
      Spinach: /local/icons/spinach_clean.png
      Olive: /local/icons/olives_clean.png
    style:
      top: 18%
      left: 50%
      width: 40px
      transform: translate(-50%, -50%)
      background-color: transparent
  - type: image
    entity: input_select.plant_slot_3
    tap_action:
      action: more-info
    state_image:
      Empty: /local/icons/Empty_clean.png
      Tomato: /local/icons/tomato_clean.png
      Spinach: /local/icons/spinach_clean.png
      Olive: /local/icons/olives_clean.png
    style:
      top: 18%
      left: 72%
      width: 40px
      transform: translate(-50%, -50%)
      background-color: transparent
  - type: image
    entity: input_select.plant_slot_4
    tap_action:
      action: more-info
    state_image:
      Empty: /local/icons/Empty_clean.png
      Tomato: /local/icons/tomato_clean.png
      Spinach: /local/icons/spinach_clean.png
      Olive: /local/icons/olives_clean.png
    style:
      top: 50%
      left: 29%
      width: 40px
      transform: translate(-50%, -50%)
      background-color: transparent
  - type: image
    entity: input_select.plant_slot_5
    tap_action:
      action: more-info
    state_image:
      Empty: /local/icons/Empty_clean.png
      Tomato: /local/icons/tomato_clean.png
      Spinach: /local/icons/spinach_clean.png
      Olive: /local/icons/olives_clean.png
    style:
      top: 50%
      left: 50%
      width: 40px
      transform: translate(-50%, -50%)
      background-color: transparent
  - type: image
    entity: input_select.plant_slot_6
    tap_action:
      action: more-info
    state_image:
      Empty: /local/icons/Empty_clean.png
      Tomato: /local/icons/tomato_clean.png
      Spinach: /local/icons/spinach_clean.png
      Olive: /local/icons/olives_clean.png
    style:
      top: 50%
      left: 72%
      width: 40px
      transform: translate(-50%, -50%)
      background-color: transparent
  - type: image
    entity: input_select.plant_slot_7
    tap_action:
      action: more-info
    state_image:
      Empty: /local/icons/Empty_clean.png
      Tomato: /local/icons/tomato_clean.png
      Spinach: /local/icons/spinach_clean.png
      Olive: /local/icons/olives_clean.png
    style:
      top: 81%
      left: 29%
      width: 40px
      transform: translate(-50%, -50%)
      background-color: transparent
  - type: image
    entity: input_select.plant_slot_8
    tap_action:
      action: more-info
    state_image:
      Empty: /local/icons/Empty_clean.png
      Tomato: /local/icons/tomato_clean.png
      Spinach: /local/icons/spinach_clean.png
      Olive: /local/icons/olives_clean.png
    style:
      top: 80%
      left: 50%
      width: 40px
      transform: translate(-50%, -50%)
      background-color: transparent
  - type: image
    entity: input_select.plant_slot_9
    tap_action:
      action: more-info
    state_image:
      Empty: /local/icons/Empty_clean.png
      Tomato: /local/icons/tomato_clean.png
      Spinach: /local/icons/spinach_clean.png
      Olive: /local/icons/olives_clean.png
    style:
      top: 80%
      left: 72%
      width: 40px
      transform: translate(-50%, -50%)
      background-color: transparent
