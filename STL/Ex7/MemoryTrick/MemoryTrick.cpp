#include <iostream>
#include <vector>
using namespace std;

int main() {
    // Инициализируем вектор целых чисел
    vector<int> v;

    // Переменная для отслеживания изменения вместимости (capacity)
    int capacity = v.capacity();
    cout << "Initial size: " << v.size() << ", capacity: " << v.capacity() << endl;

    // Переменная для подсчета количества изменений вместимости
    int count = 0;

    // Добавляем 1000 элементов в вектор
    for (int i = 0; i < 1000; ++i) {
        v.push_back(1);

        // Проверяем, изменилась ли вместимость
        if (capacity != v.capacity()) {
            // Выводим размер и новую вместимость
            cout << "size: " << v.size() << ", capacity: " << v.capacity() << endl;
            capacity = v.capacity(); // Обновляем текущую вместимость
            ++count; // Увеличиваем счетчик изменений вместимости
        }
    }

    // Выводим общее количество изменений вместимости
    cout << "Total capacity changes: " << count << endl;

    return 0;
}


