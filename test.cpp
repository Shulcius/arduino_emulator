#include <iostream>
#include <vector>
#include <chrono>
#include <thread>
#include <random>

// Constants for the pin operation mode
const int INPUT = 0x0;
const int OUTPUT = 0x1;
const int INPUT_PULLUP = 0x2;

// Simulation of pin states
enum PinState { LOW = 0x0, HIGH = 0x1 };

// Structure for storing pin settings
struct PinSettings {
    int pin_num;
    int mode;
    bool active = false;
};

// A class for managing pins
class Arduino {
public:
    void pinMode(int pin_num, int mode) {
        PinSettings newPin = { .pin_num = pin_num, .mode = mode };
        pins.emplace_back(newPin);
    }

    void setup() {
        for (auto &pin : pins) {
            addPin(pin);
        }
    }

    void digitalWrite(int pin_num, PinState state) {
        auto it = findPinBypin_num(pin_num);
        if (it != pins.end()) {
            it->active = (state == HIGH);
            setDigitalOutput(*it, state);
        }
    }

    void analogWrite(int pin_num, int value) {
        auto it = findPinBypin_num(pin_num);
        if (it != pins.end()) {
            setAnalogOutput(*it, value);
        }
    }

    // Adding a method for reading data from a pin
    int digitalRead(int pin_num) {
        auto it = findPinBypin_num(pin_num);
        if (it != pins.end()) {
            return it->active ? HIGH : LOW;
        }
        return LOW;  // По умолчанию возвращаем LOW
    }

private:
    std::vector<PinSettings> pins;

    void addPin(const PinSettings& settings) {
        std::cout << "Configuring pin " << settings.pin_num << " as ";
        switch(settings.mode) {
            case INPUT:
                std::cout << "input\n";
                break;
            case INPUT_PULLUP:
                std::cout << "input_pullup\n";
                break;
            case OUTPUT:
                std::cout << "output\n";
                break;
            default:
                std::cerr << "Invalid pin mode!\n";
                exit(EXIT_FAILURE);
        }
    }

    void setDigitalOutput(const PinSettings& settings, int state) {
        if (settings.mode != OUTPUT) {
            std::cerr << "Pin " << settings.pin_num << " is not an output pin!\n";
            return;
        }

        if (state == HIGH) {
            std::cout << "pin ON on " << settings.pin_num << "\n";
        } else {
            std::cout << "pin OFF on " << settings.pin_num << "\n";
        }
    }

    void setAnalogOutput(const PinSettings& settings, int value) {
        if (settings.mode != OUTPUT) {
            std::cerr << "Pin " << settings.pin_num << " is not an output pin!\n";
            return;
        }

        std::cout << "Analog write " << value << " to pin " << settings.pin_num << "\n";
    }

    std::vector<PinSettings>::iterator findPinBypin_num(int pin_num) {
        return std::find_if(
            pins.begin(), pins.end(),
                [&](const PinSettings& pin) {
                    return pin.pin_num == pin_num;
                });
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
    Arduino uno;

    uno.pinMode(2, OUTPUT);
    uno.setup();

    while (true) {
        uno.digitalWrite(2, HIGH);
        delay(500);
    }
}