# CarroCompetenciaWRO

Este repositorio contiene todo el código, modelos 3D, documentación y resultados del proyecto del **Carro de Competencia WRO**, un robot móvil diseñado para competencias de robótica.

---

## 📂 Contenido del Repositorio

- **Impresiones/**  
  Archivos STL exportados desde Autodesk Inventor para impresión 3D de las piezas del robot.

- **Modelos/**  
  Archivos de diseño CAD realizados en **Autodesk Inventor** (`.ipt`, `.iam`, `.stp`, etc.).

- **Programación/CarroCompetenciaWRO/**  
  Código fuente en C/C++ para Arduino Nano, incluyendo el control de motores, sensores y lógica de navegación.

- **README.md**  
  Documento de presentación y guía de uso del proyecto.

---

## 🤖 Descripción del Proyecto

El **CarroCompetenciaWRO** es un prototipo robotizado construido para competencias (como la World Robot Olympiad), con las siguientes características destacadas:

- **Diseño 100% propio**, modelado en **Autodesk Inventor** aplicando principios de ingeniería.  
- **Fabricación por impresión 3D en PLA**, logrando piezas ligeras, resistentes y fáciles de modificar.  
- **Sistema de movilidad**: motorreductores controlados con **L298N** para las ruedas traseras.  
- **Dirección**: servos SG90 para las ruedas delanteras.  
- **Orientación y giros**: **MPU6050** utilizando el eje *yaw* para conteo de vueltas.  
- **Sensores**:  
  - **Ultrasónicos** (frontal y laterales) para evitar colisiones.  
  - **Sensor de color** para detección de obstáculos y zonas de decisión.  
  - **Sensores de línea** para conteo de vueltas o trayectorias.  
- **Control central**: **Arduino Nano**, montado en una placa de pruebas.  
- **Energía**: batería recargable como fuente de poder.  

---

## ⚙️ Justificación de Componentes

| Componente                  | Función                                             | Razón de elección                                                                          |
|-----------------------------|-----------------------------------------------------|--------------------------------------------------------------------------------------------|
| **Inventor + PLA**          | Diseño y fabricación de piezas mecánicas            | Permite diseño preciso, prototipado rápido y ajustes iterativos.                          |
| **Motorreductores + L298N** | Propulsión de ruedas traseras                       | Ofrece buen torque y control de dirección/velocidad con un solo driver.                   |
| **Servos SG90**             | Dirección de ruedas delanteras                      | Compactos, precisos, de bajo consumo y fáciles de usar.                                   |
| **MPU6050 (yaw)**           | Conteo de vueltas mediante medición de orientación   | Sensor económico y confiable para medir rotación en el eje yaw.                           |
| **Sensores ultrasónicos**   | Detección de obstáculos adelante y a los lados      | Económicos, con buen rango de detección para navegación segura.                           |
| **Sensor de color**         | Reconocimiento de obstáculos o zonas por color      | Permite decisiones basadas en señales visuales.                                           |
| **Sensores de línea**       | Conteo de vueltas o seguimiento de pista            | Brindan precisión en trayectorias repetitivas.                                            |
| **Placa de pruebas + Nano** | Integración y control                               | Plataforma accesible y ampliamente soportada para coordinar sensores y actuadores.        |
| **Batería**                 | Fuente de energía                                   | Alimenta todo el sistema de forma autónoma.                                               |

---

## 🚀 Instrucciones para Empezar

### 1. Clonar el repositorio  
```sh
git clone https://github.com/Estephan2001/CarroCompetenciaWRO.git
