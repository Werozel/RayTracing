Трассировка лучей
15 база
2 за примитив куба, треугольника и конечной/бесконечной плоскости
4 за использование 3D моделей (2 и 3 сцены, 
                               500 и 2300 полигонов)
2 за использования 4-х разных материалов (+2 за стекло)
1 за использование текстур
1 за преломления
1 за устранение ступенчатости (2 сцена)
2 за субъективную реалистичность сцены (1 сцена - пузыри под водой)
2 за приятный внешний вид
1 за использование карты окружения
2 за использование многопоточности (OpenMP)

Сцены сохраняются в формате png! (Использую библиотеку stb)
Примеры сцен - в папке scenes


Результаты тестов:
Intel Core i5-8250U CPU @ 1.60GHz × 8 
32 threads
1: 0.33s
2: 104.5s
3: 274.5s

10 threads
1: 0.45s
2: 122.2s
3: 345.5s


Источники:
https://www.scratchapixel.com/lessons/3d-basic-rendering/ray-tracing-overview/light-transport-ray-tracing-whitted
https://habr.com/ru/post/436790/
http://www.ray-tracing.ru/articles213.html
http://masters.donntu.org/2015/frt/yablokov/library/transl.htm
http://kvant.mccme.ru/pdf/2003/02/23.pdf
https://github.com/nothings/stb
