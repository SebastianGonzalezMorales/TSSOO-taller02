**Autor:** Sebastián González Morales

**Correo institucional:** Sebastian.gonzalez@alumnos.uv.cl

**Diseño de la solución**


El problema está compuesto en dos módulos, el primero consiste en llenar un arreglo unidimensional de números aleatorios del tipo uint32_t y el segundo debe sumar el contenido del arreglo. Posteriormente se implementará y luego se realizarán pruebas de desempeño para ver el comportamiento del tiempo de ejecución de ambos módulos según el tamaño del arreglo, la cantidad de threads utilizados y el rango de números aleatorios. La implementación será en C++ versión 2017.
Tanto el tamaño del arreglo, la cantidad de threads utilizados y el rango de números aleatorios serán en forma dinámica, configurable a través de parámetros de entrada del programa y es global al proceso. Esto quiere decir que es visible para el número de threas creados dentro de él. La solución de la primera parte del módulo se denomina “Etapa de llenado”. Cada thread conoce los índices de inicio y fin donde debe almacenar un número aleatorio.
A medida que se va llenando el arreglo global, la segunda parte del modulo ira sumando los números, esta parte tiene el nombre de “Etapa de suma”.

**Etapa de llenado**

El llenado del arreglo global g_numbers[], se realizará a través de una función denominada fillArray, que tiene dos parámetros: uno que indica el índice de inicio (beginIdx) y otro para el índice de fin (endIdx). Esto permite que la función sea llamada en forma secuencial o parcial por los threads. Esta función tendrá un generador de número aleatorios (RNE, Random Number Engine) con dos parámetros de entrada (min,max) las cuales indicaran el rango mínimo y máximo de los números que almacenara él arreglo. Para las pruebas de funcionamiento, se probará con la función std::uniform_int_distribution<> disponible en la biblioteca <random>.

**Estapa de suma**
  
Esta etapa se realizará a través de una función llamada sumas parciales, la cual tendrá cuatro parámetros de entrada, el vector con los números a sumar, una variable llamada suma que ira almacenando la suma y los índices de inicio y fin de los números que irán sumando el thread.
Luego en la función principal se iran sumando las sumas parciales para obtener la suma total.
