# ORYMY Bass - YMY Sounds VST

Procesador y amplificador de bajo eléctrico de alta gama optimizado para **Bachata** y **Merengue**[cite: 1]. Desarrollado con C++ y JUCE para Windows x64.

## Características Principales
* **Motor DSP Dedicado:** Control total de ganancia, amplificador estilo boutique, modelado de gabinete y saturación armónica cálida.
* **Presets Exclusivos:** Incluye ajustes inspirados en leyendas de la bachata (*Antony Santos*, *Raúlín Rodríguez*, *Max Santos*, etc.)[cite: 1].
* **Sección Gráfica y Analizador:** Ecualizador gráfico de 10 bandas con modos automáticos y visualización precisa en tiempo real[cite: 1].
* **Formatos Soportados:** VST3 y aplicación Standalone independientes.

## Compilación Local en Windows
1. Instalar CMake y Visual Studio 2022 con soporte para C++.
2. Clonar el repositorio y ejecutar:
   ```cmd
   cmake -B build -G "Visual Studio 17 2022" -A x64
   cmake --build build --config Release
