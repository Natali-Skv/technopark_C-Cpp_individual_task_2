## дисциплина "углубленное программирование на C/C++"
### Индивидуальное задание 2 
### Вариант #58
Сравните и выведите в консоль время работы последовательного и параллельного с использованием нескольких потоков алгоритмов,  
каждый из которых выделяет в динамической памяти числовой массив размером 100 Мб и выполняет последовательный поиск в нем максимальной
строго возрастающей последовательности. 

***

### Описание
ИЗ2 посвящено приобретению навыков системной разработки на C и работы с внешними библиотеками. В качестве результата ИЗ2 ожидается:
* грамотное разбиение проекта на файлы;
* наличие двух реализаций – последовательной и параллельной, оформленных в виде статической и динамической библиотек соответственно, а также тестирующей программы, которая будет сравнивать на разных входных данных результаты обеих реализаций между собой;
* максимальная утилизация ресурсов процессора при параллельной обработке данных путём использования нескольких процессов или потоков;
* продуманные структуры данных в целях экономии оперативной памяти;
* реализация алгоритмов, эффективно взаимодействующих с кэш-памятью.


### Требования к оформлению:
- компиляция должна происходить с флагами -Wall -Werror -Wpedantic, то есть необработанных ворнингов быть не должно;
- количество потоков/процессов должно быть не захардкожено, а определяться в зависимости от возможностей системы (например, в зависимости от количества ядер процессора);
- при разработке обеих библиотек стоит делать общий интерфейс, не раскрывая особенностей реализации;
- библиотеки должны быть взаимозаменяемыми - конкретная реализация (последовательная/параллельная) - использоваться в зависимости от конфигурации сборки;
- юнит-тесты должны быть реализованы для обеих реализаций (последовательной/параллельной). Покрытие тестами должно быть максимально возможным;
- должны присутствовать стресс-тесты. Они могут быть реализованы внешним образом, запуская две разные программы - одну со статической библиотекой с последовательной реализацией, вторую - с динамической библиотекой с параллельной реализацией, и сравнивая их выводы друг с другом.
- для организации ввода/вывода больших данных полезно работать с файлами, а в программе - предусмотреть работу с универсальными потоками входных/выходных данных (или хотя бы перенаправлять ввод/вывод на уровне их запуска)
- если в задании сказано, что программа должна обрабатывать файлы объёмом 100 Мб – это лишь ориентир, на которых программа точно должна работать, и на котором имеет смысл делать замеры производительности и эффективности алгоритмов. Поэтому тесты на такой объём должны быть. Однако сама программа должна уметь работать с произвольными размерами входных данных
- измерение времени должно осуществляться внешним образом, а не внутри кода библиотек. При этом необходимо делать несколько замеров и усреднять. Стоит помнить о том, что clock() в многопоточных приложениях работает не так, как ожидается.
