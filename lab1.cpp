#include <iostream>
#include <vector>
#include <random>
#include <chrono>
#include <fstream>
#include <algorithm>

using namespace std;
using namespace chrono;

/**
 * Функция сортировки методом выбора.
 * Выполняет сортировку массива и подсчитывает количество проходов и перестановок.
 *
 * @param arr Вектор чисел для сортировки.
 * @param passCount Счетчик количества проходов по массиву.
 * @param swapCount Счетчик количества перестановок элементов.
 *
 * @return Нет, результат сортировки происходит по месту.
 */
void selectionSort(vector<double>& arr, long long& passCount, long long& swapCount) {
    int n = arr.size();  // Размер массива
    passCount = 0;  // Инициализация счетчика количества проходов
    swapCount = 0;  // Инициализация счетчика количества перестановок

    // Внешний цикл, проходящий по всем элементам массива
    for (int i = 0; i < n - 1; ++i) {
        int minIndex = i;  // Индекс минимального элемента в текущем проходе
        // Внутренний цикл для поиска минимального элемента в оставшейся части массива
        for (int j = i + 1; j < n; ++j) {
            ++passCount;  // Увеличиваем счетчик проходов
            if (arr[j] < arr[minIndex]) {
                minIndex = j;  // Обновляем индекс минимального элемента
            }
        }
        // Если минимальный элемент не на текущей позиции, выполняем обмен
        if (minIndex != i) {
            swap(arr[i], arr[minIndex]);  // Меняем местами элементы
            ++swapCount;  // Увеличиваем счетчик перестановок
        }
    }
}

/**
 * Функция для тестирования сортировки методом выбора для нескольких массивов.
 * Выполняет несколько прогонов для массива каждого размера и записывает результаты в файл.
 *
 * @param arraySize Размер массива для сортировки.
 * @param numRuns Количество прогонов для каждого размера массива.
 * @param outFile Поток вывода, в который записываются результаты.
 */
void runSortingTest(int arraySize, int numRuns, ofstream& outFile) {
    mt19937 engine(time(0));  // Генератор случайных чисел, инициализируем его временем
    uniform_real_distribution<double> gen(-1.0, 1.0);  // Диапазон случайных чисел от -1 до 1

    vector<double> times;  // Вектор для хранения времени выполнения для каждого прогона
    double minTime = numeric_limits<double>::max();  // Инициализация минимального времени
    double maxTime = 0;  // Инициализация максимального времени
    double totalTime = 0;  // Общее время всех прогонов
    long long totalPassCount = 0;  // Общее количество проходов
    long long totalSwapCount = 0;  // Общее количество перестановок

    // Многократный прогон сортировки для получения статистики
    for (int i = 0; i < numRuns; ++i) {
        vector<double> arr(arraySize);  // Создаем массив заданного размера
        // Заполняем массив случайными числами
        for (auto& el : arr) {
            el = gen(engine);
        }

        long long passCount = 0, swapCount = 0;  // Локальные счетчики для текущего прогона
        auto start = high_resolution_clock::now();  // Засекаем время начала сортировки
        selectionSort(arr, passCount, swapCount);  // Выполняем сортировку
        auto end = high_resolution_clock::now();  // Засекаем время окончания сортировки

        // Вычисляем время выполнения текущего прогона
        duration<double, milli> elapsed = end - start;
        times.push_back(elapsed.count());  // Добавляем время в список
        totalTime += elapsed.count();  // Обновляем общее время
        minTime = min(minTime, elapsed.count());  // Обновляем минимальное время
        maxTime = max(maxTime, elapsed.count());  // Обновляем максимальное время
        totalPassCount += passCount;  // Добавляем количество проходов
        totalSwapCount += swapCount;  // Добавляем количество перестановок

        // Выводим результаты для текущего прогона
        cout << "Run " << i + 1 << " - Time: " << elapsed.count() << " ms, Passes: " << passCount << ", Swaps: " << swapCount << endl;
        outFile << arraySize << "," << elapsed.count() << "," << passCount << "," << swapCount << endl;  // Записываем результаты в файл
    }

    // Рассчитываем средние значения
    double avgTime = totalTime / numRuns;
    double avgPasses = totalPassCount / numRuns;
    double avgSwaps = totalSwapCount / numRuns;

    // Записываем статистику в файл
    outFile << arraySize << ",AVG," << avgTime << "," << avgPasses << "," << avgSwaps << endl;
    outFile << arraySize << ",MIN," << minTime << ",0,0" << endl;
    outFile << arraySize << ",MAX," << maxTime << ",0,0" << endl;

    // Сообщаем, что завершили тестирование для текущего размера массива
    cout << "\nCompleted series for array size " << arraySize << "\n";
}

int main() {
    // Массив размеров для тестирования
    vector<int> sizes = {1000, 2000, 4000, 8000, 16000, 32000, 64000, 128000};
    int numRuns = 20;  // Количество прогонов для каждого размера массива

    // Открываем файл для записи результатов
    ofstream outFile("sorting_results.csv");
    outFile << "ArraySize,Type,Time(ms),Passes,Swaps" << endl;  // Записываем заголовки в файл

    // Запускаем тесты сортировки для каждого размера массива
    for (int size : sizes) {
        runSortingTest(size, numRuns, outFile);  // Запуск теста для данного размера массива
        cout << "--------------------------------------\n";  // Разделитель для каждого теста
    }

    // Закрываем файл после завершения всех тестов
    outFile.close();
    return 0;  // Завершаем программу
}

