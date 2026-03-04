
### Proyecto corto 1

</h4> <hr style="border: 1px solid #000;"/>



### Descripción del proyecto
En este proyecto se diseñaron dos aceleradores para la función **softmax** de 100 entradas usando Vitis HLS. El objetivo es explorar la implementación en FPGA de cálculos de punto flotante, comparando un diseño simple sin optimizaciones con uno optimizado usando directivas HLS.  

Los aceleradores cumplen con los siguientes requisitos comunes:  
- Uso de unidades de punto flotante.  
- Síntesis para la AMD Kria KV260 a 250 MHz.  
- Entrada y salida: arreglos de 100 elementos.  
- Puertos AXI-Memory Map para entradas y salidas.  

</h4> <hr style="border: 1px solid #000;"/>

### Explicación de los diseños y las instrucciones de contrucción: 


### Diseño 1 : codificación sin ninguna directiva (todo por default excepto los puertos)  
Este diseño fue implementado únicamente con los pragmas necesarios para definir los puertos AXI. No se aplicaron directivas de optimización adicionales (pipeline, unroll, etc.).

Se espera que mantenga una implementación secuencial y sin paralelización explícita, utilizando el scheduler por defecto de HLS. Este enfoque puede derivar en mayor latencia en comparación con diseños optimizados, aunque minimiza la complejidad y mantiene un uso de recursos relativamente bajo.


### Diseño 2 : Codificación con optimizaciones HLS
En este diseño se aplicaron directivas de optimización HLS, tales como:  `#pragma HLS PIPELINE`  y  `#pragma HLS UNROLL`

Con el objetivo de reducir la latencia total y mejorar el throughput, procurando acercarse a la frecuencia objetivo de 250 MHz. Además, se analizará cuidadosamente el trade-off entre área y desempeño, evaluando cómo estas optimizaciones impactan el uso de recursos frente al rendimiento.

</h4> <hr style="border: 1px solid #000;"/> 

### Diseño 3 : Codificación con optimizaciones HLS 2
En este diseño se aplicaron directivas de optimización HLS, tales como: `#pragma HLS PIPELINE`, `#pragma HLS UNROLL` y `#pragma HLS ARRAY PARTITION`.

Con el objetivo de reducir la latencia total y mejorar, de igual manera que en el diseño 2, el throughput. En este caso en particular, la directiva "ARRAY PARTITION" permite tomar el arreglo de datos y particionarlo, de modo que se puedan establecer paralelismo a nivel de datos.

</h4> <hr style="border: 1px solid #000;"/> 

### Testbench para ambos diseños

Se desarrolló un testbench simple que:

1. Inicializa un arreglo de 100 valores flotantes.
2. Ejecuta la función softmax.
3. Verifica que:
   - Todos los valores sean positivos.
   - La suma de las salidas sea aproximadamente 1.
4. Permite validar funcionalidad antes de la síntesis RTL

Y finalmente para poder correr cada uno de los diseños, se procede a ejecutar el comando:
```
   vitis_hls -f script.tcl
```
Esto realiza todo lo definido en el script.tcl:
   - Crea el proyecto HLS.
   - Agrega archivos fuente y testbench.
   - Crea la solución para AMD Kria KV260 a 250 MHz.
   - Genera simulación y síntesis RTL, incluyendo reportes de timing y recursos.

   
</h4> <hr style="border: 1px solid #000;"/>



## Analisis de Resultados

### Comparación de los diseños Softmax

| Diseño       | Critical Path Delay (ns) | Max Frequency | Latency (cycles) | LUT Usage | FF Usage | DSP Usage | BRAM Usage |
| ------------ | ------------------------ | ------------- | ---------------- | --------- | -------- | --------- | ---------- |
| Simple       | 2.920                    | 342.47 MHz    | 1199             | 3928      | 3084     | 9         | 17         |
| Optimizado 1 | 2.920                    | 342.47 MHz    | 999              | 4064      | 3093     | 7         | 17         |
| Optimizado 2 | 2.920                    | 342.47 MHz    | 924              | 4128      | 3322     | 7         | 16         |



El diseño optimizado reduce la latencia de 1199 a 999 y 924 ciclos (diseño base contra diseños optimizados 1 y 2), lo que significa que la operación completa de Softmax se ejecuta más rápido. Esto se debe a las directivas HLS aplicadas (PIPELINE y UNROLL, así como PARTITION), que permiten ejecutar múltiples operaciones en paralelo dentro del hardware, acelerando la ejecución sin depender de un aumento de frecuencia.

Las LUTs y FFs aumentan ligeramente en el diseño optimizado, porque la paralelización requiere más lógica de control y almacenamiento intermedio. Esto es incluso más visible con la segunda propuesta de optimización, donde el LUT y FF Usage continua aumentando. 

El uso de DSP disminuye, indicando que el optimizador HLS está reutilizando más eficientemente las unidades de punto flotante disponibles.
Con respecto a la BRAM, esta se mantiene constante, ya que ambos diseños requieren almacenar la misma cantidad de datos (100 entradas y 100 salidas).

Los diseños, en general, alcanzan o superan la frecuencia objetivo de 250 MHz, mostrando que la optimización no afecta la estabilidad del reloj. 

La optimización mejora el throughput, es decir, la cantidad de operaciones completadas por segundo, sin comprometer la precisión del Softmax analizado en este proyecto.

## Inteligencia Artificial
Las herramientas de Inteligencia Artificial se utilizaron para hacer más robustos los bancos de prueba, así como para apoyar la comprensión de conceptos y profundizar en detalles de los pragmas aplicados en el diseño optimizado. 

## Árbol de archivos

```
├── README.md
└── design1/src/
    ├── script.tcl
    ├── softmax.cpp
    ├── softmax.h
    ├── softmax_tb.cc
└── design2/src/
    ├── script.tcl
    ├── softmax.cpp
    ├── softmax.h
    ├── softmax_tb.cc
└── design3/src/
    ├── script.tcl
    ├── softmax.cpp
    ├── softmax.h
    ├── softmax_tb.cc
```
Donde: 
- script.tcl: Es el script de automatización para Vitis HLS. Crea el proyecto HLS, agrega los archivos fuente y testbench, además, configura la solución para AMD Kria KV260 a 250 MHz, y ejecuta simulación y síntesis.
- softmax.cpp: Es el archivo fuente principal en C++ del acelerador Softmax.
- softmax.h: Es el header del archivo softmax.cpp. Declara la función softmax y sus prototipos, incluyendo interfaces AXI y tipos de datos.
- softmax_tb.cc: Inicializa los datos de entrada, llama a la función softmax y verifica la salida (positividad y suma ~ 1) antes de la síntesis.

Los diseños tienen archivos con el mismo nombre para simplificación de la estructura y lograr comparar los diseños de una manera más rápida. Pero es importante aclarar que el "design1" es el diseño base y el "design2" es el diseño optimizado.



</h4> <hr style="border: 1px solid #000;"/>

## Información general

Curso de Diseño avanzado con FPGAs

Programa de Maestría en Electrónica

Tecnológico de Costa Rica

Profesor: Luis León Vega (l.leon@itcr.ac.cr)


### Estudiantes

- Arturo Córdoba   (arturocv16@estudiantec.cr)
- Jill Carranza    (gcarranza@estudiantec.cr)
- Juan Pablo Ureña (juurena@estudiantec.cr)
- Víctor Sánchez   (vicsma2409@estudiantec.cr)

## Repositorio

https://github.com/acordobav/softmax-hls
