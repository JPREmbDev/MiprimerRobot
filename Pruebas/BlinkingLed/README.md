
# Blinkin LED

    En este caso configuraremos un proyecto para hacer parpadear un LED en la placa.


## TECNOLOGIAS USADAS

**SOFTWARE**

    - VSCODE
    - PLATFOMRIO
    - C/C++
    - GIT
    - CMAKE
    

**HARDWARE:**

    - ESP32
    - LED integrado en la placa GPIO_2



## Tutorial Referencia


[Tutorial Referencia](https://docs.espressif.com/projects/arduino-esp32/en/latest/tutorials/blink.html)


# Resistencias Pull-up y Pull-down

Las resistencias **pull-up** y **pull-down** son esenciales en circuitos digitales para definir el estado lógico de las entradas. Aquí aprenderás en qué consisten, cómo funcionan, cuándo usarlas, y algunos ejemplos prácticos.

## 📋 Tabla de Contenidos
1. [Resistencias Pull-up](#resistencias-pull-up)
2. [Resistencias Pull-down](#resistencias-pull-down)
3. [Diferencias Clave](#diferencias-clave-entre-pull-up-y-pull-down)
4. [¿Dónde Usarlas?](#dónde-usar-resistencias-pull-up-y-pull-down)
5. [Resumen Visual](#resumen-visual)

---

## Resistencias Pull-up

Una resistencia **pull-up** se conecta entre la entrada de un dispositivo (como un microcontrolador) y el voltaje positivo de alimentación (**Vcc**). Garantiza que la entrada tenga un valor lógico "1" (alto) cuando no hay ninguna otra señal aplicada.

### 🔧 Funcionamiento:
- **Estado sin señal:** Mantiene la entrada en estado lógico alto (`1`).
- **Estado con señal:** Permite bajar a un estado lógico bajo (`0`) cuando la entrada se conecta a **GND**.

### 💡 Diagrama de Conexión:
```plaintext
Vcc (+5V)
   |
   |
  R (Pull-up)
   |
   +----> Entrada del microcontrolador
   |
  ----
   |
  GND
```

## Resistencias Pull-down

Una resistencia **pull-down** se conecta entre la entrada de un dispositivo y tierra (**GND**). Asegura que la entrada tenga un valor lógico "0" (bajo) cuando no hay ninguna otra señal aplicada.

### 🔧 Funcionamiento:
- **Estado sin señal:** Mantiene la entrada en estado lógico bajo (`0`).
- **Estado con señal:** Permite elevar a un estado lógico alto (`1`) cuando la entrada se conecta a Vcc.

### 💡 Diagrama de Conexión:
```plaintext
  Vcc (+5V)
   |
   +----> Entrada del microcontrolador
   |
  R (Pull-down)
   |
  ----
   |
  GND
```

## Diferencias Clave entre Pull-up y Pull-down

| Características | Pull-up     | Pull-down                |
| :-------- | :------- | :------------------------- |
| `Estado por defecto	` | `Alto` (`1`) | `Bajo` (`0`)  |
| `Conexión de la resistencia	` | `Entre la entrada y Vcc` | `Entre la entrada y GND`  |


## ¿Dónde Usar Resistencias Pull-up y Pull-down?

### Usos de Resistencias Pull-up:

```
    - Botones de entrada: Mantiene la entrada en estado alto cuando no está presionado.
    - Comunicación I2C: Las líneas SDA y SCL usan resistencias pull-up.
    - Entradas flotantes: Evita comportamientos erráticos en entradas sin conectar.
    - Entradas de interrupción: Asegura un estado alto cuando no hay interrupciones.
    - Pines de reset: Mantiene el pin de reset en un estado alto hasta que se necesite un reset.
```
### Usos de Resistencias Pull-down:

```
    - Botones de entrada: Mantiene la entrada en estado bajo cuando no está presionado.
    - Entradas de señales analógicas: Asegura un valor inicial bajo cuando no hay señal.
    - Entradas CMOS: Mantiene las entradas en estado bajo y reduce el consumo.
    - Pines de control en FETs: Asegura que el FET esté apagado cuando no hay señal.
    - Entradas de temporizadores: Mantiene la entrada en estado bajo hasta iniciar una señal.
```

## Resumen Visual

### Pull-up Resistor:

- Conexión entre **Vcc** y la entrada.
- Entrada predeterminada en estado lógico alto (`1`).

### Pull-down Resistor:

- Conexión entre **GND** y la entrada.
- Entrada predeterminada en estado lógico bajo (`0`).

---

## 🛠 Recursos Adicionales

- [Tutorial sobre resistencias](https://www.electronics-tutorials.ws/resistor/resistor_4.html)
- [Documentación oficial de Arduino](https://www.arduino.cc/en/Tutorial/BuiltInExamples/Button)
