#include <iostream>
#include <vector>
#include <chrono>
#include <thread>
#include <random>  // Для генерации случайных значений

// Константы для режима работы пинов
const int INPUT = 0;
const int OUTPUT = 1;

// Симуляция состояний пина
enum PinState { LOW = 0, HIGH = 1 };

// Структура для хранения настроек пина
struct PinSettings {
    int number;          // Номер пина
    int mode;            // Режим работы пина (INPUT или OUTPUT)
    bool active = false; // Флаг активности пина (для симуляции состояний)
};

// Класс для управления пинами
class PinsManager {
public:
    void addPin(int number, int mode) {
        PinSettings newPin = { .number = number, .mode = mode };
        pins.emplace_back(newPin);
    }

    void configurePins() {
        for (auto &pin : pins) {
            pinMode(pin);
        }
    }

    void setDigitalOutput(int pinNumber, PinState state) {
        auto it = findPinByNumber(pinNumber);
        if (it != pins.end()) {
            (*it).active = (state == HIGH);
            digitalWrite(*it, state);
        }
    }

    void setAnalogOutput(int pinNumber, int value) {
        auto it = findPinByNumber(pinNumber);
        if (it != pins.end()) {
            analogWrite(*it, value);
        }
    }

    // Добавляем метод для чтения данных с пина
    int readInput(int pinNumber) {
        auto it = findPinByNumber(pinNumber);
        if (it != pins.end()) {
            return (*it).active ? HIGH : LOW;
        }
        return LOW;  // По умолчанию возвращаем LOW
    }

private:
    std::vector<PinSettings> pins;

    void pinMode(const PinSettings& settings) {
        std::cout << "Configuring pin " << settings.number << " as ";
        switch(settings.mode) {
            case INPUT:
                std::cout << "input\n";
                break;
            case OUTPUT:
                std::cout << "output\n";
                break;
            default:
                std::cerr << "Invalid pin mode!\n";
                exit(EXIT_FAILURE);
        }
    }

    void digitalWrite(const PinSettings& settings, int state) {
        if (settings.mode != OUTPUT) {
            std::cerr << "Pin " << settings.number << " is not an output pin!\n";
            return;
        }

        if (state == HIGH) {
            std::cout << "LED ON on pin " << settings.number << "\n";
        } else {
            std::cout << "LED OFF on pin " << settings.number << "\n";
        }
    }

    void analogWrite(const PinSettings& settings, int value) {
        if (settings.mode != OUTPUT) {
            std::cerr << "Pin " << settings.number << " is not an output pin!\n";
            return;
        }

        std::cout << "Analog write " << value << " to pin " << settings.number << "\n";
    }

    std::vector<PinSettings>::iterator findPinByNumber(int pinNumber) {
        return std::find_if(pins.begin(), pins.end(),
                            [&](const PinSettings& pin) { return pin.number == pinNumber; });
    }
};

// Симуляция функций Arduino
void delay(unsigned long ms) {
    std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}

void delayMicroseconds(unsigned int us) {
    std::this_thread::sleep_for(std::chrono::microseconds(us));
}

// Имитация получения данных от ультразвукового датчика
unsigned long pulseIn(int pin, int state) {
    // Симуляция получения импульса, возвращаемого датчиком расстояния HC-SR04
    // Генерация случайного значения в пределах 2-30 мс
    static std::mt19937 rng(std::random_device{}());
    static std::uniform_int_distribution<> dist(2000, 30000);
    return dist(rng);
}

// Классы и методы для эмуляции работы с Serial
class Serial {
public:
    static void begin(long baudRate) {
        std::cout << "Initializing serial communication at " << baudRate << " baud rate.\n";
    }

    static void print(const char* message) {
        std::cout << message;
    }

    static void println(const char* message) {
        std::cout << message << '\n';
    }

    template<typename T>
    static void print(T value) {
        std::cout << value;
    }

    template<typename T>
    static void println(T value) {
        std::cout << value << '\n';
    }
};

int main() {
    PinsManager manager;
    int PIN_TRIG = 12;
    int PIN_ECHO = 13;
    // Настройка пинов
    manager.addPin(PIN_TRIG, OUTPUT);
    manager.addPin(PIN_ECHO, INPUT);
    manager.configurePins();
    // Основной цикл
    while (true) {
        // Генерируем короткий импульс
        manager.setDigitalOutput(PIN_TRIG, LOW);
        delayMicroseconds(5);
        manager.setDigitalOutput(PIN_TRIG, HIGH);
        // Ждем 10 микросекунд
        delayMicroseconds(10);
        manager.setDigitalOutput(PIN_TRIG, LOW);
        // Измеряем время возврата сигнала
        long duration = pulseIn(PIN_ECHO, HIGH);
        // Преобразуем время в расстояние
        long cm = (duration / 2) / 29.1;
        // Выводим результат измерения расстояния
        Serial::print("Расстояние до объекта: ");
        Serial::print(cm);
        Serial::println(" см.");
        // Задержка между измерениями
        delay(250);
    }

    return 0;
}