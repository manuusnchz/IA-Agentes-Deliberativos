# 🧠 AI Autonomous Agents & Pathfinding Engine

![C++](https://img.shields.io/badge/C++-17+-00599C?style=for-the-badge&logo=c%2B%2B&logoColor=white)
![AI](https://img.shields.io/badge/Artificial%20Intelligence-Agents-FF6F00?style=for-the-badge)
![Algorithms](https://img.shields.io/badge/Algorithms-BFS%20%2F%20Heuristics-blue?style=for-the-badge)

> **Simulación de agentes inteligentes reactivos y deliberativos capaces de navegar, explorar y resolver objetivos en entornos desconocidos 2D.**

## 📋 Descripción del Proyecto

Este proyecto implementa la lógica de toma de decisiones para agentes autónomos en un entorno simulado de cuadrícula (Grid World). El sistema gestiona dos tipos de agentes ("Rescatador" y "Auxiliar") que deben colaborar o actuar independientemente para explorar el mapa y alcanzar objetivos bajo restricciones de terreno y energía.

El núcleo del proyecto se centra en la **transición de comportamientos reactivos a deliberativos**, implementando algoritmos de búsqueda clásica para la planificación de trayectorias óptimas.

## ⚙️ Características Técnicas (Technical Core)

### 1. Arquitecturas de Agentes 🤖
Implementación de lógica de control híbrida:
* **Agentes Reactivos (Niveles 0-1):** Toma de decisiones basada en percepción local inmediata (sensores de superficie y altitud). Implementación de máquinas de estados finitos para navegación básica y evasión de obstáculos.
* **Agentes Deliberativos (Niveles 2+):** Capacidad de planificación a largo plazo. El agente construye una representación interna del mundo ("mapa mental") y genera planes de acción completos antes de ejecutarlos.

### 2. Motor de Búsqueda y Planificación (Pathfinding) 🗺️
Desarrollo desde cero de algoritmos de exploración de espacios de estados:
* **Breadth-First Search (BFS):** Implementación de búsqueda en anchura para garantizar la solución óptima en número de pasos.
* **Gestión de Frontera y Explorados:** Uso eficiente de contenedores STL (`std::list` para la frontera FIFO, `std::set` con comparadores personalizados para el conjunto de explorados) para optimizar la complejidad temporal y evitar ciclos.
* **Espacio de Estados Complejo:** El estado del agente incluye coordenadas (x, y), orientación (brújula) e inventario (ej. "Zapatillas" para cruzar bosques), lo que aumenta exponencialmente la complejidad del grafo de búsqueda.

### 3. Percepción y Mapeado 📡
* **Fusión de Sensores:** Integración de datos de visión, orientación y topografía para construir una matriz de conocimiento del entorno (`mapaResultado`).
* **Heurísticas de Navegación:** Lógica para evaluar la "viabilidad" de casillas basada en desniveles de altura y posesión de objetos clave.

## 🛠️ Stack Tecnológico

* **Lenguaje:** C++ (Uso intensivo de Punteros, Referencias y STL).
* **Estructuras de Datos:** Grafos implícitos, Colas de prioridad, Conjuntos.
* **Conceptos:** Inteligencia Artificial Simbólica, Planificación Automática.

## 🧩 Retos Resueltos

* **Optimización de Memoria:** Transición de búsqueda lineal en listas (`O(N)`) a búsqueda logarítmica en árboles rojo-negro (`std::set` / `O(log N)`) para la gestión de estados visitados, reduciendo drásticamente el tiempo de cómputo en mapas grandes.
* **Interacción Multi-Agente:** Resolución de conflictos y bloqueos entre el agente Rescatador y el Auxiliar en entornos estrechos.
* **Restricciones Físicas:** Gestión de costes de movimiento según el terreno (Agua, Bosque, Precipicios) y herramientas disponibles.

---
<div align="center">
  <sub>Desarrollado por Manuel Peñas Sánchez | Universidad de Granada (UGR)</sub>
</div>
