
### Proyecto corto 2

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

</h4> <hr style="border: 1px solid #000;"/>  Optimizaciones adicionales para determinar si se obtiene mejores resultados que el Diseño 2 

#### Diseño 3 : Codificación con optimizaciones HLS
En este diseño se aplicaron directivas de optimización HLS, tales como: `#pragma HLS PIPELINE`, `#pragma HLS UNROLL` y `#pragma HLS ARRAY PARTITION`.
Con el objetivo de reducir la latencia total y mejorar, de igual manera que en el diseño 2, el throughput. En este caso en particular, la directiva "ARRAY PARTITION" permite tomar el arreglo de datos y particionarlo, de modo que se puedan establecer paralelismo a nivel de datos.

#### Diseño 4 : Codificación con optimizaciones HLS
En este diseño se aplicaron directivas más agresivas como `#pragma HLS PIPELINE`, `#pragma HLS UNROLL` y `#pragma HLS ARRAY_PARTITION`.
Se busca bajar aún más la latencia ejecutando varias operaciones al mismo tiempo, pero esto también aumenta el consumo de recursos. Por eso se evalúa si el aumento en área realmente vale la pena en términos de desempeño.

#### Diseño 5 : Codificación con optimizaciones HLS
En este diseño se aplicaron directivas como `#pragma HLS PIPELINE`, `#pragma HLS UNROLL`, `#pragma HLS ARRAY_PARTITION`, `#pragma HLS BIND_STORAGE` y `#pragma HLS ALLOCATION`.
A diferencia del Diseño 4, aquí la idea no es paralelizar todo al máximo, sino encontrar un mejor balance entre latencia y recursos. Se controla la memoria y la cantidad de instancias que se usan, para evitar gastar más hardware del necesario.

</h4> <hr style="border: 1px solid #000;"/> 

### Testbench para los diseños

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

### Evaluación adicional (otras propuestas de optimización)

Además del diseño base y la propuesta optimizada seleccionada, se implementaron variantes adicionales con el objetivo de analizar hasta qué punto era posible mejorar el desempeño mediante mayor paralelización o un control más explícito de recursos usando pragmas HLS.

El propósito de esta exploración fue identificar el punto donde el incremento de paralelismo deja de generar beneficios significativos en latencia y comienza a impactar el área.


### Comparación completa de los diseños Softmax

| Diseño Softmax          | Critical Path Delay (ns) | Max Frequency | Latency (cycles) | LUT Usage | FF Usage | DSP Usage | BRAM Usage |
| ----------------------- | ------------------------ | ------------- | ---------------- | --------- | -------- | --------- | ---------- |
| Diseño 1 (simple)       | 2.920                    | 342.47 MHz    | 1199             | 3928      | 3084     | 9         | 17         |
| Diseño 2 (optimizado)   | 2.920                    | 342.47 MHz    | 999              | 4064      | 3093     | 7         | 17         |
| Diseño 3 (optimizado)   | 2.920                    | 342.47 MHz    | 924              | 4128      | 3322     | 7         | 16         |
| Diseño 4 (optimizado)   | 2.920                    | 342.47 MHz    | 925              | 7004      | 6201     | 19        | 32         |
| Diseño 5 (optimizado)   | 2.920                    | 342.47 MHz    | 924              | 4227      | 3353     | 7         | 24         |

La aplicación progresiva de directivas HLS permitió reducir la latencia desde 1199 ciclos en el diseño base hasta 924 ciclos en los diseños más optimizados. Esta mejora se logra principalmente mediante PIPELINE, UNROLL y ARRAY_PARTITION, que incrementan el paralelismo interno del acelerador sin necesidad de aumentar la frecuencia de operación.

El Diseño 2 fue la primera optimización aplicada. Aquí la latencia baja de 1199 a 999 ciclos con un aumento muy leve en LUTs y FFs, e incluso con una ligera reducción en DSPs. Esto demuestra que aplicar pipeline y unroll ya genera una mejora importante sin impactar significativamente el área. Es decir, es una optimización eficiente y de bajo costo en recursos.

Es importante notar que el critical path delay y la frecuencia máxima se mantienen iguales en todos los diseños. Esto significa que las mejoras no vienen propiamente del reloj, sino de organizar mejor el hardware internamente y aprovechar más el paralelismo.

El Diseño 3 logra reducir la latencia hasta 924 ciclos con un incremento moderado de recursos, convirtiéndose en el mejor punto de equilibrio entre desempeño y área.

Por otro lado, el Diseño 4 muestra un caso claro de sobre–paralelización: aunque la latencia es similar (925 ciclos), el consumo de LUTs, FFs, DSPs y BRAM aumenta considerablemente sin una mejora real en desempeño.

El Diseño 5 también alcanza los 924 ciclos, pero presenta un mayor uso de BRAM. Sin embargo, ese incremento en memoria no genera una mejora adicional en latencia, por lo que no resulta más conveniente que el Diseño 3.

Por lo tanto, el Diseño 3 se selecciona como la propuesta optimizada final, ya que logra la menor latencia con un crecimiento controlado de recursos. En este punto, agregar más paralelismo ya no genera mejoras significativas y solo incrementa el área.

Lo cual genera la tabla definitiva comparando el diseño base Softmax y su optimización mediante pragmas HLS mostrada a continuación: 

### Comparación final de los diseños seleccionados Softmax 

| Diseño Softmax          | Critical Path Delay (ns) | Max Frequency | Latency (cycles) | LUT Usage | FF Usage | DSP Usage | BRAM Usage |
| ----------------------- | ------------------------ | ------------- | ---------------- | --------- | -------- | --------- | ---------- |
| Diseño 1 (simple)       | 2.920                    | 342.47 MHz    | 1199             | 3928      | 3084     | 9         | 17         |
| Diseño 3 (optimizado)   | 2.920                    | 342.47 MHz    | 924              | 4128      | 3322     | 7         | 16         |

En general, se evidencia que el uso adecuado de pragmas HLS mejora el throughput del acelerador sin afectar la frecuencia objetivo de 250 MHz, y que existe un punto óptimo donde el balance entre paralelismo y recursos es el más conveniente.

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
